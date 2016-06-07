

#pragma once 

#include <stdio.h>
#include <stdint.h>


namespace slankdev {

uint64_t rdtsc();

uint16_t htons(uint16_t val);
uint16_t ntohs(uint16_t val);
uint32_t htonl(uint32_t val);
uint32_t ntohl(uint32_t val);

} /* namespace slankdev */
