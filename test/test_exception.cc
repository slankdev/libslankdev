
#include <stdio.h>
#include <string>
#include <slankdev/exception.h>

int main()
{
  try {
    throw slankdev::exception("const char*");
  } catch (std::exception& e) {
    printf("CATCH: %s \n", e.what());
  }

  try {
    std::string s = "std::string";
    throw slankdev::exception(s);
  } catch (std::exception& e) {
    printf("CATCH: %s \n", e.what());
  }
}

