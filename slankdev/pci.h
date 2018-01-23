
#pragma once
#include <slankdev/string.h>
#include <slankdev/exception.h>
#include <slankdev/filefd.h>

namespace slankdev {


inline void bind_pcidev(const char* pciaddr, const char* drv)
{
  std::string drv_path = slankdev::format(
      "/sys/bus/pci/drivers/%s/bind", drv);
  slankdev::filefd file;
  file.fopen(drv_path.c_str(), "w");
  file.fwrite(pciaddr, strlen(pciaddr), 1);
}

inline void unbind_pcidev(const char* pciaddr, const char* drv)
{
  std::string drv_path = slankdev::format(
      "/sys/bus/pci/drivers/%s/unbind", drv);
  slankdev::filefd file;
  file.fopen(drv_path.c_str(), "w");
  file.fwrite(pciaddr, strlen(pciaddr), 1);
}

inline std::string get_drv_name(const char* pciaddr)
{
  std::string path = slankdev::format(
      "/sys/bus/pci/devices/%s/uevent", pciaddr);
  slankdev::filefd file;
  file.fopen(path.c_str(), "r");

  while (true) {
    char str[1000];
    char name[1000];
    file.fgets(str, sizeof(str));
    int ret = sscanf(str, "DRIVER=%s\n", name);
    if (ret == 1) {
      return name;
    }
  }
}

inline std::string ifname2pciaddr(const char* ifname)
{
  std::string path = slankdev::format(
      "/sys/class/net/%s/device/uevent", ifname);
  slankdev::filefd file;
  file.fopen(path, "r");

  while (true) {
    std::string line = file.readline();
    char buf[1000];
    int ret = sscanf(line.c_str(), "PCI_SLOT_NAME=%s\n", buf);
    if (ret == 1) {
      return buf;
    }
  }
}


} /* namespace slankdev */
