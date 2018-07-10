
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  uint8_t* ptr;
  while (true) {
    usleep(100);
    ptr = (uint8_t*)malloc(100000000);
  }
}

