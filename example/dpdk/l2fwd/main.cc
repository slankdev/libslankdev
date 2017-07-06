
#include <stdio.h>
#include <slankdev/extra/dpdk.h>

int l2fwd(void*)
{
  while (true) {
    constexpr size_t BURSTSZ = 32;
    rte_mbuf* mbufs[BURSTSZ];

    constexpr size_t nb_q = 1;
    size_t nb_recv = rte_eth_rx_burst(0, 0, mbufs, BURSTSZ);
    if (nb_recv == 0) continue;
    size_t nb_send = rte_eth_tx_burst(1, 0, mbufs, nb_recv);
    if (nb_send < nb_recv)
      slankdev::rte_pktmbuf_free_bulk(&mbufs[nb_send], nb_recv-nb_send);
  }
}

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_eth_conf port_conf;
  slankdev::init_portconf(&port_conf);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  size_t nb_ports = rte_eth_dev_count();
  if (nb_ports == 0) throw slankdev::exception("no ethdev");
  printf("%zd ports found \n", nb_ports);
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, 2, 1, &port_conf, mp);
  }

  rte_eal_remote_launch(l2fwd, nullptr, 1);
  rte_eal_mp_wait_lcore();
}

