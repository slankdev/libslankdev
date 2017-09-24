
#include <stdio.h>
#include <slankdev/net/addr.h>
#include <slankdev/endian.h>

int main()
{
  using namespace slankdev;
  uint32_t ia = 0x01020304;
  uint8_t ma[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
  printf("inet addr: %s\n", inaddr2str(bswap32(ia)).c_str());
  printf("mac addr : %s\n", macaddr2str(ma).c_str());
}
