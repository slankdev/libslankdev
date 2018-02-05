
#pragma once

#include <slankdev/net/mbuf.h>
#include <slankdev/net/hdr.h>
#include <slankdev/checksum.h>
#include <slankdev/endian.h>
#include <slankdev/hash.h>


namespace slankdev {

inline const uint8_t* raw_arp_pack()
{
  static uint8_t arp_packet[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x11, 0x22,
    0x33, 0x44, 0x55, 0x66, 0x08, 0x06, 0x00, 0x01,
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00,
  };
  return arp_packet;
}

inline const uint8_t* raw_tcp_pack()
{
  static uint8_t tcp_packet[] = {
    0x84, 0xdb, 0xac, 0x31, 0x1d, 0xfa, 0x80, 0xe6,
    0x50, 0x17, 0x18, 0x46, 0x08, 0x00, 0x45, 0x00,
    0x00, 0x80, 0x91, 0xb3, 0x40, 0x00, 0x40, 0x06,
    0x3b, 0x57, 0xc0, 0xa8, 0x64, 0x67, 0xa3, 0x2c,
    0xa5, 0x31, 0x81, 0x42, 0x00, 0x50, 0x9b, 0x4c,
    0x6e, 0x59, 0x4a, 0x9e, 0x73, 0x7e, 0x80, 0x18,
    0x00, 0xe5, 0x54, 0xd9, 0x00, 0x00, 0x01, 0x01,
    0x08, 0x0a, 0x00, 0x18, 0x00, 0x16, 0x4e, 0xf8,
    0xcd, 0x22, 0x47, 0x45, 0x54, 0x20, 0x2f, 0x20,
    0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31,
    0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20,
    0x73, 0x6c, 0x61, 0x6e, 0x6b, 0x64, 0x65, 0x76,
    0x2e, 0x6e, 0x65, 0x74, 0x0d, 0x0a, 0x55, 0x73,
    0x65, 0x72, 0x2d, 0x41, 0x67, 0x65, 0x6e, 0x74,
    0x3a, 0x20, 0x63, 0x75, 0x72, 0x6c, 0x2f, 0x37,
    0x2e, 0x35, 0x30, 0x2e, 0x33, 0x0d, 0x0a, 0x41,
    0x63, 0x63, 0x65, 0x70, 0x74, 0x3a, 0x20, 0x2a,
    0x2f, 0x2a, 0x0d, 0x0a, 0x0d, 0x0a,
  };
  return tcp_packet;
}



class packet {
 public:
  virtual ~packet() {}
  virtual void summary() const = 0;
};

#if 0
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
#endif


class pkt_analyzer {

 public:

  enum pkt_type {
    proto_eth,
    proto_ipv4,
    proto_ipv6,
    proto_arp,
    proto_icmp,
    proto_tcp,
    proto_udp,
    proto_none,
  };

  static std::string pkt_type2str(pkt_type t)
  {
    switch (t) {
      case proto_eth     : return "eth" ;
      case proto_ipv4    : return "ipv4";
      case proto_ipv6    : return "ipv6";
      case proto_arp     : return "arp" ;
      case proto_icmp    : return "icmp";
      case proto_tcp     : return "tcp" ;
      case proto_udp     : return "udp" ;
      case proto_none    : return "none";
      default: throw slankdev::exception("unknown id=sfnau");
    }
  }

  struct hdr_info {
    size_t hdr_len;
    pkt_type next_proto;
    hdr_info(size_t hdrlen, pkt_type nextproto)
      : hdr_len(hdrlen), next_proto(nextproto) {}
  };

 private:

  static hdr_info analyze_eth_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(ether)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }

    const ether* eh = reinterpret_cast<const ether*>(ptr);
    uint16_t type = ntohs(eh->type);
    switch (type) {
    case 0x0800: return hdr_info(sizeof(ether), proto_ipv4);
    case 0x86dd: return hdr_info(sizeof(ether), proto_ipv6);
    case 0x0806: return hdr_info(sizeof(ether), proto_arp );
    default:     return hdr_info(sizeof(ether), proto_none);
    } /* switch */
  }

  static hdr_info analyze_ip4_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(ip)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }

    const ip* ih = reinterpret_cast<const ip*>(ptr);
    uint8_t proto = ih->proto;
    size_t hdrlen = ((0x0f & ih->ver_ihl) << 4) << 2;
    switch (proto) {
    case 0x01: return hdr_info(hdrlen, proto_icmp);
    case 0x06: return hdr_info(hdrlen, proto_tcp);
    case 0x11: return hdr_info(hdrlen, proto_udp);
    } /* switch */
  }

  static hdr_info analyze_ip6_hdr(const void* ptr, size_t len)
  {
    printf("this is non excepted function (%s())\n", __func__);
    return hdr_info(0, proto_none);
  }

  static hdr_info analyze_udp_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(udp)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }
    size_t hdrlen = sizeof(udp);
    return hdr_info(hdrlen, proto_none);
  }

  static hdr_info analyze_tcp_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(tcp)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }

    const tcp* th = reinterpret_cast<const tcp*>(ptr);
    size_t hdrlen = th->data_off * 4;
    return hdr_info(hdrlen, proto_none);
  }

  static hdr_info analyze_arp_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(arp)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }
    size_t hdrlen = sizeof(arp);
    return hdr_info(hdrlen, proto_none);
  }

  static hdr_info analyze_icmp_hdr(const void* ptr, size_t len)
  {
    if (len < sizeof(icmp)) {
      std::string err = format("invalid length (%zd)", len);
      throw slankdev::exception(err.c_str());
    }
    size_t hdrlen = sizeof(icmp);
    return hdr_info(hdrlen, proto_none);
  }

  static hdr_info
  analyze_hdr(const void* ptr, size_t len, pkt_type proto)
  {
    switch (proto) {
      case proto_eth     : return analyze_eth_hdr (ptr, len);
      case proto_ipv4    : return analyze_ip4_hdr (ptr, len);
      case proto_ipv6    : return analyze_ip6_hdr (ptr, len);
      case proto_arp     : return analyze_arp_hdr (ptr, len);
      case proto_icmp    : return analyze_icmp_hdr(ptr, len);
      case proto_tcp     : return analyze_tcp_hdr (ptr, len);
      case proto_udp     : return analyze_udp_hdr (ptr, len);
      case proto_none    : throw slankdev::exception("not exept this id=ruan");
      default: throw slankdev::exception("unknown id=rykvwuv");
    }
  }


 public:

  static std::vector<pkt_type>
  guess_protostack(const void* _ptr_, size_t len)
  {
    std::vector<pkt_type> vec;
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(_ptr_);
    pkt_type cur_proto = proto_eth;
    vec.push_back(proto_eth);
    for (size_t nstack_depth=1; ; nstack_depth++) {
      assert(nstack_depth < 20);
      hdr_info hi = analyze_hdr(ptr, len, cur_proto);
      pkt_type next_proto  = hi.next_proto;
      if (next_proto == proto_none) {
        return vec;
      }

      ptr += hi.hdr_len;
      len -= hi.hdr_len;
      cur_proto = next_proto;
      vec.push_back(cur_proto);
    }
    /* not reachable */
  }

  static std::string
  guess_protoname(const void* _ptr_, size_t len)
  {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(_ptr_);
    pkt_type cur_proto = proto_eth;
    for (size_t nstack_depth=1; ; nstack_depth++) {
      assert(nstack_depth < 20);
      hdr_info hi = analyze_hdr(ptr, len, cur_proto);
      pkt_type next_proto  = hi.next_proto;
      if (next_proto == proto_none) {
        return pkt_type2str(cur_proto);
      }

      ptr += hi.hdr_len;
      len -= hi.hdr_len;
      cur_proto = next_proto;
    }
    /* not reachable */
  }

}; /* class pkt_analyzer */



} /* namespace slankdev */
