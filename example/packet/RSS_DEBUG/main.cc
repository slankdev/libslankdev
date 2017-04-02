

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <slankdev/socketfd.h>
#include <slankdev/util.h>
#include <slankdev/net_header.h>
#include <slankdev/endian.h>
const char* ifname = "enp0s20u1";



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <slankdev/net_header.h>
#include <slankdev/hexdump.h>
#include <slankdev/endian.h>

#include <rte_thash.h>

int main()
{
  using namespace slankdev;

  std::vector<uint8_t> buf;
  const size_t packetlen = 64;
  buf.resize(packetlen);
  uint8_t* ptr = buf.data();

  ether* eh = reinterpret_cast<ether*>(ptr);
  eh->dst.addr_bytes[0] = 0x00;
  eh->dst.addr_bytes[1] = 0x11;
  eh->dst.addr_bytes[2] = 0x22;
  eh->dst.addr_bytes[3] = 0x33;
  eh->dst.addr_bytes[4] = 0x44;
  eh->dst.addr_bytes[5] = 0x55;
  eh->src.addr_bytes[0] = 0x00;
  eh->src.addr_bytes[1] = 0x11;
  eh->src.addr_bytes[2] = 0x22;
  eh->src.addr_bytes[3] = 0x33;
  eh->src.addr_bytes[4] = 0x44;
  eh->src.addr_bytes[5] = 0x55;
  eh->type = htons(0x0800);
  ptr += eh->hdr_len();

  ip* ih = reinterpret_cast<ip*>(ptr);
  ih->ver   = 4;
  ih->ihl   = 5;
  ih->tos   = 0x00;
  ih->len   = htons(ih->hdr_len() + 16);
  ih->id    = htons(0x0000);
  ih->off   = htons(0x0000);
  ih->ttl   = 0x40;
  ih->proto = 0x11;
  ih->sum   = htons(0x0000);
  ih->src.s_addr = htonl(0xc0a8000a);
  ih->dst.s_addr = htonl(0xc0a80002);
  ptr += ih->hdr_len();

  udp* uh = reinterpret_cast<udp*>(ptr);
  uh->src   = htons(1111);
  uh->dst   = htons(2222);
  uh->cksum = htons(0x0000);
  uh->len   = htons(8);
  ptr += uh->hdr_len();

  uint8_t* d = ptr;
  d[0] = 's';
  d[1] = 'l';
  d[2] = 'a';
  d[3] = 'n';
  d[4] = 'k';
  d[5] = 'd';
  d[6] = 'e';
  d[7] = 'v';

  uint8_t default_rss_key[] = {
    0x6d, 0x5a, 0x56, 0xda, 0x25, 0x5b, 0x0e, 0xc2,
    0x41, 0x67, 0x25, 0x3d, 0x43, 0xa3, 0x8f, 0xb0,
    0xd0, 0xca, 0x2b, 0xcb, 0xae, 0x7b, 0x30, 0xb4,
    0x77, 0xcb, 0x2d, 0xa3, 0x80, 0x30, 0xf2, 0x0c,
    0x6a, 0x42, 0xb7, 0x3b, 0xbe, 0xac, 0x01, 0xfa,
  };

  struct rte_ipv4_tuple tp;
  tp.src_addr = slankdev::hton(uint32_t(0xc0a8000a));
  tp.dst_addr = slankdev::hton(uint32_t(0xc0a80001));
  tp.sport    = slankdev::hton(uint16_t(0x1111));
  tp.dport    = slankdev::hton(uint16_t(0x2222));

  uint8_t raw[1000];
  uint32_t hash = rte_softrss(
      reinterpret_cast<uint32_t*>(&tp), sizeof(tp), default_rss_key);
  printf("hash: 0x%08x\n", hash);

  slankdev::socketfd sock;
  sock.open_afpacket(ifname);
  sock.write(buf.data(), buf.size());
}


