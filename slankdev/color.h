
#pragma once

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#include <stdio.h>
void color_test()
{
  printf("[ %s%s%s ] red     \n", RED, "NG", RESET);
  printf("[ %s%s%s ] green   \n", GRN, "OK", RESET);
  printf("[ %s%s%s ] yellow  \n", YEL, "OK", RESET);
  printf("[ %s%s%s ] blue    \n", BLU, "OK", RESET);
  printf("[ %s%s%s ] magenta \n", MAG, "OK", RESET);
  printf("[ %s%s%s ] cyan    \n", CYN, "OK", RESET);
  printf("[ %s%s%s ] white   \n", WHT, "OK", RESET);
}

#if 0
int main()
{
  color_test();
}
#endif
