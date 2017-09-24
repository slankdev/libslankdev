

#include <slankdev/net/hdr.h>
#include <slankdev/socketfd.h>
#include <slankdev/hexdump.h>
using namespace slankdev;


int main(int argc, char** argv)
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s ifname \n", argv[0]);
    return -1;
  }

  uint8_t pkt_ptr[1000] = {0x00};
  size_t  pkt_len = 0;
  const char* str = "slankdev";

  ether* eh = (ether*)pkt_ptr;
  eh->type = bswap16(0x0800);
  pkt_len += sizeof(ether);

  ip* ih = (ip*)(eh + 1);
  ih->ver_ihl = 0x45;
  ih->tos     = 0x00;
  ih->tot_len = bswap16(sizeof(ip)+strlen(str));
  ih->id      = bswap16(0x0000);
  ih->off     = bswap16(0x0000);
  ih->ttl     = 0x40;
  ih->proto   = 0x11;
  ih->sum     = bswap16(0x0000);
  ih->src     = bswap32(0xc0a80001);
  ih->dst     = bswap32(0xc0a80304);
  pkt_len += sizeof(ip);

  udp* uh = (udp*)(ih + 1);
  uh->sport = bswap16(0xeeee);
  uh->dport = bswap16(0xdddd);
  uh->len   = bswap16(strlen(str));
  uh->cksum = bswap16(0x0000);

  pkt_len += sizeof(udp);

  uint8_t* dp = (uint8_t*)(uh + 1);
  memcpy(dp, str, strlen(str));
  pkt_len += strlen(str);

  eh->print(stdout);
  ih->print(stdout);
  uh->print(stdout);
  slankdev::socketfd sock;
  sock.open_afpacket(argv[1]);
  // for (size_t i=0; i<10; i++) sock.write(pkt_ptr, pkt_len);
  sock.write(pkt_ptr, pkt_len);
  slankdev::hexdump(stdout, pkt_ptr, pkt_len);
}


