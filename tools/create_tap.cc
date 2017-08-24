

#include <stdio.h>
#include <slankdev/net/tuntap.h>

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s ifname\n", argv[0]);
    return -1;
  }

  slankdev::tap tap(argv[1]);
  while (1) {}
}


