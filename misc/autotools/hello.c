
#include "hello.h"
#include <stdio.h>

void hello()
{
  printf("hello in %s:%d\n", __FILE__, __LINE__);
}
