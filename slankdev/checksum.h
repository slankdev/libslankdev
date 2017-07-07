
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
 * @file   slankdev/checksum.h
 * @brief  checksum function
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */



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

inline uint16_t checksum(const void* data, size_t len)
{
  uint32_t sum = 0;
  const uint8_t* data_pointer = reinterpret_cast<const uint8_t*>(data);

  for (; len > 1; len-=2, data_pointer+=2) {
    uint16_t tmp = read_as_little_endian(data_pointer);
    sum += tmp;
  }

  if (len == 1) {
    fprintf(stderr, "pgen::checksum: byte length isn't even num\n");
    uint16_t tmp = read_as_little_endian(data_pointer);
    sum += tmp;
  }

  uint16_t overflowd = sum >> 16;
  sum = sum & 0x0000ffff;
  sum = sum + overflowd;

  return ~sum;
}


} /* namespace slankdev */



