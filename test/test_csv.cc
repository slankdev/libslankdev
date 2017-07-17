
#include <stdio.h>
#include <slankdev/csv.h>

int main(int, char**)
{
  slankdev::csv csv;
  csv.read("/home/slank/oui.csv");

  size_t row = csv.get_row();
  for (size_t r=1; row; r++) {
    printf("%-30s, %-s \n",
        csv.get(r, 1).c_str(),
        csv.get(r, 2).c_str());
  }
}

