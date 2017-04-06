
#pragma once

#include <slankdev/mbuf.h>
#include <slankdev/net_header.h>
#include <slankdev/checksum.h>
#include <slankdev/endian.h>
#include <slankdev/hash.h>


namespace slankdev {


class packet {
 public:
  virtual ~packet() {}
  virtual void summary() const = 0;
};


class udp_packet : public packet {
 public:
  slankdev::mbuf   m;
  slankdev::udp*   uh;
  slankdev::ip*    ih;
  slankdev::ether* eh;
  udp_packet();

  void show() const;
  uint32_t hash() const;
  virtual void summary() const override;
};


inline uint32_t udp_packet::hash() const
{
  using namespace slankdev;
  rss_tuple rt;
  rt.src_addr = ntohl(ih->src.s_addr);
  rt.dst_addr = ntohl(ih->dst.s_addr);
  rt.sport    = ntohs(uh->src);
  rt.dport    = ntohs(uh->dst);

  return rt.hash(rss_key_def());
}


inline void udp_packet::summary() const
{
  printf("%s:%d -> %s:%d  0x%08x\n",
      ih->src.to_string().c_str(), ntohs(uh->src),
      ih->dst.to_string().c_str(), ntohs(uh->dst), hash());
}


inline udp_packet::udp_packet()
{
  const char* str = "slankdev";

  m.prepend(strlen(str));
  uint8_t* d = m.mtod();
  memcpy(d, str, strlen(str));

  m.prepend(sizeof(slankdev::udp));
  uh = reinterpret_cast<slankdev::udp*>(m.mtod());
  uh->src = htons(2794);
  uh->dst = htons(1766);

  uh->src = htons(0);
  uh->dst = htons(0);

  uh->len = m.dlen();
  uh->cksum = htons(0x0000);

  m.prepend(sizeof(slankdev::ip));
  ih = reinterpret_cast<slankdev::ip*>(m.mtod());
  ih->ver   = 4;
  ih->ihl   = 5;
  ih->len   = htons(m.dlen());
  ih->proto = 0x11;
  ih->sum   = htons(0x0000);
  ih->src.s_addr = htonl(0x420995bb);
  ih->dst.s_addr = htonl(0xa18e6450);

  m.prepend(sizeof(slankdev::ether));
  eh = reinterpret_cast<slankdev::ether*>(m.mtod());
  eh->type = htons(0x0800);
}


inline void udp_packet::show() const
{
  m.dump(stdout);
  printf("\n");
  eh->print();
  ih->print();
  uh->print();
}


} /* namespace slankdev */
