

#include <slankdev/socketfd.h>
#include <slankdev/hexdump.h>
const char* ifname = "lo";

uint8_t packet[] = {
  /* arp packet */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01,
  0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x00,
};

int main()
{
  slankdev::socketfd sock;
  sock.open_afpacket(ifname);
  sock.write(packet, sizeof(packet));
  slankdev::hexdump(stdout, packet, sizeof(packet));
}


