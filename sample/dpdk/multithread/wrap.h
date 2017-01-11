
#pragma once
#include <slankdev/dpdk.h>


size_t rx_burst(uint8_t port, rte_mbuf** bufs, size_t nb_bufs)
{
    const uint16_t nb_rx = rte_eth_rx_burst(port, 0, bufs, nb_bufs);
    return nb_rx;
}

void tx_burst(uint8_t port, rte_mbuf** bufs, size_t nb_bufs)
{
    const uint16_t nb_tx = rte_eth_tx_burst(port, 0, bufs, nb_bufs);
    if (nb_tx != nb_bufs)
        throw slankdev::exception("rte_eth_tx_burst");
}
