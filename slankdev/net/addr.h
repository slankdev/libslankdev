
/*
 * MIT License
 * Copyright (c) 2018 Hiroki SHIROKURA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/net/addr.h
 * @author Hiroki SHIROKURA
 */

#pragma once
#include <string>
#include <stdint.h>
#include <stddef.h>
#include <slankdev/string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>


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
  // std::string str = slankdev::format("%02x.%02x.%02x.%02x",
  std::string str = slankdev::format("%d.%d.%d.%d",
      U.u8[0], U.u8[1], U.u8[2], U.u8[3]);
  return str;
}

inline std::string macaddr2str(uint8_t macaddr[6])
{
  std::string str = slankdev::format("%02x:%02x:%02x:%02x:%02x:%02x",
      macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
  return str;
}

inline uint32_t
inetaddr_net(uint8_t u1, uint32_t u2, uint32_t u3, uint32_t u4)
{
  union {
    uint8_t u8[4];
    uint32_t u32;
  } U;
  U.u8[0] = u1;
  U.u8[1] = u2;
  U.u8[2] = u3;
  U.u8[3] = u4;
  return U.u32;
}

inline uint32_t
inetaddr_host(uint8_t u1, uint32_t u2, uint32_t u3, uint32_t u4)
{
  uint32_t val = inetaddr_net(u1,u2,u3,u4);
  return ntohl(val);
}

} /* namespace slankdev */


