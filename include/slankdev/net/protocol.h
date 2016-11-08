
#pragma once

namespace slankdev {


struct in_addr {
    uint32_t s_addr;
};


struct ether {
    uint8_t dst[6];
    uint8_t src[6];
    uint16_t type;
};


struct ip {
#if __BYTE_ORDER == __LITTLE_ENDIAN   /* Intel */
    unsigned char    ihl:4;           /* header length */
    unsigned char    version:4;       /* version */
#elif __BYTE_ORDER == __BIG_ENDIAN      /* Motorola */
    unsigned int     version:4;       /* version */
    unsigned int     ihl:4;           /* header length */
#else
# error "fix endian!!"
#endif
    unsigned char    tos;             /* type of service */
    unsigned short   len;             /* total length */
    unsigned short   id;              /* identification */
    unsigned short   off;             /* fragment offset field */
    unsigned char    ttl;             /* time to live (TTL) */
    unsigned char    protocol;        /* protocol */
    unsigned short   sum;             /* header checksum */
    struct in_addr   src;             /* source ip address */
    struct in_addr   dst;             /* destination ip address */
};


struct udp {
    uint16_t src;
    uint16_t dst;
    uint16_t len;
    uint16_t checksum;
};



struct arp {
    uint16_t       hwtype;
    uint16_t       ptype;
    uint8_t        hwlen;
    uint8_t        plen;
    uint16_t       operation;
    uint8_t        hwsrc[6];
    struct in_addr psrc[4];
    uint8_t        hwdst[6];
    struct in_addr pdst[4];
};




    
} /* namespace slankdev */
