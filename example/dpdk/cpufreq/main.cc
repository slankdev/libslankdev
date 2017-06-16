
#include <stdio.h>
#include <assert.h>
#include <slankdev/extra/dpdk.h>
#include <slankdev/system.h>
#include <rte_power.h>

int func(void*)
{
  while (true) ;
}


int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  printf("nb lcore %u\n", rte_lcore_count());

  int ret = rte_power_init(1);
  printf("ret: %d\n", ret);

  rte_eal_remote_launch(func, nullptr, 1);
  sleep(3);
  printf("ffff\n");
  rte_eal_mp_wait_lcore();
}


