

#include <stdio.h>
#include <slankdev/socketfd.h>

int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s ifname\n", argv[0]);
    return -1;
  }

  slankdev::socketfd::linkup(argv[1]);
}


