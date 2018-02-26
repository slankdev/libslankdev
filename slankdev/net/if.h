
#pragma once
#include <net/if.h>

namespace slankdev {

inline std::string if_index2name(uint32_t ifindex)
{
  char str[1000];
  return if_indextoname(ifindex, str);
}

inline uint32_t if_name2index(const char* ifname)
{
  return if_nametoindex(ifname);
}

} /* namespace slankdev */


