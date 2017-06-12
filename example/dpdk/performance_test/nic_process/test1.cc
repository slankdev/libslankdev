
/*
 * Caluculate Only Rx Function Latency
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <slankdev/extra/dpdk.h>
#include <slankdev/system.h>


void test(size_t BURSTSIZE)
{
  uint64_t tmp;
  uint64_t rx_sum = 0;
  uint32_t lcore_id = -1;
  size_t loops = 10000000000;
  for (size_t count=0; count<loops; count++) {
    struct rte_mbuf* mbufs[BURSTSIZE];

    tmp = slankdev::rdtscp(&lcore_id);
    size_t nb_recv = rte_eth_rx_burst(0,0, mbufs, BURSTSIZE);
    rx_sum += slankdev::rdtscp(&lcore_id) - tmp;
    slankdev::rte_pktmbuf_free_bulk(mbufs, nb_recv);
  }
  printf("%10lu, %10lu\n", BURSTSIZE, rx_sum/loops);
}


int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  struct rte_eth_conf port_conf;
  slankdev::init_portconf(&port_conf);
  size_t nb_ports = rte_eth_dev_count();
  printf("%zd ports found \n", nb_ports);
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, 1, 1, &port_conf, mp);
  }

  printf("waiting port configuration is done...\n");
  // sleep(15);
  getchar();
  printf("start\n");

  printf("%10s, %10s \n", "bulksize", "rx[clk]");
  test(1);
  test(2);
  test(4);
  test(8);
  test(16);
  test(32);
}


