

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <slankdev/dpdk.h>
#include <slankdev/exception.h>

#define RX_RING_SIZE 128
#define TX_RING_SIZE 512
#define NUM_MBUFS 8192
#define MBUF_CACHE_SIZE 0
#define BURST_SIZE 32
#define MBUF_SIZ RTE_MBUF_DEFAULT_BUF_SIZE


void lcore_main()
{
    const uint8_t nb_ports = rte_eth_dev_count();
    uint8_t port;

	/*
	 * Check that the port is on the same NUMA node as the polling thread
	 * for best performance.
	 */
	for (port = 0; port < nb_ports; port++)
		if (rte_eth_dev_socket_id(port) > 0 &&
				rte_eth_dev_socket_id(port) !=
						(int)rte_socket_id())
			printf("WARNING, port %u is on remote NUMA node to "
					"polling thread.\n\tPerformance will "
					"not be optimal.\n", port);

	printf("\nCore %u forwarding packets. [Ctrl+C to quit]\n", rte_lcore_id());

	/* Run until the application is quit or killed. */
	for (;;) {
		/*
		 * Receive packets on a port and forward them on the paired
		 * port. The mapping is 0 -> 1, 1 -> 0, 2 -> 3, 3 -> 2, etc.
		 */
		for (port = 0; port < nb_ports; port++) {

			/* Get burst of RX packets, from first port of pair. */
			struct rte_mbuf *bufs[BURST_SIZE];
			const uint16_t nb_rx = rte_eth_rx_burst(port, 0,
					bufs, BURST_SIZE);

			if (unlikely(nb_rx == 0))
				continue;

			/* Send burst of TX packets, to second port of pair. */
			const uint16_t nb_tx = rte_eth_tx_burst(port ^ 1, 0,
					bufs, nb_rx);

			/* Free any unsent packets. */
			if (unlikely(nb_tx < nb_rx)) {
				uint16_t buf;
				for (buf = nb_tx; buf < nb_rx; buf++)
					rte_pktmbuf_free(bufs[buf]);
			}
		}
	}
}


int main(int argc, char** argv)
{
    slankdev::dpdk_boot(argc, argv);

    slankdev::pool mp;
    mp.create("Pool0", NUM_MBUFS*rte_eth_dev_count(), MBUF_CACHE_SIZE, MBUF_SIZ, rte_socket_id());

    slankdev::port_conf conf;
    uint16_t nb_ports = rte_eth_dev_count();
	for (uint16_t portid = 0; portid < nb_ports; portid++) {
        port_init(portid, &mp, &conf, 1, 1, RX_RING_SIZE, TX_RING_SIZE);

        slankdev::ether_addr addr;
        rte_eth_macaddr_get(portid, &addr);

        printf("Port %u MAC: ", (unsigned)portid);
        addr.print(stdout);
        printf("\n");
    }

	if (rte_lcore_count() > 1)
		printf("\nWARNING: Too many lcores enabled. Only 1 used.\n");

	lcore_main(); /* Call lcore_main on the master core only. */
	return 0;
}

