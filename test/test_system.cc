
#include <stdio.h>
#include <slankdev/system.h>

int main()
{
#ifdef __linux__
  size_t np = slankdev::n_processors();
  printf("n_processors: %zd \n", np);
#endif

  printf("---------------\n");
  printf("%s\n", slankdev::uname_get_sysname ().c_str());
  printf("%s\n", slankdev::uname_get_nodename().c_str());
  printf("%s\n", slankdev::uname_get_release ().c_str());
  printf("%s\n", slankdev::uname_get_version ().c_str());
  printf("%s\n", slankdev::uname_get_machine ().c_str());
}

