
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include <net/if.h>
#include <netinet/in.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>


int open_socket(const char* name);
void send_packet(int fd, const void* buf, size_t size);
size_t recv_packet(int fd, void* buf, size_t size);
void hexdump(const void* buf, size_t size);


inline int open_socket(const char* name)
{
  int ret;
  int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (ret < 0) {
    perror("socket");
    exit(-1);
  }

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name)-1);
  ret = ioctl(fd, SIOCGIFINDEX, &ifr);
  if (ret < 0) {
    perror("ioctl_getifindex");
    exit(-1);
  }

  struct sockaddr_ll sa;
  sa.sll_family = PF_PACKET;
  sa.sll_protocol = htons(ETH_P_ALL);
  sa.sll_ifindex = ifr.ifr_ifindex;
  ret = bind(fd, (struct sockaddr*)&sa, sizeof(sa));
  if (ret < 0) {
    perror("bind");
    exit(-1);
  }

  ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
  if (ret < 0) {
    perror("ioctl");
    exit(-1);
  }

  ifr.ifr_flags = ifr.ifr_flags | IFF_PROMISC;
  ret = ioctl(fd, SIOCSIFFLAGS, &ifr);
  if (ret < 0) {
    perror("ioctl");
    exit(-1);
  }
  return fd;
}

inline void send_packet(int fd, const void* buf, size_t size)
{
  int ret = write(fd, buf, size);
  if (ret < 0) {
    perror("write");
    exit(-1);
  }
}

inline size_t recv_packet(int fd, void* buf, size_t size)
{
  int ret = read(fd, buf, size);
  if (ret < 0) {
    perror("read");
    exit(-1);
  }
  return ret;
}

inline void hexdump(const void* buf, size_t bufferlen)
{
  FILE* fp = stdout;
  const uint8_t* data = (const uint8_t*)buf;
  size_t row = 0;
  while (bufferlen > 0) {
    fprintf(fp, "%04zx:   ", row);

    size_t n;
    if (bufferlen < 16) n = bufferlen;
    else                n = 16;

    for (size_t i = 0; i < n; i++) {
      if (i == 8) fprintf(fp, " ");
      fprintf(fp, " %02x", data[i]);
    }
    for (size_t i = n; i < 16; i++) {
      fprintf(fp, "   ");
    }
    fprintf(fp, "   ");
    for (size_t i = 0; i < n; i++) {
      if (i == 8) fprintf(fp, "  ");
      uint8_t c = data[i];
      if (!(0x20 <= c && c <= 0x7e)) c = '.';
      fprintf(fp, "%c", c);
    }
    fprintf(fp, "\n");
    bufferlen -= n;
    data += n;
    row  += n;
  }
}

