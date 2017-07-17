

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <slankdev/mac2vendor.h>

static void test(uint8_t* m)
{
  using namespace slankdev;
  for (size_t i=0; i<3; i++) {
    printf("%02x%s", m[i], i+1<3?":":"");
  }
  printf(" --> %s\n", mac2vendor(m).c_str());
}

int main()
{
  uint8_t list[][3] = {
    {0x52,0x54,0x00},
    {0x70,0x4d,0x7b},
    {0xcc,0xe1,0xd5},
    {0x80,0xe6,0x50},
  };

  test(list[0]);
  test(list[1]);
  test(list[2]);
  test(list[3]);
}


