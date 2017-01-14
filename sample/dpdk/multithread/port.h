

#pragma once
#include "types.h"
#include "ring.h"
#include "util.h"


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

        // struct {
        //  struct rte_eth_vmdq_dcb_conf vmdq_dcb_conf;
        //  struct rte_eth_dcb_rx_conf dcb_rx_conf;
        //  struct rte_eth_vmdq_rx_conf vmdq_rx_conf;
        // } rx_adv_conf; /**< Port RX filtering configuration (union). */

        // union {
        //  struct rte_eth_vmdq_dcb_tx_conf vmdq_dcb_tx_conf;
        //  struct rte_eth_dcb_tx_conf dcb_tx_conf;
        //  struct rte_eth_vmdq_tx_conf vmdq_tx_conf;
        // } tx_adv_conf; /**< Port TX DCB configuration (union). */

        // struct rte_fdir_conf fdir_conf; /**< FDIR configuration. */
        // struct rte_intr_conf intr_conf; /**< Interrupt mode configuration. */

    public:
        rte_eth_conf raw;
        port_conf(Port* p) : port(p)
        {
            memset(&raw, 0x00, sizeof(raw));
            raw.rxmode.max_rx_pkt_len = ETHER_MAX_LEN;
        }
        void show()
        {
            printf("# port config %p \n", &raw);
            printf("link_speeds                : %u(%s) \n", raw.link_speeds, linkspeed2str(raw.link_speeds));

            printf("rxmode                     : struct\n");
            printf("    mq_mode                : %s\n", rxmqmode2str(raw.rxmode.mq_mode));
            printf("    max_rx_pkt_len         : %u\n", raw.rxmode.max_rx_pkt_len);
            printf("    split_hdr_size         : %u\n", raw.rxmode.split_hdr_size);
            printf("    header_split           : %s\n", (raw.rxmode.header_split  ==1)?"Yes":"No");
            printf("    hw_ip_checksum         : %s\n", (raw.rxmode.hw_ip_checksum==1)?"Yes":"No");
            printf("    hw_vlan_filter         : %s\n", (raw.rxmode.hw_vlan_filter==1)?"Yes":"No");
            printf("    hw_vlan_strip          : %s\n", (raw.rxmode.hw_vlan_strip ==1)?"Yes":"No");
            printf("    hw_vlan_extend         : %s\n", (raw.rxmode.hw_vlan_extend==1)?"Yes":"No");
            printf("    jumbo_frame            : %s\n", (raw.rxmode.jumbo_frame   ==1)?"Yes":"No");
            printf("    hw_strip_crc           : %s\n", (raw.rxmode.hw_strip_crc  ==1)?"Yes":"No");
            printf("    enable_scatter         : %s\n", (raw.rxmode.enable_scatter==1)?"Yes":"No");
            printf("    enable_lro             : %s\n", (raw.rxmode.enable_lro    ==1)?"Yes":"No");

            printf("txmode                     : struct\n");
            printf("    mq_mode                : %s\n", txmqmode2str(raw.txmode.mq_mode)   );
            printf("    hw_vlan_reject_tagged  : %s\n", (raw.txmode.hw_vlan_reject_tagged  ==1)?"Yes":"No");
            printf("    hw_vlan_reject_untagged: %s\n", (raw.txmode.hw_vlan_reject_untagged==1)?"Yes":"No");
            printf("    hw_vlan_insert_pvid    : %s\n", (raw.txmode.hw_vlan_insert_pvid    ==1)?"Yes":"No");

            printf("lpbk_mode                  : %u \n", raw.lpbk_mode          );

            printf("rx_adv_conf                : struct\n");
            printf("    rss_conf               : struct\n");
            printf("        rss_key            : %p \n", raw.rx_adv_conf.rss_conf.rss_key       );
            if (raw.rx_adv_conf.rss_conf.rss_key) {
                printf("          val              : ");
                for (auto i=0; i<40; i++) {
                    printf("%02x", raw.rx_adv_conf.rss_conf.rss_key[i]);
                } printf("\n");
            }
            printf("        rss_key_len        : %u\n", raw.rx_adv_conf.rss_conf.rss_key_len   );
            printf("        rss_hf             : %lu\n", raw.rx_adv_conf.rss_conf.rss_hf       );

            printf("  vmdq_dcb_conf            : struct [TBD] \n");
            printf("  dcb_rx_conf              : struct [TBD] \n");
            printf("  vmdq_rx_conf             : struct [TBD] \n");

            printf("tx_adv_conf                : union        \n");
            printf("    vmdq_dcb_tx_conf       : struct [TBD] \n");
            printf("    dcb_tx_conf            : struct [TBD] \n");
            printf("    vmdq_tx_conf           : struct [TBD] \n");

            printf("dcb_capability_en          : %u \n", raw.dcb_capability_en  );

            printf("fdir_conf                  : struct [TBD] \n");
            printf("intr_conf                  : struct [TBD] \n");
        }
    };
    class port_stats {
        const Port* port;
        rte_eth_stats raw;
    public:
        port_stats(Port* p) : port(p) {}
        void reset()  { rte_eth_stats_reset(port->port_id);     }
        void update() { rte_eth_stats_get(port->port_id, &raw); }
        void show()
        {
            update();
            size_t nb_rxq = port->rxq.size();
            size_t nb_txq = port->txq.size();

            printf("Port%u Stats %p\n", port->port_id, &raw);
            printf("ipackets  : %lu \n", raw.ipackets );
            printf("opackets  : %lu \n", raw.opackets );
            printf("ibytes    : %lu \n", raw.ibytes   );
            printf("obytes    : %lu \n", raw.obytes   );
            printf("imissed   : %lu \n", raw.imissed  );
            printf("ierrors   : %lu \n", raw.ierrors  );
            printf("oerrors   : %lu \n", raw.oerrors  );
            printf("rx_nombuf : %lu \n", raw.rx_nombuf);

            for (size_t qid=0; qid<nb_rxq; qid++) {
                printf("%s \n", port->rxq[qid].name());
                printf("q_ipackets : %lu \n", raw.q_ipackets[qid]);
                printf("q_ibytes   : %lu \n", raw.q_ibytes  [qid]);
            }
            for (size_t qid=0; qid<nb_txq; qid++) {
                printf("%s \n", port->txq[qid].name());
                printf("q_opackets : %lu \n", raw.q_opackets[qid]);
                printf("q_obytes   : %lu \n", raw.q_obytes  [qid]);
                printf("q_errors   : %lu \n", raw.q_errors  [qid]);
            }
        }
    };


public:
    uint8_t port_id;
    ether_addr addr;

    std::vector<Ring> rxq;
    std::vector<Ring> txq;

    pool* mp_;
    port_conf conf;
    port_stats stats;

    Port() : conf(this), stats(this) {}

    void boot(uint8_t id, dpdk::pool* mp)
    {
        mp_ = mp;
        port_id = id;
        rte_eth_macaddr_get(id, &addr);
    }
    void configure(size_t nb_rx_rings, size_t nb_tx_rings,
            size_t rx_ring_size, size_t tx_ring_size)
    {
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
                    rte_eth_dev_socket_id(port_id), NULL, mp_->get_raw());
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
            sprintf(ringname, "port%u rxring%zd", port_id, i);
            rxq[i].init(ringname, rx_ring_size, 0);
        }
        for (size_t i=0; i<nb_tx_rings; i++) {
            char ringname[32];
            sprintf(ringname, "port%u txring%zd", port_id, i);
            txq[i].init(ringname, tx_ring_size, 0);
        }
    }
    void show_queues_state()
    {
        printf("   - port%u : %s\n", port_id, addr.toString().c_str());

        for (const Ring& queue : rxq) queue.stats();
        for (const Ring& queue : txq) queue.stats();
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


