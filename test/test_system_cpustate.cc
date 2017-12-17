
#include <stdio.h>
#include <slankdev/system.h>


int main(void)
{
  slankdev::cpus_state cpus;

  while (true) {
    cpus.update();

    const size_t n_cores = cpus.n_processors();
    for (size_t i=0; i<n_cores; i++) {
      double u = cpus.get_processor(i).cpu_rate();
      printf("rate[%zd]: %.1f%% \n", i, u);
    }
    printf("----------------------\n");

    sleep(1);
  }
}

