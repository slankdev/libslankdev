
#pragma once
#include <stdio.h>
#include "system.h"
#include "types.h"



extern dpdk::System sys;


int thread_worker(void* arg)
{
	UNUSED(arg);
	for (;;) {
		const uint8_t nb_ports = sys.ports.size();
		for (uint8_t i=0; i<nb_ports; i++) {
			const uint8_t nb_rxque = sys.ports[i].rxq.size();
			const uint8_t nb_txque = sys.ports[i].txq.size();
			assert(nb_rxque != nb_txque);

			for (uint8_t qid=0; qid<nb_rxque; qid++) {
				dpdk::Port& in_port = sys.ports[i];
				dpdk::Port& out_port = sys.ports[i^1];

				rte_mbuf* m = nullptr;
				in_port.rxq[qid].pop(&m);
				if (m)
					out_port.txq[qid].push(m);
			}
	    }
	}
    return 0;
}
int thread_tx(void* arg)
{
    UNUSED(arg);
    const uint8_t nb_ports = sys.ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys.ports[pid].tx_burst();
	    }
	}
}
int thread_rx(void* arg)
{
    UNUSED(arg);
    const uint8_t nb_ports = sys.ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys.ports[pid].rx_burst();
	    }
	}
    return 0;
}
int thread_txrx(void* arg)
{
    UNUSED(arg);
    const uint8_t nb_ports = sys.ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys.ports[pid].rx_burst();
            sys.ports[pid].tx_burst();
	    }
	}
    return 0;
}
int thread_viewer(void* arg)
{
    UNUSED(arg);
	while (1) {
        sys.ports[0].stats.show();
		usleep(50000);
	}
	return 0;
}


