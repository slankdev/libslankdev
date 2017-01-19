

#pragma once
#include "mempool.h"
#include "ring.h"
#include "log.h"
#include "struct_utils.h"





namespace dpdk {




class Port {

    /*
     * This class has dynamically infomations.
     */
    class port_conf {
        Port* port;
    public:
        rte_eth_conf raw;
        port_conf(Port* p) : port(p)
        {
            memset(&raw, 0x00, sizeof(raw));
            raw.rxmode.max_rx_pkt_len = ETHER_MAX_LEN;
        }
    };

    /*
     * This class has dynamically informations.
     */
    class port_stats {
        const Port* port;
    public:
        struct rte_eth_stats raw;
        port_stats(Port* p) : port(p) {}
        void reset()  { rte_eth_stats_reset(port->id);       }
        void update() { rte_eth_stats_get  (port->id, &raw); }
    };

    /*
     * This class has statically infomations.
     */
    class dev_info {
        const Port* port;
    public:
        struct rte_eth_dev_info raw;
        dev_info(Port* p) : port(p) {}
        void get()
        {
            rte_eth_dev_info_get(port->id, &raw);
        }
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
        void update() { rte_eth_macaddr_get(port->id, this); }
        void set(::ether_addr* addr)
        {
            int ret = rte_eth_dev_default_mac_addr_set(port->id, addr);
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
        void add(::ether_addr* addr)
        {
            int ret = rte_eth_dev_mac_addr_add(port->id, addr, 0);
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
        void del(::ether_addr* addr)
        {
            int ret = rte_eth_dev_mac_addr_remove(port->id, addr);
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
    const std::string name;
    const uint8_t     id;
    Mempool*          mempool;
    ether_addr        addr;

    std::vector<Ring> rxq;
    std::vector<Ring> txq;

    port_conf         conf;
    port_stats        stats;
    dev_info          info;

    Port(uint8_t pid, dpdk::Mempool* mp,
            size_t rx_ring_size, size_t tx_ring_size) :
        name   ("port" + std::to_string(id)),
        id     (pid),
        mempool(mp),
        addr   (this),
        conf   (this),
        stats  (this),
        info   (this)
    {
        kernel_log(SYSTEM, "boot port%u ... \n", id);
        rte_eth_macaddr_get(id, &addr);
        info.get();

        kernel_log(SYSTEM, "%s address=%s \n", name.c_str(), addr.toString().c_str());

        if (id >= rte_eth_dev_count())
            throw slankdev::exception("port is not exist");

        /*
         * Configure the Ethernet device.
         */
        size_t nb_rx_rings = 1;
        size_t nb_tx_rings = 1;
        configure(nb_rx_rings, nb_tx_rings, rx_ring_size, tx_ring_size);

        /*
         * Start the Ethernet port.
         */
        start();

        promiscuous_set(true);
        kernel_log(SYSTEM, "%s configure ... done\n", name.c_str());
    }
    void linkup  ()
    {
        int ret = rte_eth_dev_set_link_up  (id);
        if (ret < 0) {
            throw slankdev::exception("rte_eth_dev_link_up: failed");
        }
    }
    void linkdown() { rte_eth_dev_set_link_down(id); }
    void start()
    {
        int ret = rte_eth_dev_start(id);
        if (ret < 0) {
            throw slankdev::exception("rte_eth_dev_start: failed");
        }
    }
    void stop () { rte_eth_dev_stop (id); }
    void promiscuous_set(bool on)
    {
        if (on) rte_eth_promiscuous_enable(id);
        else    rte_eth_promiscuous_disable(id);
    }
    void configure(size_t nb_rx_rings, size_t nb_tx_rings,
            size_t rx_ring_size, size_t tx_ring_size)
    {
        int retval = rte_eth_dev_configure(id, nb_rx_rings, nb_tx_rings, &conf.raw);
        if (retval != 0)
            throw slankdev::exception("rte_eth_dev_configure failed");


        /*
         * Allocate and set up RX $nb_rx_rings queue(s) per Ethernet port.
         */
        int socket_id = rte_socket_id();
        for (uint16_t qid = 0; qid < nb_rx_rings; qid++) {
            retval = rte_eth_rx_queue_setup(id, qid, rx_ring_size,
                    socket_id, NULL, mempool->get_raw());
            if (retval < 0)
                throw slankdev::exception("rte_eth_rx_queue_setup failed");

            std::string ringname = "PORT" + std::to_string(id);
            ringname += "RX" + std::to_string(qid);
            rxq.push_back(Ring(ringname.c_str(), rx_ring_size, socket_id));
        }

        /*
         * Allocate and set up $nb_tx_rings TX queue per Ethernet port.
         */
        for (uint16_t qid = 0; qid < nb_tx_rings; qid++) {
            retval = rte_eth_tx_queue_setup(id, qid, tx_ring_size,
                    socket_id, NULL);
            if (retval < 0)
                throw slankdev::exception("rte_eth_tx_queue_setup failed");

            std::string ringname = "PORT" + std::to_string(id);
            ringname += "TX" + std::to_string(qid);
            txq.push_back(Ring(ringname.c_str(), tx_ring_size, socket_id));
        }

        kernel_log(SYSTEM, "%s configure \n", name.c_str());
        kernel_log(SYSTEM, "  nb_rx_rings=%zd size=%zd\n", nb_rx_rings, rx_ring_size);
        kernel_log(SYSTEM, "  nb_tx_rings=%zd size=%zd\n", nb_tx_rings, tx_ring_size);
    }
    void rx_burst_bulk(size_t burst_size)
    {
        struct rte_mbuf* rx_pkts[burst_size];
        uint16_t nb_rx = rte_eth_rx_burst(id, 0, rx_pkts, burst_size);
        if (nb_rx == 0) return;
        rxq[0].push_bulk(rx_pkts, nb_rx);
    }
    void rx_burst()
    {
        const size_t burst_size = 32;
        struct rte_mbuf* rx_pkts[burst_size];
        uint16_t nb_rx = rte_eth_rx_burst(id, 0, rx_pkts, burst_size);
        for (uint16_t i=0; i<nb_rx; i++) {
            rxq[0].push(rx_pkts[i]);
        }
    }
    void tx_burst_bulk(size_t burst_size)
    {
        if (txq.size() >= burst_size) {
            struct rte_mbuf* tx_pkts[burst_size];
            txq[0].pop_bulk(tx_pkts, burst_size);
            uint16_t nb_tx = rte_eth_tx_burst(id, 0, tx_pkts, burst_size);
            if (nb_tx != burst_size) {
                for (size_t i=nb_tx; i<burst_size; i++)
                    rte_pktmbuf_free(tx_pkts[i]);
            }
        } else if (txq.size() > 0) {
            tx_burst();
        }
    }
    void tx_burst()
    {
        struct rte_mbuf* m = nullptr;
        txq[0].pop(&m);
        if (m) {
            rte_eth_tx_burst(id, 0, &m, 1);
        }
    }
};



} /* namespace dpdk */

