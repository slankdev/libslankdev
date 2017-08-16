

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


namespace slankdev {


inline uint16_t bswap16(uint16_t x)
{
	return (uint16_t)(((x & 0x00ffU) << 8) |
		((x & 0xff00U) >> 8));
}
inline uint32_t bswap32(uint32_t x)
{
	return  ((x & 0x000000ffUL) << 24) |
		((x & 0x0000ff00UL) << 8) |
		((x & 0x00ff0000UL) >> 8) |
		((x & 0xff000000UL) >> 24);
}
inline uint64_t bswap64(uint64_t x)
{
  uint64_t ret
    = ((0xff00000000000000 & x) >> 56)
    | ((0x00ff000000000000 & x) >> 40)
    | ((0x0000ff0000000000 & x) >> 24)
    | ((0x000000ff00000000 & x) >> 8 )
    | ((0x00000000ff000000 & x) << 8 )
    | ((0x0000000000ff0000 & x) << 24)
    | ((0x000000000000ff00 & x) << 40)
    | ((0x00000000000000ff & x) << 56);
  return ret;
}


} /* namespace slankdev */
