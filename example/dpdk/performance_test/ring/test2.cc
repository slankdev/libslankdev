
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <slankdev/extra/dpdk.h>
#include <slankdev/system.h>
#include <slankdev/string.h>

const uint8_t packet[] = {0xee};

void test(size_t BURSTSIZE)
{
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  struct rte_mbuf* mbufs[BURSTSIZE];
  for (size_t i=0; i<BURSTSIZE; i++) {
    mbufs[i] = rte_pktmbuf_alloc(mp);
    slankdev::set_mbuf_raw(mbufs[i], packet, sizeof(packet));
  }

  struct rte_ring* ring = slankdev::ring_alloc("WK0", 1024);
  slankdev::safe_ring_enqueue_bulk(ring, (void**)mbufs, BURSTSIZE);

  uint64_t tmp;
  uint64_t enq_sum = 0;
  uint64_t deq_sum = 0;
  uint32_t lcore_id = -1;
  size_t loops = 10000000000; /* 10^10 */
  for (size_t count=0; count<loops; count++) {
    tmp = slankdev::rdtscp(&lcore_id);
    slankdev::safe_ring_dequeue_bulk(ring, (void**)mbufs, BURSTSIZE);
    deq_sum += slankdev::rdtscp(&lcore_id) - tmp;

    tmp = slankdev::rdtscp(&lcore_id);
    slankdev::safe_ring_enqueue_bulk(ring, (void**)mbufs, BURSTSIZE);
    enq_sum += slankdev::rdtscp(&lcore_id) - tmp;
  }
  printf("%10zd, %10lu, %10lu\n", BURSTSIZE, enq_sum/loops, deq_sum/loops);

  slankdev::rte_pktmbuf_free_bulk(mbufs, BURSTSIZE);
  rte_ring_free(ring);
  rte_mempool_free(mp);
}

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);

  printf("\n\n");
  printf("%10s, %10s, %10s\n", "bulksize", "enq[clk]", "deq[clk]");
  test(1);
  test(2);
  test(4);
  test(8);
  test(16);
  test(32);
}


