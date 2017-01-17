
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
#include "log.h"

#define MESGTYPE 1


namespace dpdk {

void print_message();


class System {
public:
	std::vector<Cpu>  cpus;
	std::vector<Port> ports;
	dpdk::pool    mp;

    /*
     * Boot DPDK System.
     */
	System(int argc, char** argv)
	{
        kernel_log(SYSTEM, "[+] Booting ...\n");
        print_message();

		int ret = rte_eal_init(argc, argv);
		if (ret < 0) {
			throw slankdev::exception("rte_eal_init");
		}

		uint8_t nb_cpus  = rte_lcore_count();
		uint16_t nb_ports = rte_eth_dev_count();
		cpus.resize(nb_cpus);
		ports.resize(nb_ports);

		for (uint8_t i=0; i<nb_cpus; i++) {
			cpus[i].boot(i);
		}
		for (uint16_t i=0; i<nb_ports; i++) {
			ports[i].boot(i, &mp);
		}

        kernel_log(SYSTEM, "DPDK boot Done! \n");
	}
    ~System()
    {
		rte_eal_mp_wait_lcore();
    }

    void halt()
    {
        kernel_log(SYSTEM, "[+] System Halt ...\n");
        rte_exit(0, "Bye...\n");
    }

	void configure(size_t rx_ring_size, size_t tx_ring_size)
	{
        kernel_log(SYSTEM, "configure \n");

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
			ports[i].configure(rx_ring_size, tx_ring_size);
		}

        kernel_log(SYSTEM, "configure ... done\n");
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
        kernel_log(SYSTEM, "launch thread to each-cores \n");
		rte_eal_mp_wait_lcore();
	}
};


void print_message()
{
    switch (MESGTYPE) {
        case 1:
            kernel_log(SYSTEM, "\n");
            kernel_log(SYSTEM, " oooooooooo.   8@slankdev.   oYoooooooo.   oooo    oooo       .oooooo..o                          \n");
            kernel_log(SYSTEM, " `888'   `Y8b  `888   `Y88. `8U8'   `Y8b  `888   .8P'       d8P'    `Y8         @slankdev         \n");
            kernel_log(SYSTEM, "   888     888  888   .d88'   KB8     888  888  d8'         Y88bo.      oooo    ooo  .oooo.o      \n");
            kernel_log(SYSTEM, "   888     888  888ooo88P'    AO8     888  88888[            `\"Y8888o.   `88.  .8'  d88(  \"8    \n");
            kernel_log(SYSTEM, "   888     888  888           RN8     888  888`88b.              `\"Y88b   `88..8'   `\"Y88b.     \n");
            kernel_log(SYSTEM, "   888    d88'  888           IK8     d88'  888  `88b.       oo     .d8P    `888'    o.  )88b     \n");
            kernel_log(SYSTEM, " o@slankdev'   o888o         o888bood8P'   o888o  o888o      8""88888P'      .8'     8""888P'     \n");
            kernel_log(SYSTEM, "                                                                        .o..P'                    \n");
            kernel_log(SYSTEM, "        @slankdev:please follow me on GitHub                            `Y8P'                     \n");
            kernel_log(SYSTEM, "\n");
            break;
        case 2:
            kernel_log(SYSTEM, "8888888b.  8888888b.  8888888b.  888    d8P       \n");
            kernel_log(SYSTEM, "888  \"Y88b 888   Y88b 888  \"Y88b 888   d8P      \n");
            kernel_log(SYSTEM, "888    888 888    888 888    888 888  d8P         \n");
            kernel_log(SYSTEM, "888    888 888   d88P 888    888 888d88K          \n");
            kernel_log(SYSTEM, "888    888 8888888P\"  888    888 8888888b        \n");
            kernel_log(SYSTEM, "888    888 888        888    888 888  Y88b        \n");
            kernel_log(SYSTEM, "888  .d88P 888        888  .d88P 888   Y88b       \n");
            kernel_log(SYSTEM, "8888888P\"  888        8888888P\"  888    Y88b    \n");
            kernel_log(SYSTEM, "\n");
            break;
        case 3:
            kernel_log(SYSTEM, "+------------------------------------------------------------------------------+\n");
            kernel_log(SYSTEM, "|  ######  ######  ######  #    #     #####                                    |\n");
            kernel_log(SYSTEM, "|  #     # #     # #     # #   #     #     # #   #  ####  ##### ###### #    #  |\n");
            kernel_log(SYSTEM, "|  #     # #     # #     # #  #      #        # #  #        #   #      ##  ##  |\n");
            kernel_log(SYSTEM, "|  #     # ######  #     # ###        #####    #    ####    #   #####  # ## #  |\n");
            kernel_log(SYSTEM, "|  #     # #       #     # #  #            #   #        #   #   #      #    #  |\n");
            kernel_log(SYSTEM, "|  #     # #       #     # #   #     #     #   #   #    #   #   #      #    #  |\n");
            kernel_log(SYSTEM, "|  ######  #       ######  #    #     #####    #    ####    #   ###### #    #  |\n");
            kernel_log(SYSTEM, "+------------------------------------------------------------------------------+\n");
            break;
        case 4:
            kernel_log(SYSTEM, "+-------------------------------------------------------------------+\n");
            kernel_log(SYSTEM, "|  _____  _____  _____  _  __   _____           _                   |\n");
            kernel_log(SYSTEM, "| |  __ \\|  __ \\|  __ \\| |/ /  / ____|         | |     @slankdev    |\n");
            kernel_log(SYSTEM, "| | |  | | |__) | |  | | ' /  | (___  _   _ ___| |_ ___ _ __ ___    |\n");
            kernel_log(SYSTEM, "| | |  | |  ___/| |  | |  <    \\___ \\| | | / __| __/ _ \\ '_ ` _ \\   |\n");
            kernel_log(SYSTEM, "| | |__| | |    | |__| | . \\   ____) | |_| \\__ \\ ||  __/ | | | | |  |\n");
            kernel_log(SYSTEM, "| |_____/|_|    |_____/|_|\\_\\ |_____/ \\__, |___/\\__\\___|_| |_| |_|  |\n");
            kernel_log(SYSTEM, "|                                      __/ |                        |\n");
            kernel_log(SYSTEM, "|            @slankdev                |___/                         |\n");
            kernel_log(SYSTEM, "+-------------------------------------------------------------------+\n");
            break;
        case 5:
            kernel_log(SYSTEM, "+----------------------------------------------------------------------------------------------+   \n");
            kernel_log(SYSTEM, "| oooooooooo.   8@slankdev.   oYoooooooo.   oooo    oooo       .oooooo..o                      |    \n");
            kernel_log(SYSTEM, "| `888'   `Y8b  `888   `Y88. `8U8'   `Y8b  `888   .8P'       d8P'    `Y8         @slankdev     |    \n");
            kernel_log(SYSTEM, "|   888     888  888   .d88'   KB8     888  888  d8'         Y88bo.      oooo    ooo  .oooo.o  |    \n");
            kernel_log(SYSTEM, "|   888     888  888ooo88P'    AO8     888  88888[            `\"Y8888o.   `88.  .8'  d88(  \"8  |  \n");
            kernel_log(SYSTEM, "|   888     888  888           RN8     888  888`88b.              `\"Y88b   `88..8'   `\"Y88b.   |  \n");
            kernel_log(SYSTEM, "|   888    d88'  888           IK8     d88'  888  `88b.       oo     .d8P    `888'    o.  )88b |    \n");
            kernel_log(SYSTEM, "| o@slankdev'   o888o         o888bood8P'   o888o  o888o      8""88888P'      .8'     8""888P'     |\n");
            kernel_log(SYSTEM, "|                                                                        .o..P'                |    \n");
            kernel_log(SYSTEM, "|        @slankdev:please follow me on GitHub                            `Y8P'                 |    \n");
            kernel_log(SYSTEM, "+----------------------------------------------------------------------------------------------+   \n");
            break;
        default: throw slankdev::exception("not found");
    }
}



} /* namespace dpdk */


