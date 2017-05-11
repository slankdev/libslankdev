

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
 * @file   slankdev/endian.h
 * @brief  endian operation
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdint.h>
#include <arpa/inet.h>


namespace slankdev {


inline uint32_t hton(uint32_t n) { return ::htonl(n); }
inline uint32_t ntoh(uint32_t n) { return ::ntohl(n); }
inline uint16_t hton(uint16_t n) { return ::htons(n); }
inline uint16_t ntoh(uint16_t n) { return ::ntohs(n); }


inline uint16_t bswap16(uint16_t num)
{
  uint16_t ret
    = (0xff00&num)>>8
    | (0x00ff&num)<<8;
  return ret;
}
inline uint32_t bswap32(uint32_t num)
{
  uint32_t ret
    = (0xff000000&num)>>24
    | (0x00ff0000&num)>>8
    | (0x0000ff00&num)<<8
    | (0x000000ff&num)<<24;
  return ret;
}
inline uint64_t bswap64(uint64_t num)
{
  uint64_t ret
    = (0xff00000000000000&num) >> 56
    | (0x00ff000000000000&num) >> 40
    | (0x0000ff0000000000&num) >> 24
    | (0x000000ff00000000&num) >> 8
    | (0x00000000ff000000&num) << 8
    | (0x0000000000ff0000&num) << 24
    | (0x000000000000ff00&num) << 40
    | (0x00000000000000ff&num) << 56;
  return ret;
}


} /* namespace slankdev */
