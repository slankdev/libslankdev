#include <linux/bpf.h>
#include <stdint.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/if_vlan.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#ifndef __section
#define __section(NAME) \
  __attribute__((section(NAME), used))
#endif

struct eh {
  uint8_t dst[6];
};

__section("prog")
int filter(struct xdp_md *ctx)
{
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;
  int rc = XDP_DROP;

  uint64_t nh_off = sizeof(*eth);
  if (data + nh_off > data_end)
    return rc;

  eth->h_dest[0] = 0x5a;
  eth->h_dest[1] = 0xa2;
  eth->h_dest[2] = 0x01;
  eth->h_dest[3] = 0x57;
  eth->h_dest[4] = 0xef;
  eth->h_dest[5] = 0x6e;

  eth->h_source[0] = 0x56;
  eth->h_source[1] = 0xd9;
  eth->h_source[2] = 0xce;
  eth->h_source[3] = 0x26;
  eth->h_source[4] = 0xfa;
  eth->h_source[5] = 0x2b;

  return XDP_PASS;
  /* return XDP_DROP; */
}
char __license[] __section("license") = "GPL";
