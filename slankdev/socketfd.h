

/*
 * MIT License
 *
 * Copyright (c) 2017 Hiroki SHIROKURA
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
 * @file   slankdev/socketfd.h
 * @brief  socket wrapper
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#ifndef _LINUX_IF_H
#include <net/if.h>
#endif

#include <slankdev/intfd.h>



namespace slankdev {






/**
 * @class  socketfd
 * @breief wrapper class of socket
 */
class socketfd : public safe_intfd {
 public:
  socketfd() {}
  socketfd(int fd);
  bool dead() const;
  void socket(int domain, int type, int protocol);
  void connect(const struct sockaddr* addr, socklen_t addrlen);
  void bind(const struct sockaddr* sa, size_t len);
  void bind(uint32_t addr, uint16_t port);
  void listen(int backlog);
  int  accept(struct sockaddr* sa, socklen_t* len);
  void sendto(const void* buffer, size_t bufferlen,int flags,
      const struct sockaddr* dest_addr, socklen_t dest_len);
  size_t recvfrom(void* buffer, size_t bufferlen, int flags,
      struct sockaddr* address, socklen_t* address_len);
  void getsockopt(int level, int optname, void* optval, socklen_t *optlen);
  void setsockopt(int level, int optname, const void* optval, socklen_t optlen);
  void send(const void* buf, size_t nbyte, int flags);
  size_t recv(void* buf, size_t nbyte, int flags);
#ifdef __linux__
  void open_afpacket(const char* name);
#endif
  void open_connect(uint32_t addr, uint16_t port);
  void open_bind(uint32_t addr, uint16_t port);

  static void linkup(const char* name);
  static void linkdown(const char* name);
};


} /* namespace slankdev */





/*
 * Function Implementations
 */

#include <slankdev/intfd.h>
#include <slankdev/endian.h>

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef __linux__
#include <netpacket/packet.h>
#ifndef _NET_IF_H
#include <linux/if.h>
#endif
#include <linux/if_ether.h>
#endif

#include <string>
#include <slankdev/string.h>



namespace slankdev {


inline socketfd::socketfd(int fd) { set_fd(fd); }
inline bool socketfd::dead() const { return fd < 0; }
inline void socketfd::socket(int domain, int type, int protocol)
{
  fd = ::socket(domain, type, protocol);
  if (fd < 0) {
    perror("socket");
    exit(-1);
  }
}
inline void socketfd::connect(const struct sockaddr* addr, socklen_t addrlen)
{
  int res = ::connect(fd, addr, addrlen);
  if (res < 0) {
    perror("connect");
    exit(-1);
  }
}
inline void socketfd::bind(const struct sockaddr* sa, size_t len)
{
  int res = ::bind(fd, sa, len);
  if (res < 0) {
    perror("bind");
    exit(-1);
  }
}
inline void socketfd::bind(uint32_t addr, uint16_t port)
{
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(addr);
  bind((sockaddr*)&server, sizeof(server));
}
inline void socketfd::listen(int backlog)
{
  int res = ::listen(fd, backlog);
  if (res < 0) {
    perror("listen");
    exit(-1);
  }
}
inline int  socketfd::accept(struct sockaddr* sa, socklen_t* len)
{
  int res = ::accept(fd, sa, len);
  if (res < 0) {
    perror("accept");
    exit(-1);
  }
  return res;
}
inline void socketfd::sendto(const void* buffer, size_t bufferlen,int flags,
    const struct sockaddr* dest_addr, socklen_t dest_len)
{
  ssize_t res = ::sendto(fd, buffer, bufferlen, flags, dest_addr, dest_len);
  if (res < 0) {
    perror("sendto");
    exit(-1);
  }
}
inline size_t socketfd::recvfrom(void* buffer, size_t bufferlen, int flags,
    struct sockaddr* address, socklen_t* address_len)
{
  ssize_t res = ::recvfrom(fd, buffer, bufferlen, flags, address, address_len);
  if (res < 0) {
    perror("recvfrom");
    exit(-1);
  }
  return res;
}
inline void socketfd::getsockopt(int level, int optname, void* optval, socklen_t *optlen)
{
  int res = ::getsockopt(fd, level, optname, optval, optlen);
  if (res < 0) {
    perror("getsockopt");
    exit(-1);
  }
}
inline void socketfd::setsockopt(int level, int optname, const void* optval, socklen_t optlen)
{
  int res = ::setsockopt(fd, level, optname, optval, optlen);
  if (res < 0) {
    perror("setsockopt");
    exit(-1);
  }
}
inline void socketfd::send(const void* buf, size_t nbyte, int flags)
{
  int res = ::send(fd, buf, nbyte, flags);
  if (res < 0) {
    perror("send");
    exit(-1);
  }
}
inline size_t socketfd::recv(void* buf, size_t nbyte, int flags)
{
  ssize_t res = ::recv(fd, buf, nbyte, flags);
  if (res < 0) {
    perror("recv");
    exit(-1);
  }
  return res;
}
#ifdef __linux__
inline void socketfd::open_afpacket(const char* name)
{
  socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

  struct ifreq ifreq;
  memset(&ifreq, 0, sizeof(ifreq));
  strncpy(ifreq.ifr_name, name, sizeof(ifreq.ifr_name)-1);
  ioctl(SIOCGIFINDEX, &ifreq);

  struct sockaddr_ll sa;
  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = ifreq.ifr_ifindex;
  bind((struct sockaddr*)&sa, sizeof(sa));

  ioctl(SIOCGIFFLAGS, &ifreq);
  ifreq.ifr_flags = ifreq.ifr_flags | IFF_PROMISC;
  ioctl(SIOCSIFFLAGS, &ifreq);
}
#endif
inline void socketfd::open_connect(uint32_t addr, uint16_t port)
{
  socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server;
  server.sin_addr.s_addr = htonl(addr);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  connect(reinterpret_cast<struct sockaddr*>(&server), sizeof(server));
}

inline void socketfd::open_bind(uint32_t addr, uint16_t port)
{
  socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(addr);
  bind((sockaddr*)&server, sizeof(server));
}

inline void socketfd::linkup(const char* name)
{
  slankdev::socketfd sock;
  sock.socket(AF_INET, SOCK_DGRAM, 0);

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, name, IFNAMSIZ-1);
  sock.ioctl(SIOCGIFFLAGS, &ifr);
  ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
  sock.ioctl(SIOCSIFFLAGS, &ifr);
}

inline void socketfd::linkdown(const char* name)
{
  slankdev::socketfd sock;
  sock.socket(AF_INET, SOCK_DGRAM, 0);

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, name, IFNAMSIZ-1);
  sock.ioctl(SIOCGIFFLAGS, &ifr);
  ifr.ifr_flags &= ~IFF_UP;
  sock.ioctl(SIOCSIFFLAGS, &ifr);
}


} /* namespace slankdev */
