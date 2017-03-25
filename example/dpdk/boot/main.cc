

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
    printf("\n\n");
    printf("Number of lcores: %u\n", rte_lcore_count());
    printf("Number of Ports : %u\n", rte_eth_dev_count());
    printf("\n\n");
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

    lcore_main();

	return 0;
}

