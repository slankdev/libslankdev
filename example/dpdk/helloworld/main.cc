
#include <stdio.h>
#include <assert.h>
#include <slankdev/extra/dpdk.h>
#include <slankdev/system.h>


int thread(void*)
{
  uint8_t lid = rte_lcore_id();
  uint32_t i = 0xbeaf;
  uint64_t c = slankdev::rdtscp(&i);
  assert(lid == i);
  printf("hello world lcoreid=%u, tscp=%lu\n", lid, c);
}

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  // rte_eal_remote_launch(thread, nullptr, 1);
  // rte_eal_remote_launch(thread, nullptr, 2);
  rte_eal_mp_wait_lcore();
}

