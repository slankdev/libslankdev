
#include <slankdev/util.h>
using namespace slankdev;

void print(size_t c=3)
{
  if (c < 1) return;
  depth_fprintf_pusher p;
  depth_fprintf(stdout, "hiroki c=%zd {\n", c);
  print(c-1);
  depth_fprintf(stdout, "}\n");
}


int main()
{
  print();
  // depth_fprintf(stdout, "hiroki\n");
  // {
  //   depth_fprintf(stdout, "child\n");
  // }
  // depth_fprintf(stdout, "parent\n");

}
