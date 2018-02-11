
#include <slankdev/util.h>
using namespace slankdev;

void print(size_t c)
{
  depth_fprintf(stdout, "hiroki c=%zd %s\n", c, c==0?"":"{");
  if (c < 1) return;
  {
    depth_fprintf_pusher p;
    print(c-1);
  }
  depth_fprintf(stdout, "}\n");
}

int main()
{
  print(3);
}

