

/*
 * This is Intel-82599-Datasheet's Example
 * tp.dst_addr = (0xa18e6450); // 161.142.100.80 -> 0xa18e6450
 * tp.src_addr = (0x420995bb); // 66.9.149.187   -> 0x420995bb
 * tp.dport    = (0x6e6     ); // 1766           -> 0x6e6
 * tp.sport    = (0xaea     ); // 2794           -> 0xaea
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <slankdev/hexdump.h>
#include <slankdev/socketfd.h>
#include <slankdev/packet.h>

const char* ifname = "eth2";

int main()
{
  slankdev::socketfd sock;
  sock.open_afpacket(ifname);

  slankdev::udp_packet pack;
  for (uint32_t src=0; /*src<10*/; src++) {
  for (uint32_t dst=0; /*dst<10*/; dst++) {
      pack.ih->src.set(src);
      pack.ih->dst.set(dst);

      pack.summary();
      sock.write(pack.m.mtod(), pack.m.dlen());
  }
  }
}


