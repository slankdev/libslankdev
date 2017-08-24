
#pragma once
#include <string>
#include <stdint.h>
#include <stddef.h>
#include <slankdev/string.h>


/*
 * Prototypes
 */
namespace slankdev {
std::string inaddr2str(uint32_t inaddr);
std::string macaddr2str(uint8_t macaddr[6]);
} /* namespace slankdev */


/*
 * Implementations
 */
namespace slankdev {

inline std::string inaddr2str(uint32_t inaddr)
{
  union {
    uint8_t  u8[4];
    uint32_t u32;
  } U;
  U.u32 = inaddr;
  std::string str = slankdev::format("%02x.%02x.%02x.%02x",
      U.u8[0], U.u8[1], U.u8[2], U.u8[3]);
  return str;
}

inline std::string macaddr2str(uint8_t macaddr[6])
{
  std::string str = slankdev::format("%02x:%02x:%02x:%02x:%02x:%02x",
      macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
  return str;
}

} /* namespace slankdev */


