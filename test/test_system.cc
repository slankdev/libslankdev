
#include <stdio.h>
#include <slankdev/system.h>

int main()
{
  size_t np = slankdev::n_processors();
  printf("n_processors: %zd \n", np);
}

