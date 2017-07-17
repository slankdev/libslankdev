
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
  printf(" %s\n", mac2vendor(m).c_str());
}

int main()
{
  uint8_t m[] = {00,00,00};
  test(m);
}
