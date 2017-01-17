

#pragma once
#include "types.h"
#include "ring.h"
#include "log.h"
#include "dpdk_struct_utils.h"





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
        void reset()  { rte_eth_stats_reset(port->port_id);     }
        void update() { rte_eth_stats_get(port->port_id, &raw); }
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
            rte_eth_dev_info_get(port->port_id, &raw);
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
        void update() { rte_eth_macaddr_get(port->port_id, this); }
        void set(::ether_addr* addr)
        {
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
        void add(::ether_addr* addr)
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
        void del(::ether_addr* addr)
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

    Ring rxq;
    Ring txq;

    pool*      mempool;
    port_conf  conf;
    port_stats stats;
    dev_info   info;

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
    void configure(size_t rx_ring_size, size_t tx_ring_size)
    {
        kernel_log(SYSTEM, "configure %s ...\n", name.c_str());

        if (port_id >= rte_eth_dev_count())
            throw slankdev::exception("port is not exist");

        /*
         * Configure the Ethernet device.
         */
        const uint16_t nb_rx_rings = 1;
        const uint16_t nb_tx_rings = 1;
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
        char ringname[32];
        sprintf(ringname, "port%u-rx", port_id);
        rxq.init(ringname, rx_ring_size, 0);
        sprintf(ringname, "port%u-tx", port_id);
        txq.init(ringname, tx_ring_size, 0);

        kernel_log(SYSTEM, "configure %s ... done\n", name.c_str());

    }
    void rx_burst()
    {
        const size_t burst_size = 32;
        struct rte_mbuf* rx_pkts[burst_size];
        uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, rx_pkts, burst_size);
        for (uint16_t i=0; i<nb_rx; i++) {
            rxq.push(rx_pkts[i]);
        }
    }
    void tx_burst()
    {
        while (txq.size() > 0) {
            struct rte_mbuf* m = nullptr;
            txq.pop(&m);
            if (m == nullptr) {
                break;
            }
            rte_eth_tx_burst(port_id, 0, &m, 1);
        }
    }
};



} /* namespace dpdk */


