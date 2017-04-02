
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define UNUSED(x) (void)(x)

namespace slankdev {


inline void hexdump(FILE* fp, const void *buffer, size_t bufferlen)
{
    const uint8_t *data = reinterpret_cast<const uint8_t*>(buffer);
    size_t row = 0;
    while (bufferlen > 0) {
        fprintf(fp, "%04zx:   ", row);

        size_t n;
        if (bufferlen < 16) n = bufferlen;
        else                n = 16;

        for (size_t i = 0; i < n; i++) {
            if (i == 8) fprintf(fp, " ");
            fprintf(fp, " %02x", data[i]);
        }
        for (size_t i = n; i < 16; i++) {
            fprintf(fp, "   ");
        }
        fprintf(fp, "   ");
        for (size_t i = 0; i < n; i++) {
            if (i == 8) fprintf(fp, "  ");
            uint8_t c = data[i];
            if (!(0x20 <= c && c <= 0x7e)) c = '.';
            fprintf(fp, "%c", c);
        }
        fprintf(fp, "\n");
        bufferlen -= n;
        data += n;
        row  += n;
    }
}

// for compatibility
inline void hexdump(const char* tittle, const void *buffer, size_t bufferlen)
{
  UNUSED(tittle);
  hexdump(stdout, buffer, bufferlen);
}


inline void bindump(FILE* fp, const void* buffer, size_t bufferlen)
{
    const uint8_t* p0 = reinterpret_cast<const uint8_t*>(buffer);
    char buf[] = "00000000 ";
    for (size_t i=0; i<bufferlen; i++) {
        uint8_t c = p0[i];
        for (size_t j = 0; j < 8; j++) {
            buf[j] = '0' + (c & 1);
            c >>= 1;
        }
        fprintf(fp, "%s", buf);
        if ((i+1) % 8 == 0) fprintf(fp, "\n");
    }
}


} /* namespace slankdev */

