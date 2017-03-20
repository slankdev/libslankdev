
#include <stdio.h>
#include <rte_eal.h>
volatile bool force_quit;
#include <lthread_api.h>

bool running;
struct lthread *lt[3];

static void lthread_start(void* arg)
{
  int* i = reinterpret_cast<int*>(arg);
  running = true;
  while (running) {
    printf("test %d\n", *i);
    lthread_sleep(1 * 1000 * 1000 * 1000);
    lthread_yield ();
  }
  lthread_exit (NULL);
}

static void lthread_fin(void*)
{
  lthread_sleep(1 * 1000 * 1000 * 1000);
  lthread_sleep(1 * 1000 * 1000 * 1000);
  printf("FINISH\n");

  running = false;
  lthread_cancel(lt[0]);
  lthread_cancel(lt[1]);
  printf("adfadf\n");
  int res;
  res = lthread_join(lt[0], NULL);
  printf("join[0] %d\n", res);
  res = lthread_join(lt[1], NULL);
  printf("join[1] %d\n", res);

  lthread_exit (NULL);
}


int main(int argc, char** argv)
{
  rte_eal_init(argc, argv);

  int one = 1, two = 2;
  lthread_create (&lt[0], -1, lthread_start, &one);
  lthread_create (&lt[1], -1, lthread_start, &two);
  lthread_create (&lt[2], -1, lthread_fin  , NULL);
  // lthread_run ();
  // int res = lthread_join(lt[2], NULL);
  // printf("join[2] %d\n", res);
  // printf("lthread finished \n");
}
