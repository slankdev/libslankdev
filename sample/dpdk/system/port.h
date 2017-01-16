

#pragma once
#include "types.h"
#include "ring.h"
#include "log.h"
#include "dpdk_struct_utils.h"


#if 0
static bool filter(const rte_mbuf* m)
{
    uint8_t* p = rte_pktmbuf_mtod(m, uint8_t*);
    if (p[12] == 0x08 && p[13]==0x06) {
        return true;
    } else {
        return false;
    }
}
#endif




namespace dpdk {




class Port {

    class port_conf {
        Port* port;
    public:
        rte_eth_conf raw;
        port_conf(Port* p) : port(p)
        {
            memset(&raw, 0x00, sizeof(raw));
            raw.rxmode.max_rx_pkt_len = ETHER_MAX_LEN;
        }
        void show() { dpdk::util::print(&raw, "portconf"); }
    };

    class port_stats {
        struct rte_eth_stats raw;
        const Port* port;
    public:
        port_stats(Port* p) : port(p) {}
        void reset()  { rte_eth_stats_reset(port->port_id);     }
        void update() { rte_eth_stats_get(port->port_id, &raw); }
        uint64_t ipackets  () const { return raw.ipackets ; }
        uint64_t opackets  () const { return raw.opackets ; }
        uint64_t ibytes    () const { return raw.ibytes   ; }
        uint64_t obytes    () const { return raw.obytes   ; }
        uint64_t imissed   () const { return raw.imissed  ; }
        uint64_t ierrors   () const { return raw.ierrors  ; }
        uint64_t oerrors   () const { return raw.oerrors  ; }
        uint64_t rx_nombuf () const { return raw.rx_nombuf; }
        uint64_t q_ipackets(size_t queue_id) const { return raw.q_ipackets[queue_id]; }
        uint64_t q_opackets(size_t queue_id) const { return raw.q_opackets[queue_id]; }
        uint64_t q_ibytes  (size_t queue_id) const { return raw.q_ibytes  [queue_id]; }
        uint64_t q_obytes  (size_t queue_id) const { return raw.q_obytes  [queue_id]; }
        uint64_t q_errors  (size_t queue_id) const { return raw.q_errors  [queue_id]; }
        void show() { update(); dpdk::util::print(&raw, "port_stats"); }
    };

    class port_info {
        const Port* port;
    public:
        struct rte_eth_dev_info raw;
        port_info(Port* p) : port(p) {}
        void get()
        {
            rte_eth_dev_info_get(port->port_id, &raw);
        }
        void show() { get(); dpdk::util::print(&raw); }
    };
    class ether_addr : public ::ether_addr {
        const Port* port;
    public:
        ether_addr(Port* p) : port(p) {}
        void print(FILE* fd) const { fprintf(fd, "%s", toString().c_str()); }
        std::string toString() const
        {
            char buf[32];
            snprintf(buf, sizeof(buf),
                    "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
                       ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
                    addr_bytes[0], addr_bytes[1],
                    addr_bytes[2], addr_bytes[3],
                    addr_bytes[4], addr_bytes[5]);

            return buf;
        }
        void update() { rte_eth_macaddr_get(port->port_id, this); }
        void set_addr(ether_addr* addr) {
            int ret = rte_eth_dev_default_mac_addr_set(port->port_id, addr);
            if (ret < 0) {
                if (ret == -ENOTSUP) {
                    throw slankdev::exception(
                            "rte_eth_dev_default_mac_addr_set: hardware doesn't suppoer");
                } else if (ret == -ENODEV) {
                    throw slankdev::exception(
                            "rte_eth_dev_default_mac_addr_set: port invalid");
                } else if (ret == -EINVAL) {
                    throw slankdev::exception(
                            "rte_eth_dev_default_mac_addr_set: MAC address is invalid");
                } else {
                    throw slankdev::exception(
                            "rte_eth_dev_default_mac_addr_set: unknown error");
                }
            }
            update();
        }
        void add_addr(ether_addr* addr)
        {
            int ret = rte_eth_dev_mac_addr_add(port->port_id, addr, 0);
            if (ret < 0) {
                if (ret == -ENOTSUP) {
                    throw slankdev::exception(
                    "rte_eth_dev_mac_addr_add: hardware doesn't support this feature.");
                } else if (ret == -ENODEV) {
                    throw slankdev::exception(
                        "rte_eth_dev_mac_addr_add: port is invalid.");
                } else if (ret == -ENOSPC) {
                    throw slankdev::exception(
                        "rte_eth_dev_mac_addr_add: no more MAC addresses can be added.");
                } else if (ret == -EINVAL) {
                    throw slankdev::exception(
                        "rte_eth_dev_mac_addr_add: MAC address is invalid.");
                } else {
                    throw slankdev::exception("rte_eth_dev_mac_addr_add: unknown");
                }
            }
            update();
        }
        void del_addr(ether_addr* addr)
        {
            int ret = rte_eth_dev_mac_addr_remove(port->port_id, addr);
            if (ret < 0) {
                if (ret == -ENOTSUP) {
                    throw slankdev::exception(
                            "rte_eth_dev_mac_addr_remove: hardware doesn't support.");
                } else if (ret == -ENODEV) {
                    throw slankdev::exception(
                            "rte_eth_dev_mac_addr_remove: if port invalid.");
                } else if (ret == -EADDRINUSE) {
                    std::string errstr = "rte_eth_dev_mac_addr_remove: ";
                    errstr += "attempting to remove the default MAC address";
                    throw slankdev::exception(errstr.c_str());
                }
            }
            update();
        }
    };



public:
    std::string name;
    uint8_t port_id;
    ether_addr addr;

    std::vector<Ring> rxq;
    std::vector<Ring> txq;

    pool*      mempool;
    port_conf  conf;
    port_stats stats;
    port_info  info;

    Port() : addr(this), conf(this), stats(this), info(this) {}

    void boot(uint8_t id, dpdk::pool* mp)
    {
        kernel_log(SYSTEM, "boot port%u ", id);

        mempool = mp;
        port_id = id;
        rte_eth_macaddr_get(id, &addr);
        name = "port" + std::to_string(port_id);
        info.get();

        kernel_log(SYSTEM, "address=%s ", addr.toString().c_str());
        kernel_log(SYSTEM, " ... done\n");
    }
    void configure(size_t nb_rx_rings, size_t nb_tx_rings,
            size_t rx_ring_size, size_t tx_ring_size)
    {
        kernel_log(SYSTEM, "configure %s ...\n", name.c_str());

        if (port_id >= rte_eth_dev_count())
            throw slankdev::exception("port is not exist");

        /*
         * Configure the Ethernet device.
         */
        int retval = rte_eth_dev_configure(port_id, nb_rx_rings, nb_tx_rings, &conf.raw);
        if (retval != 0)
            throw slankdev::exception("rte_eth_dev_configure failed");

        /*
         * Allocate and set up RX $nb_rx_rings queue(s) per Ethernet port.
         */
        for (uint16_t qid = 0; qid < nb_rx_rings; qid++) {
            retval = rte_eth_rx_queue_setup(port_id, qid, rx_ring_size,
                    rte_eth_dev_socket_id(port_id), NULL, mempool->get_raw());
            if (retval < 0)
                throw slankdev::exception("rte_eth_rx_queue_setup failed");
        }

        /*
         * Allocate and set up $nb_tx_rings TX queue per Ethernet port.
         */
        for (uint16_t qid = 0; qid < nb_tx_rings; qid++) {
            retval = rte_eth_tx_queue_setup(port_id, qid, tx_ring_size,
                    rte_eth_dev_socket_id(port_id), NULL);
            if (retval < 0)
                throw slankdev::exception("rte_eth_tx_queue_setup failed");
        }

        /*
         * Start the Ethernet port.
         */
        retval = rte_eth_dev_start(port_id);
        if (retval < 0)
            throw slankdev::exception("rte_eth_dev_start failed");

        /*
         * Enable Promiscous mode
         */
        rte_eth_promiscuous_enable(port_id);

        /*
         * Setup DPDK-Rings
         */
        rxq.resize(nb_rx_rings);
        txq.resize(nb_tx_rings);
        for (size_t i=0; i<nb_rx_rings; i++) {
            char ringname[32];
            sprintf(ringname, "port%u-rx%zd", port_id, i);
            rxq[i].init(ringname, rx_ring_size, 0);
        }
        for (size_t i=0; i<nb_tx_rings; i++) {
            char ringname[32];
            sprintf(ringname, "port%u-tx%zd", port_id, i);
            txq[i].init(ringname, tx_ring_size, 0);
        }
        kernel_log(SYSTEM, "configure %s ... done\n", name.c_str());

    }
    void rx_burst()
    {
        const uint8_t nb_rxq = rxq.size();
        for (uint8_t qid=0; qid<nb_rxq; qid++) {
            const size_t burst_size = 32;
            struct rte_mbuf* rx_pkts[burst_size];
            uint16_t nb_rx = rte_eth_rx_burst(port_id, qid, rx_pkts, burst_size);
            for (uint16_t i=0; i<nb_rx; i++) {
                rxq[qid].push(rx_pkts[i]);
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



} /* namespace dpdk */


