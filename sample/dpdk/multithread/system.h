
#pragma once
#include <slankdev/dpdk.h>


using function = int(*)(void*);
class Cpu {
public:
	uint8_t lcore_id;
	function func;

	Cpu() : func(nullptr) {}

	void init(uint8_t id)
	{
		lcore_id = id;
	}
	void set_func(function f)
	{
		func = f;
	}
	void launch()
	{
		rte_eal_remote_launch(func, nullptr, lcore_id);
	}
};


class Port {
public:
	uint8_t port_id;
	slankdev::ether_addr addr;
	slankdev::pool* mp_;
	void init(uint8_t id, slankdev::pool* mp,
			size_t nb_rx_rings, size_t nb_tx_rings,
			size_t rx_ring_size, size_t tx_ring_size)
	{
		mp_ = mp;
		port_id = id;
		slankdev::port_conf conf;
		port_init(id, mp, &conf, nb_rx_rings, nb_tx_rings, rx_ring_size, tx_ring_size);

		rte_eth_macaddr_get(id, &addr);

		printf("Port %u MAC: ", (unsigned)id);
		addr.print(stdout);
		printf("\n");
	}
};



class System {
public:
	std::vector<Cpu>  cpus;
	std::vector<Port> ports;

	void init(int argc, char** argv)
	{
		slankdev::dpdk_boot(argc, argv);

		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();

		size_t mbuf_cache_size = 0;
		size_t mbuf_siz = RTE_MBUF_DEFAULT_BUF_SIZE;
		size_t num_mbufs = 8192;
		slankdev::pool mp;
		mp.create(
				"Pool0",
				num_mbufs * rte_eth_dev_count(),
				mbuf_cache_size, mbuf_siz,
				rte_socket_id()
		);

		cpus.resize(nb_cpus);
		ports.resize(nb_ports);

		for (uint8_t i=0; i<nb_cpus; i++) {
			cpus[i].init(i);
		}
		for (uint16_t i=0; i<nb_ports; i++) {
			size_t nb_rx_rings = 1;
			size_t nb_tx_rings = 1;
			size_t rx_ring_size = 128;
			size_t tx_ring_size = 512;
			ports[i].init(i, &mp,
					nb_rx_rings, nb_tx_rings,
					rx_ring_size, tx_ring_size);
		}

	}
	void launch()
	{
		for (size_t i=0; i<cpus.size(); i++) {
			if (cpus[i].func)
				cpus[i].launch();
		}
		rte_eal_mp_wait_lcore();
	}
	void show()
	{
		printf("System\n");
		printf("+ CPUs\n");
		for (uint8_t i=0; i<cpus.size(); i++) {
			printf(" + CPU%u \n", i);
		}
		printf("+ PORTs\n");
		for (uint16_t i=0; i<ports.size(); i++) {
			printf(" + PORT%u \n", i);
		}

	}
};

