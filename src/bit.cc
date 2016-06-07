
#include <slankdev/bit.h>
#include <endian.h>


namespace slankdev {



#define slankdev_bswap_16(x) \
    ( (((val) >> 8) & 0x00FF) | (((val) << 8) & 0xFF00) )

#define slankdev_bswap_32(x) \
    ( (((val) >> 24) & 0x000000FF) | (((val) >>  8) & 0x0000FF00) | \
      (((val) <<  8) & 0x00FF0000) | (((val) << 24) & 0xFF000000) )


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
