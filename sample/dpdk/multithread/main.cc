

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <vector>

#include <slankdev/dpdk.h>
#include <slankdev/util.h>
#include <slankdev/exception.h>

#include "system.h"


System sys;


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
				Port& in_port = sys.ports[i];
				Port& out_port = sys.ports[i^1];

				rte_mbuf* m = nullptr;
				in_port.rxq[qid].pop(&m);
				if (m)
					out_port.txq[qid].push(m);
			}
	    }
	}
    return 0;
}
#if 1
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
#else
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
#endif
int thread_viewer(void* arg)
{
    UNUSED(arg);
	while (1) {
		sys.show_state();
		usleep(50000);
	}
	return 0;
}

int main(int argc, char** argv)
{
	sys.boot(argc, argv);
	sys.show_state();
	sys.ports[0].nb_rx_rings = 2;
	sys.ports[0].nb_tx_rings = 1;
	sys.ports[1].nb_rx_rings = 2;
	sys.ports[1].nb_tx_rings = 1;
	sys.show_config();


	sys.configure();
	sys.cpus[1].func = thread_tx;
	sys.cpus[2].func = thread_worker;
	sys.cpus[3].func = thread_rx;
	sys.cpus[4].func = thread_viewer;
	sys.show_state();

	// sys.launch();
}


