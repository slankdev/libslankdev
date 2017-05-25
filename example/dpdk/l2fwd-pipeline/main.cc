
#include <stdio.h>
#include <stdlib.h>
#include <slankdev/extra/dpdk.h>


struct wk_arg {
  struct rte_ring* src;
  struct rte_ring* dst;
};

constexpr size_t BURSTSZ = 32;
size_t cnt_call_wk[10];

int wk(void* arg)
{
  uint8_t lcoreid = rte_lcore_id();
  printf("launched wk on lcore%u \n", lcoreid);
  struct wk_arg* wk = reinterpret_cast<wk_arg*>(arg);
  while (true) {
    struct rte_mbuf* mbufs[BURSTSZ];
    size_t nb_deq = rte_ring_dequeue_bulk(wk->src, (void**)mbufs, sizeof(mbufs), nullptr);
    if (nb_deq == 0) continue;

#if 1 // dummiy
    size_t a = cnt_call_wk[lcoreid];
    for (size_t i=0; i<nb_deq; i++) {
      // cnt_call_wk[lcoreid] ++;
      a ++;
      struct rte_mbuf* m = mbufs[i];
      rte_prefetch0(rte_pktmbuf_mtod(m, void*));
      rte_pktmbuf_mtod(m, uint8_t*)[12] = 0x11;
      uint8_t* p = rte_pktmbuf_mtod(m, uint8_t*);
      p[12] = 0x00;
      p[40] = 0x00;
      auto s = p[43];
      if (p[12]==0x08 && p[13]==0x06)  p[12]=0;
    }
    cnt_call_wk[lcoreid] = a;
#endif

    size_t nb_enq = rte_ring_enqueue_bulk(wk->dst, (void**)mbufs, nb_deq, nullptr);
    if (nb_enq < nb_deq)
      slankdev::rte_pktmbuf_free_bulk(&mbufs[nb_enq], nb_deq-nb_enq);
  }
  return 0;
}

int l2fwd(void* arg)
{
  printf("launched l2fwd on lcore%u \n", rte_lcore_id());
  struct wk_arg* wk = reinterpret_cast<wk_arg*>(arg);
  size_t nb_ports = rte_eth_dev_count();
  while (true) {
    for (size_t pid=0; pid<nb_ports; pid++) {
      rte_mbuf* mbufs[BURSTSZ];

      constexpr size_t nb_q = 1;
      size_t nb_recv = rte_eth_rx_burst(pid, 0, mbufs, BURSTSZ);
      if (nb_recv == 0) continue;

      size_t nb_enq = rte_ring_enqueue_bulk(wk->src, (void**)mbufs, nb_recv, nullptr);
      if (nb_enq < nb_recv)
        slankdev::rte_pktmbuf_free_bulk(&mbufs[nb_enq], nb_recv-nb_enq);

      size_t nb_deq = rte_ring_dequeue_bulk(wk->dst, (void**)mbufs, sizeof(mbufs), nullptr);
      size_t nb_send = rte_eth_tx_burst(pid^1, 0, mbufs, nb_deq);
      if (nb_send < nb_deq)
        slankdev::rte_pktmbuf_free_bulk(&mbufs[nb_send], nb_deq-nb_send);
    }
#if 0
    printf("-------------------\n");
    for (size_t i=0; i<10; i++) {
      printf("wk[%zd] call %zd times\n",i, cnt_call_wk[i]);
    }
    printf("-------------------\n");
#endif
  }
  return 0;
}

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_eth_conf port_conf;
  slankdev::init_portconf(&port_conf);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  size_t nb_ports = rte_eth_dev_count();
  printf("%zd ports found \n", nb_ports);
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, 1, 1, &port_conf, mp);
  }

  struct rte_ring* r0 = slankdev::ring_alloc("WK0", 1024);
  struct rte_ring* r1 = slankdev::ring_alloc("WK1", 1024);
  struct rte_ring* r2 = slankdev::ring_alloc("WK2", 1024);
  struct rte_ring* r3 = slankdev::ring_alloc("WK3", 1024);
  struct rte_ring* r4 = slankdev::ring_alloc("WK4", 1024);
  struct rte_ring* r5 = slankdev::ring_alloc("WK5", 1024);
  struct rte_ring* r6 = slankdev::ring_alloc("WK6", 1024);

  struct wk_arg fwd_arg = {r0, r6};
  struct wk_arg wk0_arg = {r0, r1};
  struct wk_arg wk1_arg = {r1, r2};
  struct wk_arg wk2_arg = {r2, r3};
  struct wk_arg wk3_arg = {r3, r4};
  struct wk_arg wk4_arg = {r4, r5};
  struct wk_arg wk5_arg = {r5, r6};

  rte_eal_remote_launch(l2fwd, &fwd_arg, 1);
  rte_eal_remote_launch(wk   , &wk0_arg, 2);
  rte_eal_remote_launch(wk   , &wk1_arg, 3);
  rte_eal_remote_launch(wk   , &wk2_arg, 4);
  rte_eal_remote_launch(wk   , &wk3_arg, 5);
  rte_eal_remote_launch(wk   , &wk4_arg, 6);
  rte_eal_remote_launch(wk   , &wk5_arg, 7);
  rte_eal_mp_wait_lcore();
}

