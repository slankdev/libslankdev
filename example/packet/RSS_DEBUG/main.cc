

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <slankdev/socketfd.h>
#include <slankdev/util.h>
#include <slankdev/net_header.h>
#include <slankdev/endian.h>
const char* ifname = "enp1s0f0";


int main()
{
  using namespace slankdev;

  slankdev::socketfd sock;
  sock.open_afpacket(ifname);
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

  for (size_t i=0; i<=32; i++) {
    uh->src = htons(i);

		uint32_t addr = 0xc0a80001;
		for (size_t c=0; c<255; c++) {
			ih->dst.s_addr = htonl(addr + c);
			sock.write(buf.data(), buf.size());
		}
  }
}


