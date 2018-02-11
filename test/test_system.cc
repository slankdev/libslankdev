
#include <stdio.h>
#include <slankdev/system.h>

int main()
{
#ifdef __linux__
  size_t np = slankdev::n_processors();
  printf("n_processors: %zd \n", np);
#endif
}

