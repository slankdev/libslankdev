
#include <slankdev/extra/dpdk.h>

int argc = 2;
const char* argv[] = { "./aout", "--vdev=net_tap0,iface=tap0" };

int main()
{
  slankdev::dpdk_boot(argc, (char**)argv);
  size_t nb_ports = rte_eth_dev_count();
  if (nb_ports != 1) {
    throw slankdev::exception("nb of port is invalid");
  }
  printf("port nb: %zd \n", nb_ports);

  rte_mempool* mp = slankdev::mp_alloc("BB");
  rte_eth_conf conf;
  slankdev::init_portconf(&conf);
  conf.rxmode.mq_mode = ETH_MQ_RX_RSS;

  constexpr size_t nb_rxq = 4;
  constexpr size_t nb_txq = 4;
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, nb_rxq, nb_txq, &conf, mp);
  }

  while (true) {

    for (size_t p=0; p<nb_ports; p++) {
      for (size_t q=0; q<nb_rxq; q++) {
        constexpr size_t BURSTSIZE = 32;
        rte_mbuf* mbufs[BURSTSIZE];
        size_t recvlen = rte_eth_rx_burst(p, q, mbufs, BURSTSIZE);
        if (recvlen != 0) {
          printf("port%zd:%zd: cnt=%zd \n", p, q, recvlen);
          for (size_t i=0; i<recvlen; i++) {
            printf(" mbus[%zd]: hash=%u \n", i, mbufs[i]->hash.rss);
          }
          slankdev::rte_pktmbuf_free_bulk(mbufs, recvlen);
        }
      }
    }
  }
}


