

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
#include <slankdev/net/pkt.h>
#include <pgen/io.h>

const char* ifname1 = "eth1";
const char* ifname2 = "eth2";

int main()
{
  // slankdev::socketfd sock1;
  // slankdev::socketfd sock2;
  // sock1.open_afpacket(ifname1);
  // sock2.open_afpacket(ifname2);
  pgen::pcap_stream s("in.pcap", pgen::open_mode::pcap_write);

  slankdev::udp_packet pack;
  pack.m.append(64-pack.m.dlen());

  for (uint32_t src=0; src<10; src++) {
  for (uint32_t dst=0; dst<10; dst++) {
      pack.ih->src.set(src);
      pack.ih->dst.set(dst);

      pack.summary();
      s.send(pack.m.mtod(), pack.m.dlen());
      // sock1.write(pack.m.mtod(), pack.m.dlen());
      // sock2.write(pack.m.mtod(), pack.m.dlen());
  }
  }
}


