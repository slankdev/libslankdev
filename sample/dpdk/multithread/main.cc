

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <vector>

#include <slankdev/dpdk.h>
#include <slankdev/util.h>
#include <slankdev/exception.h>

#include "ring.h"
#include "system.h"
#include "wrap.h"


ring<rte_mbuf> ring0;

int thread_worker(void* arg)
{
    UNUSED(arg);
    while (1) {
        sleep(1);
        printf("fa\n");
    }
    return 0;
}

int thread_txrx(void* arg)
{
    UNUSED(arg);
	const size_t burst_size = 32;
    const uint8_t nb_ports = rte_eth_dev_count();
	for (;;) {
        for (uint8_t port = 0; port < nb_ports; port++) {
            struct rte_mbuf *bufs[burst_size];
            const uint16_t nb_rx = rx_burst(port, bufs, burst_size);
            if (nb_rx == 0) continue;

            static uint8_t C = 0x01;
            uint8_t* p = rte_pktmbuf_mtod(bufs[0], uint8_t*);
            for (int i=0; i<6; i++) p[i] = C;
            C++;

            ring0.push(bufs[0]);
            printf("enque %p \n", bufs[0]);

            if (ring0.size() > 4) {
                struct rte_mbuf *bb[100];
                ring0.pop_bulk(bb, 5);
                tx_burst(port, bb, 5);
                printf("deque_bulk\n");
            }
	    }
	}
    return 0;
}




int main(int argc, char** argv)
{
	System sys;
	sys.init(argc, argv);
	ring0.init("SLANKDEV", 16, 0);

	sys.show();
	sys.cpus[1].set_func(thread_worker);
	sys.cpus[2].set_func(thread_txrx);
	sys.launch();
}


