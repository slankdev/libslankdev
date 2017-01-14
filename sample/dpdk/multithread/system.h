
#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <vector>

#include <slankdev/util.h>
#include <slankdev/exception.h>

#include "types.h"
#include "cpu.h"
#include "port.h"


namespace dpdk {









class System {
public:
	std::vector<Cpu>  cpus;
	std::vector<Port> ports;
	dpdk::pool    mp;

    ~System()
    {
		rte_eal_mp_wait_lcore();
    }

    void print_banner() const
    {
		// printf("+------------------------------------------------------------------------------+\n");
		// printf("|  ######  ######  ######  #    #     #####                                    |\n");
		// printf("|  #     # #     # #     # #   #     #     # #   #  ####  ##### ###### #    #  |\n");
		// printf("|  #     # #     # #     # #  #      #        # #  #        #   #      ##  ##  |\n");
		// printf("|  #     # ######  #     # ###        #####    #    ####    #   #####  # ## #  |\n");
		// printf("|  #     # #       #     # #  #            #   #        #   #   #      #    #  |\n");
		// printf("|  #     # #       #     # #   #     #     #   #   #    #   #   #      #    #  |\n");
		// printf("|  ######  #       ######  #    #     #####    #    ####    #   ###### #    #  |\n");
		// printf("+------------------------------------------------------------------------------+\n");

		printf("+-------------------------------------------------------------------+\n");
		printf("|  _____  _____  _____  _  __   _____           _                   |\n");
		printf("| |  __ \\|  __ \\|  __ \\| |/ /  / ____|         | |                  |\n");
		printf("| | |  | | |__) | |  | | ' /  | (___  _   _ ___| |_ ___ _ __ ___    |\n");
		printf("| | |  | |  ___/| |  | |  <    \\___ \\| | | / __| __/ _ \\ '_ ` _ \\   |\n");
		printf("| | |__| | |    | |__| | . \\   ____) | |_| \\__ \\ ||  __/ | | | | |  |\n");
		printf("| |_____/|_|    |_____/|_|\\_\\ |_____/ \\__, |___/\\__\\___|_| |_| |_|  |\n");
		printf("|                                      __/ |                        |\n");
        printf("|                                     |___/                         |\n");
		printf("+-------------------------------------------------------------------+\n");
    }
	void boot(int argc, char** argv)
	{
        printf("Booting...\n");
        printf("[+] boot dpdk...\n");
        print_banner();

		int ret = rte_eal_init(argc, argv);
		if (ret < 0) {
			throw slankdev::exception("rte_eal_init");
		}

        printf("[+] boot dpdk... done\n");

		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();
		cpus.resize(nb_cpus);
		ports.resize(nb_ports);

		for (uint8_t i=0; i<nb_cpus; i++) {
            printf("[+] Init lcore%u ...", i);
			cpus[i].boot(i);
            printf("done\n");
		}
		for (uint16_t i=0; i<nb_ports; i++) {
            printf("[+] Init port%u ", i);
			ports[i].boot(i, &mp);
            printf("address=%s ", ports[i].addr.toString().c_str());
            printf(" ... done\n");
		}

        printf("[+] DPDK boot Done! \n");
	}
	void configure(size_t nb_rx_rings, size_t nb_tx_rings,
            size_t rx_ring_size, size_t tx_ring_size)
	{
		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();

		size_t mbuf_cache_size = 0;
		size_t mbuf_siz = RTE_MBUF_DEFAULT_BUF_SIZE;
		size_t num_mbufs = 8192;
		mp.create(
				"Pool0",
				num_mbufs * rte_eth_dev_count(),
				mbuf_cache_size, mbuf_siz,
				rte_socket_id()
		);

		for (uint8_t i=0; i<nb_cpus; i++) {
			cpus[i].configure();
		}
		for (uint16_t i=0; i<nb_ports; i++) {
			ports[i].configure(nb_rx_rings, nb_tx_rings, rx_ring_size, tx_ring_size);
		}

	}
	void launch()
	{
		/*
		 * The lcore0 is com cpu core.
		 * So it must not launch that.
		 */
		for (size_t i=1; i<cpus.size(); i++) {
			if (cpus[i].func)
				cpus[i].launch();
		}
		rte_eal_mp_wait_lcore();
	}
};



} /* namespace dpdk */


