

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <slankdev/hexdump.h>
#include <slankdev/util.h>
#include <slankdev/net/mbuf.h>
#include <slankdev/net/hdr.h>


class packet {
 public:
  slankdev::mbuf   m;
  slankdev::udp*   uh;
  slankdev::ip*    ih;
  slankdev::ether* eh;
  packet()
  {
    const char* str = "slankdev";

    m.prepend(strlen(str));
    uint8_t* d = m.mtod();
    memcpy(d, str, strlen(str));

    m.prepend(sizeof(slankdev::udp));
    uh = reinterpret_cast<slankdev::udp*>(m.mtod());
    uh->src = htons(80);
    uh->dst = htons(53);
    uh->len = m.dlen();
    uh->cksum = htons(0x0000);

    m.prepend(sizeof(slankdev::ip));
    ih = reinterpret_cast<slankdev::ip*>(m.mtod());
    ih->ver   = 4;
    ih->ihl   = 5;
    ih->len   = htons(m.dlen());
    ih->proto = 0x11;
    ih->sum   = htons(0x0000);
    ih->src.s_addr = htonl(0xc0a8000a);
    ih->dst.s_addr = htonl(0xc0a80001);

    m.prepend(sizeof(slankdev::ether));
    eh = reinterpret_cast<slankdev::ether*>(m.mtod());
    eh->type = htons(0x0800);

  }
  void show() const
  {
    m.dump(stdout);
    printf("\n");
    eh->print();
    ih->print();
    uh->print();
  }
};


int main()
{
  packet pack;
  pack.show();
}
