

#include <slankdev/extra/dpdk.h>

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_eth_conf port_conf;
  slankdev::init_portconf(&port_conf);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  size_t nb_ports = rte_eth_dev_count();
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, 1, 1, &port_conf, mp);
  }

  while (1) {
    constexpr size_t BURSTSZ = 32;
    rte_mbuf* mbufs[BURSTSZ];

    size_t nb_ports = rte_eth_dev_count();
    for (size_t port=0; port<nb_ports; port++) {
      constexpr size_t nb_q = 1;
      for (size_t q=0; q<nb_q; q++) {
        size_t nb_recv = rte_eth_rx_burst(port, q, mbufs, BURSTSZ);
        if (nb_recv != 0) {
          rte_eth_tx_burst(port^1, q, mbufs, nb_recv);
        }
      }
    }
  }
}
