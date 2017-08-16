
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <slankdev/hexdump.h>
#include <slankdev/dns.h>


int main()
{
	uint8_t domain[] = {
      0x04,
      'd',
      'p',
      'd',
      'k',
      0x05,
      'n',
      'i',
      'n',
      'j',
      'a',
      0x00,
  };
	char str[100] = {0};
  using namespace slankdev;
  ssize_t len = domain_reader(domain, sizeof(domain), str, sizeof(str));
  printf("str=\"%s\" len: %zd\n", str, len);
}


