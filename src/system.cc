
#include <slankdev/system.h>

namespace slankdev {

uint64_t rdtsc()
{
    unsigned int eax, edx;
    __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
    return ((uint64_t)edx << 32) | eax;
}






# if __BYTE_ORDER == __BIG_ENDIAN
// # warning "BIG ENDIAN"
# define slankdev_ntohl(x)	(x)
# define slankdev_ntohs(x)	(x)
# define slankdev_htonl(x)	(x)
# define slankdev_htons(x)	(x)
# elif __BYTE_ORDER == __LITTLE_ENDIAN
// # warning "LITTLE ENDIAN"
#  define slankdev_ntohl(x)	slankdev_bswap_32 (x)
#  define slankdev_ntohs(x)	slankdev_bswap_16 (x)
#  define slankdev_htonl(x)	slankdev_bswap_32 (x)
#  define slankdev_htons(x)	slankdev_bswap_16 (x)
# else
# error "Unknown __BYTE_ORDER"
# endif



#define slankdev_bswap_16(n) \
    (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define slankdev_bswap_32(n) \
    (((((unsigned long)(n) & 0xFF)) << 24)   | ((((unsigned long)(n) & 0xFF00)) << 8) | \
	((((unsigned long)(n) & 0xFF0000)) >> 8) | ((((unsigned long)(n) & 0xFF000000)) >> 24))





uint16_t htons(uint16_t val)
{
    return slankdev_htons(val);
}
uint16_t ntohs(uint16_t val)
{
    return slankdev_ntohs(val);
}
uint32_t htonl(uint32_t val)
{
    return slankdev_htonl(val);
}
uint32_t ntohl(uint32_t val)
{
    return slankdev_ntohl(val);
}





} /* namespace slankdev */
