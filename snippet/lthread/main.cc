

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

#include <lthread.h>
#include <slankdev/extra/dpdk.h>


struct lthread *lt = NULL;
constexpr size_t interval = 1000;

void d(void *x)
{
  for (size_t i=0; i<5; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%ud%zd== ", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
  }
}

void c(void *x)
{
  for (size_t i=0; i<5; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%uc%zd== ", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
  }
}

void b(void *x)
{
  for (size_t i=0; i<5; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%ub%zd== ", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
  }
}

void a(void *arg)
{
  for (size_t i=0; i<7; i++) {
    uint8_t lid = rte_lcore_id();
    printf(" ==%ua%zd== ", lid, i);
    fflush(stdout);
    lthread_sleep(interval);
    if (i==1) lthread_create(&lt, b, NULL);
  }
}

void test(void*)
{
  for (size_t i=0; i<8; i++) {
    lthread_sleep(interval-100);
    printf("\n");
    lthread_sleep(100);
  }
}

int eal_thread_1(void*)
{
  lthread_create(&lt, a, NULL);
  lthread_create(&lt, test, NULL);
  lthread_run();
  printf("all done 1\n");
  return 0;
}

int eal_thread_2(void*)
{
  lthread_create(&lt, c, NULL);
  lthread_create(&lt, d, NULL);
  lthread_run();
  printf("all done 2\n");
  return 0;
}

int main(int argc, char **argv)
{
  slankdev::dpdk_boot(argc, argv);
  rte_eal_remote_launch(eal_thread_1, nullptr, 1);
  rte_eal_remote_launch(eal_thread_2, nullptr, 2);
  rte_eal_mp_wait_lcore();
  printf("all done master\n");
}



