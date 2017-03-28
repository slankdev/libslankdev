
#pragma once

#include <stdint.h>
#include <arpa/inet.h>


namespace slankde {


inline uint32_t hton(uint32_t n) { return ::htonl(n); }
inline uint32_t ntoh(uint32_t n) { return ::ntohl(n); }
inline uint16_t hton(uint16_t n) { return ::htons(n); }
inline uint16_t ntoh(uint16_t n) { return ::ntohs(n); }


inline uint16_t bswap16(uint16_t n)
{
    return ((n << 8)&0xff00) | ((n>>8)&0x00ff);
}

inline uint32_t bswap32(uint32_t n)
{
    return ((n << 24)&0xff000000) | ((n<<8)&0x00ff0000)
        | ((n>>8)&0x0000ff00) | ((n>>24)&0x000000ff);
}


} /* namespace slankdev */
