
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <slankdev/extra/dpdk.h>
#include <slankdev/system.h>

const uint8_t packet[64] = {0xee};

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  struct rte_mbuf* mbuf = rte_pktmbuf_alloc(mp);
  slankdev::set_mbuf_raw(mbuf, packet, sizeof(packet));

  struct rte_ring* ring = slankdev::ring_alloc("WK0", 1024);
  slankdev::safe_ring_enqueue(ring, mbuf);

  uint64_t tmp;
  uint64_t enq_sum = 0;
  uint64_t deq_sum = 0;
  uint32_t lcore_id = -1;
  size_t loops = 10000000000;
  printf("loops %lu \n", loops);
  for (size_t count=0; count<loops; count++) {
    tmp = slankdev::rdtscp(&lcore_id);
    slankdev::safe_ring_dequeue(ring, (void**)&mbuf);
    deq_sum += slankdev::rdtscp(&lcore_id) - tmp;

    tmp = slankdev::rdtscp(&lcore_id);
    slankdev::safe_ring_enqueue(ring, mbuf);
    enq_sum += slankdev::rdtscp(&lcore_id) - tmp;
  }
  printf("\n\n");
  printf("%10s, %10s, %10s\n", "", "enq[clk]", "deq[clk]");
  printf("%10s, %10lu, %10lu\n", "1shot", enq_sum/loops, deq_sum/loops);
}


