
#include <slankdev/color.h>
#include <stdio.h>

void _color_test()
{
  printf("[ %s%s%s ] red     \n", RED, "NG", RESET);
  printf("[ %s%s%s ] green   \n", GRN, "OK", RESET);
  printf("[ %s%s%s ] yellow  \n", YEL, "OK", RESET);
  printf("[ %s%s%s ] blue    \n", BLU, "OK", RESET);
  printf("[ %s%s%s ] magenta \n", MAG, "OK", RESET);
  printf("[ %s%s%s ] cyan    \n", CYN, "OK", RESET);
  printf("[ %s%s%s ] white   \n", WHT, "OK", RESET);
}

int main()
{
  _color_test();
}

