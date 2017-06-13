

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include <lthread.h>
#include <slankdev/extra/dpdk.h>


struct lthread *lt = NULL;
constexpr size_t interval = 1000;

void b(void *x)
{
  for (size_t i=0; i<4; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%ub%zd== \n", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
  }
  printf("quit b\n");
}

void a(void *arg)
{
  for (size_t i=0; i<4; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%ua%zd== \n", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
    if (i==1) lthread_create(&lt, b, NULL);
  }
  printf("quit a\n");
}

int eal_thread_1(void*)
{
  lthread_create(&lt, a, NULL);
  lthread_run();
  printf("all done 1\n");
  return 0;
}

int main(int argc, char **argv)
{
  slankdev::dpdk_boot(argc, argv);
  rte_eal_remote_launch(eal_thread_1, nullptr, 1);
  printf("rerer\n");
  rte_eal_mp_wait_lcore();
  printf("all done master\n");
}



