

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <slankdev/intfd.h>

#define slankdev_bswap_16(n) \
    (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define slankdev_bswap_32(n) \
    (((((unsigned long)(n) & 0xFF)) << 24)   | ((((unsigned long)(n) & 0xFF00)) << 8) | \
	((((unsigned long)(n) & 0xFF0000)) >> 8) | ((((unsigned long)(n) & 0xFF000000)) >> 24))


#ifdef __linux
# if __BYTE_ORDER == __BIG_ENDIAN
// # warning "BIG ENDIAN"
#  define slankdev_ntohl(x)	x
#  define slankdev_ntohs(x)	x
#  define slankdev_htonl(x)	x
#  define slankdev_htons(x)	x
# elif __BYTE_ORDER == __LITTLE_ENDIAN
// # warning "LITTLE ENDIAN"
#  define slankdev_ntohl(x)	slankdev_bswap_32 (x)
#  define slankdev_ntohs(x)	slankdev_bswap_16 (x)
#  define slankdev_htonl(x)	slankdev_bswap_32 (x)
#  define slankdev_htons(x)	slankdev_bswap_16 (x)
# else
# error "Unknown __BYTE_ORDER"
# endif
#endif

namespace slankdev {

uint64_t rdtsc()
{
    unsigned int eax, edx;
    __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
    return ((uint64_t)edx << 32) | eax;
}

#ifdef __linux
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
#endif


/*
 * This class manages Battery state.
 * However, not implemented yet.
 */
class battery {
    private:
        std::vector<safe_intfd> bats_full;
        std::vector<safe_intfd> bats_now;

    public:
        battery();
        void update_state();
        int percent();
};


// Sample Code that Gets Battery State
// #include <stdio.h>
// #include <string.h>
// #include <slankdev.h>
// #define BAT1_FULL "/sys/class/power_supply/BAT1/energy_full"
// #define BAT1_NOW  "/sys/class/power_supply/BAT1/energy_now"
// #define BAT0_FULL "/sys/class/power_supply/BAT0/energy_full"
// #define BAT0_NOW  "/sys/class/power_supply/BAT0/energy_now"
// static const char* getstr(const char* path)
// {
//     slankdev::safe_intfd fd;
//     fd.open(path, O_RDONLY);
//     static char str[256];
//     memset(str, 0, sizeof(str));
//     ssize_t readlen = fd.read(str, sizeof(str));
//     return str;
// }
// void print(const char* fullpath, const char* nowpath)
// {
//     int full = atoi(getstr(fullpath));
//     int now  = atoi(getstr(nowpath));
//     double per  = (double)(now) / full * 100;
//     printf("full: %d\n", full);
//     printf("now : %d\n", now );
//     printf("per : %d\n", (int)per );
// }
// int main()
// {
//     print(BAT1_FULL, BAT1_NOW);
//     print(BAT0_FULL, BAT0_NOW);
// }




} /* namespace slankdev */
