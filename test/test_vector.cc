
#include <slankdev/hexdump.h>
#include <slankdev/vector.h>

int main()
{
  slankdev::fixed_size_vector<size_t> vec(3);
  vec[2] = 4;
  vec.at(5) = 2;
}
