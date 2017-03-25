/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2015 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <stdio.h>
#include <stdlib.h>

#include <rte_common.h>
#include <rte_spinlock.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_version.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "rte_ethtool.h"
#include <slankdev/dpdk.h>


#define MAX_PORTS RTE_MAX_ETHPORTS
#define MAX_BURST_LENGTH 32
#define PORT_RX_QUEUE_SIZE 128
#define PORT_TX_QUEUE_SIZE 256
#define PKTPOOL_EXTRA_SIZE 512
#define PKTPOOL_CACHE 32


struct app_port {
    struct ether_addr mac_addr;
    int idx_port;
    slankdev::pool pkt_pool;
};

struct app_config {
    struct app_port ports[MAX_PORTS];
    int cnt_ports;
    int exit_now;
} app_cfg;


static int slave_main(__attribute__((unused)) void *nouse)
{

    while (app_cfg.exit_now == 0) {
        for (uint16_t idx_port = 0; idx_port < app_cfg.cnt_ports; idx_port++) {

            /*
             * Incoming frames
             */
            struct app_port *ptr_port = &app_cfg.ports[idx_port];
            rte_mbuf* mbufs[MAX_BURST_LENGTH];
            uint16_t cnt_recv_frames = rte_eth_rx_burst(
                ptr_port->idx_port, 0, mbufs, MAX_BURST_LENGTH);
            if (cnt_recv_frames > 0) {
                for (uint16_t i = 0; i<cnt_recv_frames; i++)
                    rte_pktmbuf_free(mbufs[i]);
            }
        }
    }

    return 0;
}


void ethapp_main()
{

    printf("\n\n");
    printf("ETHTOOL: number of devices is %u \n", rte_eth_dev_count());

    for (uint16_t i=0; i<rte_eth_dev_count(); i++) {
        printf("\n PORT%u\n", i);
        int ret = rte_ethtool_get_link(0);
        printf(" + link: %s \n", ret==1?"Yes":"No");

        printf(" + \n");

    }

    printf("\nbye...\n");
    printf("\n\n");

    app_cfg.exit_now = 1;
}


int main(int argc, char **argv)
{
    /*
     * Boot dpdk
     */
    slankdev::dpdk_boot(argc, argv);
    uint32_t nb_ports = rte_eth_dev_count();
    printf("Number of NICs: %i\n", nb_ports);
    if (nb_ports == 0)
        rte_exit(EXIT_FAILURE, "No available NIC ports!\n");
    if (nb_ports > MAX_PORTS) {
        printf("Info: Using only %i of %i ports\n", nb_ports, MAX_PORTS);
        nb_ports = MAX_PORTS;
    }


    /*
     * Init ports
     */
    for (uint16_t idx_port = 0; idx_port < nb_ports; idx_port++) {
        app_port *ptr_port = &app_cfg.ports[idx_port];

        struct rte_eth_dev_info dev_info;
        rte_eth_dev_info_get(idx_port, &dev_info);
        int size_pktpool = dev_info.rx_desc_lim.nb_max +
            dev_info.tx_desc_lim.nb_max + PKTPOOL_EXTRA_SIZE;

        char str_name[16];
        snprintf(str_name, 16, "pkt_pool%i", idx_port);
        ptr_port->pkt_pool.create( str_name, size_pktpool,
            PKTPOOL_CACHE, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

        printf("Init port %i..\n", idx_port);
        ptr_port->idx_port = idx_port;

        slankdev::port_conf cfg_port;
        cfg_port.txmode.mq_mode = ETH_MQ_TX_NONE;

        slankdev::port_init(idx_port, &ptr_port->pkt_pool, &cfg_port,
            1, 1, PORT_RX_QUEUE_SIZE, PORT_TX_QUEUE_SIZE);
    }


    app_cfg.exit_now = 0;
    app_cfg.cnt_ports = nb_ports;

    if (rte_lcore_count() < 2)
        rte_exit(EXIT_FAILURE, "No available slave core!\n");

    uint32_t id_core = rte_lcore_id();
    id_core = rte_get_next_lcore(id_core, 1, 1);
    rte_eal_remote_launch(slave_main, NULL, id_core);

    ethapp_main();

    app_cfg.exit_now = 1;
    RTE_LCORE_FOREACH_SLAVE(id_core) {
        if (rte_eal_wait_lcore(id_core) < 0)
            return -1;
    }
}
