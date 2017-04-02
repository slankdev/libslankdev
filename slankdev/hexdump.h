
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/hexdump.h
 * @brief  hex dump
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


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

