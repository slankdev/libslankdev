

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


inline uint16_t bswap16(uint16_t n)
{
  return ((n << 8)&0xff00) | ((n>>8)&0x00ff);
}

inline uint32_t bswap32(uint32_t n)
{
  return ((n << 24)&0xff000000) | ((n<<8)&0x00ff0000)
    | ((n>>8)&0x0000ff00) | ((n>>24)&0x000000ff);
}


} /* namespace slankdev */
