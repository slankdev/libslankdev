
#include <stdio.h>
#include <stdint.h>
#include <slankdev/mac2vendor.h>

static void test(uint8_t* m)
{
  for (size_t i=0; i<3; i++) {
    printf("%02x%s", i+1<3:":","");
  }
  printf(" %s\n", mac2vendor(m).c_str());
}

int main()
{
  test({00,00,00});
}
