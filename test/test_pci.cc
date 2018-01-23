
#include <stdio.h>
#include <string>
#include <slankdev/pci.h>

int main()
{
  using namespace slankdev;
  std::string s = get_drv_name("0000:3b:00.0");
  printf("%s\n", s.c_str());

  std::string ss = ifname2pciaddr("eth6");
  printf("%s\n", ss.c_str());

  // unbind_pcidev("0000:3b:00.0", "uio_pci_generic");
  // bind_pcidev("0000:3b:00.0", "ixgbe");
}
