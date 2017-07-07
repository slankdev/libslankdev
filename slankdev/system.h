
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
 * @file   slankdev/system.h
 * @brief  system operations
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */



#pragma once

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <slankdev/exception.h>


namespace slankdev {


inline uint64_t rdtscp(uint32_t* cpuid)
{
  unsigned int eax, edx;
  __asm__ volatile("rdtscp"
      : "=a"(eax), "=d"(edx)
      :: "%rcx");
  __asm__ volatile(
      "movl %%ecx, %0;"
      :"=r"(*cpuid));
  return ((uint64_t)edx << 32) | eax;
}


inline uint64_t rdtsc()
{
  unsigned int eax, edx;
  __asm__ volatile("rdtsc" : "=a"(eax), "=d"(edx));
  return ((uint64_t)edx << 32) | eax;
}

inline void delay_clk(size_t num_clks)
{
  uint64_t begin = rdtsc();
  while (rdtsc() - begin < num_clks) ;
}

#ifdef __linux__
inline void daemon()
{
  if (::daemon(0, 0) != 0) {
    throw slankdev::exception("daemon");
  }
}
#endif


} /* namespace slankdev */
