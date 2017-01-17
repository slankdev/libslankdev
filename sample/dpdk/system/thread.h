
#pragma once
#include <stdio.h>
#include "system.h"
#include "types.h"



static void ifconfig(dpdk::System* sys)
{
    for (dpdk::Port& port : sys->ports) {
        port.stats.update();
        size_t nb_rxq = port.rxq.size();
        size_t nb_txq = port.txq.size();

        printf("%s\n", port.name.c_str());
        printf("  HWaddr %s \n", port.addr.toString().c_str());

        auto& stats = port.stats;
        printf("  RX packets:%lu errors:%lu dropped:%lu allocmiss:%lu \n",
                    stats.raw.ipackets, stats.raw.ierrors,
                    stats.raw.imissed, stats.raw.rx_nombuf);
        printf("  TX packets:%lu errors:%lu  \n",
                stats.raw.opackets, stats.raw.oerrors);
        printf("  RX bytes:%lu TX bytes:%lu \n", stats.raw.ibytes, stats.raw.obytes);

        for (size_t qid=0; qid<nb_rxq; qid++) {
            printf("  RING:%s RX packets:%lu bytes:%lu \n", port.rxq[qid].name(),
                    stats.raw.q_ipackets[qid], stats.raw.q_ibytes[qid]);
        }
        for (size_t qid=0; qid<nb_txq; qid++) {
            printf("  RING:%s TX packets:%lu bytes:%lu errors:%lu \n",
                    port.txq[qid].name(), stats.raw.q_opackets[qid],
                    stats.raw.q_obytes[qid], stats.raw.q_errors[qid]);
        }
    }
}


int thread_worker(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
	for (;;) {
		const uint8_t nb_ports = sys->ports.size();
		for (uint8_t pid=0; pid<nb_ports; pid++) {
			const uint8_t nb_rxque = sys->ports[pid].rxq.size();
			const uint8_t nb_txque = sys->ports[pid].txq.size();

			for (uint8_t rx_qid=0; rx_qid<nb_rxque; rx_qid++) {
				dpdk::Port& in_port  = sys->ports[pid];
				dpdk::Port& out_port = sys->ports[pid^1];

				rte_mbuf* m = nullptr;
				in_port.rxq[rx_qid].pop(&m);
				if (m) {
                    out_port.txq[1].push(m);
                    for (uint8_t tx_qid=0; tx_qid<nb_txque; tx_qid++) {
                        out_port.txq[tx_qid].push(m);
                    }
                }
			}
	    }
	}
    return 0;
}
int thread_txrx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst();
            sys->ports[pid].tx_burst();
	    }
	}
    return 0;
}
int thread_viewer(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
	while (1) {
        slankdev::clear_screen();
        ifconfig(sys);
		usleep(50000);
	}
	return 0;
}






#if 0
int thread_tx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].tx_burst();
	    }
	}
}
int thread_rx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst();
	    }
	}
    return 0;
}
#endif
