
#pragma once
#include <slankdev/dpdk.h>


static bool filter(const rte_mbuf* m)
{
	uint8_t* p = rte_pktmbuf_mtod(m, uint8_t*);
	if (p[12] == 0x08 && p[13]==0x06) {
		return true;
	} else {
		return false;
	}
}



using function = int(*)(void*);
using func_arg = void*;
class Cpu {
public:
	uint8_t lcore_id;
	function func;
	func_arg arg;

	Cpu() : lcore_id(0), func(nullptr), arg(nullptr) {}

	void boot(uint8_t id)
	{
		lcore_id = id;
	}
	void configure() {}
	void show_state()
	{
		printf("   - lcore%u : %p(%p) \n", lcore_id, func, arg);
	}
	void launch()
	{
		rte_eal_remote_launch(func, arg, lcore_id);
	}
};


template <class T>
class Ring {
    struct rte_ring* ring_;
	size_t ring_depth;
public:
    Ring() : ring_(nullptr), ring_depth(0) {}
    void init(const char* name, size_t count, uint16_t socket_id)
    {
		ring_depth = count;
        ring_ = rte_ring_create(name, count, socket_id, 0);
        if (!ring_)
            throw slankdev::exception("rte_ring_create");
    }
    ~Ring()
    {
        if (!ring_)
            rte_ring_free(ring_);
    }
    void push(T* data)
    {
        int ret = rte_ring_enqueue(ring_, data);
		if (ret < 0) {
			if (ret == -EDQUOT) {
				throw slankdev::exception("rte_ring_enqueue: quota exceeded");
			} else if (ret == -ENOBUFS) {
				// throw slankdev::exception("rte_ring_enqueue: no buf");
			} else {
				throw slankdev::exception("rte_ring_enqueue: unknown");
			}
		}
    }
    void pop(T** data)
    {
        int ret = rte_ring_dequeue(ring_, reinterpret_cast<void**>(data));
        if (ret < 0) {
			if (ret == -ENOENT) {
				// throw slankdev::exception("rte_ring_dequeue: no ent");
			} else {
				throw slankdev::exception("rte_ring_dequeue: unknown");
			}
		}

    }
	size_t count()
	{
        return rte_ring_count(ring_);
	}
    size_t size()
    {
		return ring_depth;
    }
};



class Port {
public:
	uint8_t port_id;
	slankdev::ether_addr addr;
	slankdev::pool* mp_;
	std::vector<Ring<rte_mbuf>> rxq;
	std::vector<Ring<rte_mbuf>> txq;

	size_t nb_rx_rings;
	size_t nb_tx_rings;
	size_t rx_ring_size;
	size_t tx_ring_size;
	Port() :
		nb_rx_rings (1),
		nb_tx_rings (1),
		rx_ring_size(128),
		tx_ring_size(512) {}

	void boot(uint8_t id)
	{
		port_id = id;
		rte_eth_macaddr_get(id, &addr);
	}
	void configure(slankdev::pool* mp)
	{
		mp_ = mp;
		slankdev::port_conf conf;
		port_init(port_id, mp, &conf, nb_rx_rings, nb_tx_rings, rx_ring_size, tx_ring_size);

		rxq.resize(nb_rx_rings);
		txq.resize(nb_tx_rings);
		for (size_t i=0; i<nb_rx_rings; i++) {
			std::string ringname =
				"port" + std::to_string(port_id) + " rxring" + std::to_string(i);
			rxq[i].init(ringname.c_str(), rx_ring_size, 0);
		}
		for (size_t i=0; i<nb_tx_rings; i++) {
			std::string ringname =
				"port" + std::to_string(port_id) + " txring" + std::to_string(i);
			txq[i].init(ringname.c_str(), tx_ring_size, 0);
		}
	}
	void show_config()
	{
		printf("Port%u config\n", port_id);
		printf(" * nb_rx_rings : %zd \n", nb_rx_rings );
		printf(" * nb_tx_rings : %zd \n", nb_tx_rings );
		printf(" * rx_ring_size: %zd \n", rx_ring_size);
		printf(" * tx_ring_size: %zd \n", tx_ring_size);
	}
	void show_state()
	{
		char str[32];
		addr.sprint(str);
		printf("   - port%u : %s\n", port_id, str);

		uint16_t nb_q;
		/*
		 * Rx
		 */
		nb_q = rxq.size();
		for (uint16_t qid=0; qid<nb_q; qid++) {
			printf("     - rxqueue%u: %zu/%zu \n", qid,
					rxq[qid].count(),
					rxq[qid].size());
		}

		/*
		 * Tx
		 */
		nb_q = txq.size();
		for (uint16_t qid=0; qid<nb_q; qid++) {
			printf("     - txqueue%u: %zu/%zu \n", qid,
					txq[qid].count(),
					txq[qid].size());
		}
	}
	void rx_burst()
	{
		const uint8_t nb_rxq = rxq.size();
		for (uint8_t qid=0; qid<nb_rxq; qid++) {
			const size_t burst_size = 32;
			struct rte_mbuf* rx_pkts[burst_size];
			uint16_t nb_rx = rte_eth_rx_burst(port_id, qid, rx_pkts, burst_size);
			for (uint16_t i=0; i<nb_rx; i++) {
#if 1
				if (filter(rx_pkts[i])) rxq[qid].push(rx_pkts[i]);
#else
				rxq[qid].push(rx_pkts[i]);
#endif
			}
		}
	}
	void tx_burst()
	{
		const uint8_t nb_txq = txq.size();
		for (uint8_t qid=0; qid<nb_txq; qid++) {
			while (txq[qid].size() > 0) {
				struct rte_mbuf* m = nullptr;
				txq[qid].pop(&m);
				if (m == nullptr) {
					break;
				}
				rte_eth_tx_burst(port_id, qid, &m, 1);
			}
		}
	}
};



class System {
public:
	std::vector<Cpu>  cpus;
	std::vector<Port> ports;
	slankdev::pool    mp;

    ~System()
    {
		rte_eal_mp_wait_lcore();
    }

	void boot(int argc, char** argv)
	{
		slankdev::dpdk_boot(argc, argv);

		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();
		cpus.resize(nb_cpus);
		ports.resize(nb_ports);

		for (uint8_t i=0; i<nb_cpus; i++) {
			cpus[i].boot(i);
		}
		for (uint16_t i=0; i<nb_ports; i++) {
			ports[i].boot(i);
		}
	}
	void configure()
	{
		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();

		size_t mbuf_cache_size = 0;
		size_t mbuf_siz = RTE_MBUF_DEFAULT_BUF_SIZE;
		size_t num_mbufs = 8192;
		mp.create(
				"Pool0",
				num_mbufs * rte_eth_dev_count(),
				mbuf_cache_size, mbuf_siz,
				rte_socket_id()
		);

		for (uint8_t i=0; i<nb_cpus; i++) {
			cpus[i].configure();
		}
		for (uint16_t i=0; i<nb_ports; i++) {
			ports[i].configure(&mp);
		}

	}
	void launch()
	{
		/*
		 * The lcore0 is com cpu core.
		 * So it must not launch that.
		 */
		for (size_t i=1; i<cpus.size(); i++) {
			if (cpus[i].func)
				cpus[i].launch();
		}
		rte_eal_mp_wait_lcore();
	}
	void show_config()
	{
		printf("-------------------------------------------------\n");
		printf("System Config\n");
		// printf(" [+] processors\n");
		// for (uint8_t i=0; i<cpus.size(); i++) cpus[i].show_config();
		printf(" [+] interfaces\n");
		for (uint16_t i=0; i<ports.size(); i++) ports[i].show_config();
		printf("-------------------------------------------------\n");
	}
	void show_state()
	{
		printf("-------------------------------------------------\n");
		printf("System\n");
		printf(" [+] processors\n");
		for (uint8_t i=0; i<cpus.size(); i++) cpus[i].show_state();
		printf(" [+] interfaces\n");
		for (uint16_t i=0; i<ports.size(); i++) ports[i].show_state();
		printf("-------------------------------------------------\n");
	}
};

