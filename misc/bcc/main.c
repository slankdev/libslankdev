
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  uint8_t* ptr;
  for (size_t i=0; ; i++) {
    /* usleep(100); */
    usleep(1000000);
    ptr = (uint8_t*)malloc(100000000);
    free(ptr);
    printf("%04zd: malloc and free\n", i);
  }
}

