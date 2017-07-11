
#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


struct eth_hdr {
  uint8_t dst[6];
  uint8_t src[6];
  uint16_t type;
};

struct arp_hdr {
  uint16_t    hwtype   ;
  uint16_t    ptype    ;
  uint8_t     hwlen    ;
  uint8_t     plen     ;
  uint16_t    operation;
  uint8_t     hwsrc[6];
  uint8_t     psrc[4];
  uint8_t     hwdst[6];
  uint8_t     pdst[4];
};

inline void print_eth_hdr(const struct eth_hdr* eth)
{
  printf("dst: ");

  for (size_t i=0; i<6; i++) printf("%02x:", eth->dst[i]);
  printf("\n");
  printf("src: ");
  for (size_t i=0; i<6; i++) printf("%02x:", eth->src[i]);
  printf("\n");
  printf("type: 0x%04x\n", ntohs(eth->type));
}

inline void print_ip(const uint8_t* ip)
{
  for (size_t i=0; i<6; i++) printf("%02x:", ip[i]);
}

inline void print_mac(const uint8_t* mac)
{
  for (size_t i=0; i<6; i++) printf("%02x:", mac[i]);
}

inline void print_arp_hdr(const struct arp_hdr* ah)
{
  printf("hwtype   : %u\n", ntohs(ah->hwtype   ));
  printf("ptype    : %u\n", ntohs(ah->ptype    ));
  printf("hwlen    : %u\n", ah->hwlen    );
  printf("plen     : %u\n", ah->plen     );
  printf("operation: %u\n", ntohs(ah->operation));

  printf("hwsrc    : ");
  print_mac(ah->hwsrc);
  printf("\n");

  printf("psrc     : ");
  print_ip(ah->psrc);
  printf("\n");

  printf("hwdst    : ");
  print_mac(ah->hwdst);
  printf("\n");

  printf("pdst     : ");
  print_ip(ah->pdst);
  printf("\n");
}


