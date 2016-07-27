

#pragma once 

#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <slankdev/intfd.h>


namespace slankdev {

uint64_t rdtsc();

uint16_t htons(uint16_t val);
uint16_t ntohs(uint16_t val);
uint32_t htonl(uint32_t val);
uint32_t ntohl(uint32_t val);



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
