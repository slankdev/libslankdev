
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
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
 * @file   slankdev/net/hdr.h
 * @brief  nw protocol header structure
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
#include <slankdev/endian.h>
#include <slankdev/string.h>
#include <slankdev/checksum.h>
#include <arpa/inet.h>



namespace slankdev {



#pragma pack(1)


struct ether_addr {
  uint8_t addr_bytes[6];

  std::string to_string() const
  {
    static char str[256];
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
        addr_bytes[0], addr_bytes[1], addr_bytes[2],
        addr_bytes[3], addr_bytes[4], addr_bytes[5]);
    return str;
  }
};


struct ether {
  slankdev::ether_addr dst;
  slankdev::ether_addr src;
  uint16_t type;

  void print(FILE* fp) const
  {
    fprintf(fp, "Ether header\n");
    fprintf(fp, "+ dst  : %s    \n", dst.to_string().c_str());
    fprintf(fp, "+ src  : %s    \n", src.to_string().c_str());
    fprintf(fp, "+ type : 0x%04x\n", ntohs(type));
  }
  size_t hdr_len() const { return 14; }
};



#if 0
struct in_addr {
  uint32_t s_addr;

  void set(uint32_t u) { s_addr = htonl(u); }
  uint32_t get() const { return ntohl(s_addr); }
  std::string to_string() const
  {
    union {
      uint8_t u8[4];
      uint32_t u32;
    } U;

    U.u32 = s_addr;
    static char str[256];
    sprintf(str, "%d.%d.%d.%d",
        U.u8[0], U.u8[1], U.u8[2], U.u8[3]);
    return str;
  }
};
#endif



struct ip {
  uint8_t 	ver_ihl;
  uint8_t   tos    ;        /* type of service        */
  uint16_t  tot_len;        /* total length           */
  uint16_t  id     ;        /* identification         */
  uint16_t  off    ;        /* fragment offset field  */
  uint8_t   ttl    ;        /* time to live (TTL)     */
  uint8_t   proto  ;        /* protocol               */
  uint16_t  sum    ;        /* header checksum        */
  uint32_t  src; /* source ip address      */
  uint32_t  dst; /* destination ip address */

  void print(FILE* fp) const
  {
    fprintf(fp,"IP header\n");
    fprintf(fp,"+ ver  : %u        \n", (ver_ihl&0xf0)>>4);
    fprintf(fp,"+ ihl  : %u        \n", (ver_ihl&0x0f)   );
    fprintf(fp,"+ tos  : 0x%02x    \n", tos       );
    fprintf(fp,"+ tot_len: %u        \n", ntohs(tot_len));
    fprintf(fp,"+ id   : 0x%04x    \n", ntohs(id ));
    fprintf(fp,"+ off  : 0x%04x    \n", ntohs(off));
    fprintf(fp,"+ ttl  : %u (0x%x) \n", ttl  , ttl  );
    fprintf(fp,"+ proto: %u (0x%x) \n", proto, proto);
    fprintf(fp,"+ sum  : 0x%04x    \n", ntohs(sum ));
    fprintf(fp,"+ src  : 0x%08x  \n", bswap32(src));
    fprintf(fp,"+ dst  : 0x%08x  \n", bswap32(dst));
  }
  size_t hdr_len() const { return (ver_ihl&0x0f)<<2; }
  void* get_next() const
  {
    uint8_t* ptr = (uint8_t*)this;
    return ptr + hdr_len();
  }
};


struct ip6 {
  uint32_t vtc_flow;
  uint16_t payload_len;
  uint8_t  proto;
  uint8_t  hop_limits;
  struct in6_addr src;
  struct in6_addr dst;

  void print(FILE* fp) const
  {
    fprintf(fp, "IPv6\n");
    fprintf(fp, "+ vtc_flow   : 0x%08x(%u)\n", vtc_flow   , vtc_flow   );
    fprintf(fp, "+ payload_len: 0x%08x(%u)\n", payload_len, payload_len);
    fprintf(fp, "+ proto      : 0x%08x(%u)\n", proto      , proto      );
    fprintf(fp, "+ hop_limits : 0x%08x(%u)\n", hop_limits , hop_limits );
    char src_buf[100], dst_buf[100];
    inet_ntop(AF_INET6, &src, src_buf, sizeof(src_buf));
    inet_ntop(AF_INET6, &dst, dst_buf, sizeof(dst_buf));
    fprintf(fp, "+ src        : %s\n", src_buf);
    fprintf(fp, "+ dst        : %s\n", dst_buf);
  }
  size_t hdr_len() const { return 40; }
  const void* get_next() const
  {
    uint8_t* ptr = (uint8_t*)this;
    return ptr + hdr_len();
  }
};

struct srh {
  uint8_t  next_hdr;
  uint8_t  hdr_ext_len;
  uint8_t  routing_type;
  uint8_t  segments_left;
  uint8_t  last_entry;
  uint8_t  flags;
  uint16_t tag;
  struct in6_addr segment_list[100];

  void print(FILE* fp) const
  {
    fprintf(fp, "SRH\n");
    fprintf(fp, "+ next_hdr     : 0x%02x(%u)\n", next_hdr     , next_hdr     );
    fprintf(fp, "+ hdr_ext_len  : 0x%02x(%u)\n", hdr_ext_len  , hdr_ext_len  );
    fprintf(fp, "+ routing_type : 0x%02x(%u)\n", routing_type , routing_type );
    fprintf(fp, "+ segments_left: 0x%02x(%u)\n", segments_left, segments_left);
    fprintf(fp, "+ last_entry   : 0x%02x(%u)\n", last_entry   , last_entry   );
    fprintf(fp, "+ flags        : 0x%02x(%u)\n", flags        , flags        );
    fprintf(fp, "+ tag          : 0x%04x(%u)\n", tag          , tag          );
    fprintf(fp, "+ slist: [");
    const size_t n_loop = segments_left + 1;
    for (size_t i=0; i<n_loop; i++) {
      char strbuf[100];
      inet_ntop(AF_INET6,
          (const void*)&segment_list[i],
          strbuf, sizeof(strbuf));
      printf("%s%s", strbuf, i+1<n_loop?",":"]\n");
    }
  }

  size_t hdr_len() const { return hdr_ext_len * 8 + 8; }
  const void* get_next() const
  {
    uint8_t* ptr = (uint8_t*)this;
    return ptr + hdr_len();
  }
};

static inline std::string
in6_addr_to_string(const struct in6_addr* addr)
{
  char buf[100];
  inet_ntop(AF_INET6, addr, buf, sizeof(buf));
  return buf;
}

struct arp {
  uint16_t    hwtype   ;
  uint16_t    ptype    ;
  uint8_t     hwlen    ;
  uint8_t     plen     ;
  uint16_t    operation;
  ether_addr  hwsrc    ;
  uint32_t    psrc     ;
  ether_addr  hwdst    ;
  uint32_t    pdst     ;

  void print(FILE* fp) const
  {
    fprintf(fp, "ARP header\n");
    fprintf(fp, "+ hwtype   : %u\n", ntohs(hwtype   ));
    fprintf(fp, "+ ptype    : %u\n", ntohs(ptype    ));
    fprintf(fp, "+ hwlen    : %u\n", hwlen    );
    fprintf(fp, "+ plen     : %u\n", plen     );
    fprintf(fp, "+ operation: %u\n", ntohs(operation));
    fprintf(fp, "+ hwsrc    : %s\n", hwsrc.to_string().c_str());
    fprintf(fp, "+ psrc     : 0x%08x  \n", bswap32(psrc));
    fprintf(fp, "+ hwdst    : %s\n", hwdst.to_string().c_str());
    fprintf(fp, "+ pdst     : 0x%08x  \n", bswap32(pdst));
  }
  size_t hdr_len() const { return sizeof(arp); }
};




struct udp {
  uint16_t sport;
  uint16_t dport;
  uint16_t len  ;
  uint16_t cksum;

  void print(FILE* fp) const
  {
    fprintf(fp, "UDP header\n");
    fprintf(fp, "+ sport: %u (0x%04x)\n", ntohs(sport), ntohs(sport));
    fprintf(fp, "+ dport: %u (0x%04x)\n", ntohs(dport), ntohs(dport));
    fprintf(fp, "+ len  : %u\n", ntohs(len  ));
    fprintf(fp, "+ cksum: %u\n", ntohs(cksum));
  }
  size_t hdr_len() const { return sizeof(udp); }
};


enum {
  th_fin	 = 0x01,
  th_syn	 = 0x02,
  th_rst	 = 0x04,
  th_push = 0x08,
  th_ack	 = 0x10,
  th_urg	 = 0x20,
};


struct tcp {
  uint16_t sport;     /* TCP source port.            */
  uint16_t dport;     /* TCP destination port.       */
  uint32_t seq_num;   /* TX data sequence number.    */
  uint32_t ack_num;   /* RX data ack number.         */
  uint8_t  data_off;  /* Data offset.                */
  uint8_t  tcp_flags; /* TCP flags                   */
  uint16_t rx_win;    /* RX flow control window.     */
  uint16_t cksum;     /* TCP checksum.               */
  uint16_t tcp_urp;   /* TCP urgent pointer, if any. */

  void print(FILE* fp) const
  {
    fprintf(fp, "TCP header \n");
    fprintf(fp, "+ sport    : %u 0x%04x \n", ntohs(sport)  , ntohs(sport)  );
    fprintf(fp, "+ dport    : %u 0x%04x \n", ntohs(dport)  , ntohs(dport)  );
    fprintf(fp, "+ seq num  : %u 0x%08x \n", ntohl(seq_num), ntohl(seq_num));
    fprintf(fp, "+ ack num  : %u 0x%08x \n", ntohl(ack_num), ntohl(ack_num));
    fprintf(fp, "+ data off : 0x%02x \n", data_off                         );
    fprintf(fp, "+ tcp flags: 0x%02x \n", tcp_flags                        );
    fprintf(fp, "+ rx win   : 0x%04x \n", ntohs(rx_win)                    );
    fprintf(fp, "+ cksum    : 0x%04x \n", ntohs(cksum )                    );
  }
};


struct icmp {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;

  void print(FILE* fp) const
  {
    fprintf(fp, "ICMP header \n");
    fprintf(fp, "+ type     : %u \n", type);
    fprintf(fp, "+ code     : %u \n", code);
    fprintf(fp, "+ checksum : %u \n", ntohs(checksum));
  }
};



#pragma pack(0)


inline uint16_t
ipv4_checksum(const void* ih_ptr)
{
  const ip* ih = reinterpret_cast<const ip*>(ih_ptr);
	uint16_t cksum;
	cksum = checksum(ih, sizeof(ip));
	return (cksum == 0xffff) ? cksum : ~cksum;
}

static inline uint16_t
ipv4_phdr_cksum(const ip *ipv4_hdr)
{
	struct ipv4_psd_header {
		uint32_t src_addr; /* IP address of source host. */
		uint32_t dst_addr; /* IP address of destination host. */
		uint8_t  zero;     /* zero. */
		uint8_t  proto;    /* L4 protocol type. */
		uint16_t len;      /* L4 length. */
	} psd_hdr;

	psd_hdr.src_addr = ipv4_hdr->src;
	psd_hdr.dst_addr = ipv4_hdr->dst;
	psd_hdr.zero = 0;
	psd_hdr.proto = ipv4_hdr->proto;
  psd_hdr.len = htons(
    (uint16_t)(ntohs(ipv4_hdr->tot_len) - sizeof(ip)));
	return checksum(&psd_hdr, sizeof(psd_hdr));
}

inline uint16_t
ipv4_tcpudp_checksum(const void* ih_ptr, const void* l4_hdr)
{
  const ip* ih = reinterpret_cast<const ip*>(ih_ptr);
	uint32_t l4_len = ntohs(ih->tot_len) - sizeof(ip);

	uint32_t cksum = checksum(l4_hdr, l4_len);
	cksum += ipv4_phdr_cksum(ih);
	cksum = ((cksum & 0xffff0000) >> 16) + (cksum & 0xffff);
	cksum = (~cksum) & 0xffff;
	if (cksum == 0)
		cksum = 0xffff;
	return cksum;
}


} /* namespace slankdev */
