
#pragma once


#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


namespace slankdev {


/* Thanks @herumi */
static uint16_t read_as_little_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
    return uint16_t(p[0]) | (uint16_t(p[1]) << 8);
}
#if 0
/* Thanks @herumi, Isn't used yet */
static uint16_t read_as_big_endian(const void* data)
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
    return uint16_t(p[1]) | (uint16_t(p[0]) << 8);
}
#endif

inline uint16_t checksum(const void* data, size_t len)
{
    uint32_t sum;
    const uint8_t* data_pointer = reinterpret_cast<const uint8_t*>(data);

    for (; len > 1; len-=2, data_pointer+=2) {
        uint16_t tmp = read_as_little_endian(data_pointer);
        sum += tmp;
    }

    if (len == 1) {
        fprintf(stderr, "pgen::checksum: byte length isn't even num\n");
        // uint8_t tmp = *data_pointer;
        uint16_t tmp = read_as_little_endian(data_pointer);
        sum += tmp;
    }

    uint16_t overflowd = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + overflowd;

    return ~sum;
}

} /* namespace slankdev */
