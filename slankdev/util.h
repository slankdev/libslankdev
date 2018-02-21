

/*
 * MIT License
 *
 * Copyright (c) 2018 Hiroki SHIROKURA
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
 * @file   slankdev/util.h
 * @brief  util function
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */



#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <vector>
#include <array>

#define UNUSED(x) (void)(x)

namespace slankdev {


template <class T>
inline void vec_delete_all_ptr_elements(std::vector<T*>& vec)
{
  while (vec.size() > 0) {
    T* tmp = vec.back();
    vec.pop_back();
    delete tmp;
  }
}

#if 0
template <class... ARGS>
inline void fdprintf(int fd, const char* fmt, ARGS... args)
{
  FILE* fp = fdopen(fd, "w");
  ::fprintf(fp, fmt, args...);
  fflush(fp);
}
#else
inline void fdprintf(int fd, const char* fmt, ...)
{
  FILE* fp = fdopen(fd, "w");
  va_list args;
  va_start(args, fmt);
  vfprintf(fp, fmt, args);
  va_end(args);
  fflush(fp);
}
#endif

class depth_fprintf_pusher {
  static size_t depth_cnt_;
  static std::string   str_;
 public:
  depth_fprintf_pusher() { depth_cnt_++; }
  ~depth_fprintf_pusher() { depth_cnt_--; }
  static size_t cnt() { return depth_cnt_; }
  static std::string str() { return str_; }
  static void set_str(const char* s)
  {
    if (depth_cnt_ != 0) {
      fprintf(stderr, "maybe un expected call? id=395581\n");
    }
    str_ = s;
  }

}; /* class depth_fprintf_pusher */

size_t depth_fprintf_pusher::depth_cnt_ = 0;
std::string depth_fprintf_pusher::str_ = "  ";

inline void depth_fprintf(FILE* fp, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  for (size_t i=0; i<depth_fprintf_pusher::cnt(); i++)
    fprintf(fp, "%s", depth_fprintf_pusher::str().c_str());
  vfprintf(fp, fmt, args);
  va_end(args);
}


inline void clear_screen()
{
  printf("\033[2J\n");
}


inline size_t popcnt32(uint32_t n)
{
  size_t cnt = 0;
  for (size_t i=0; i<32; i++) {
    if ((0x01&n) != 0) cnt ++;
    n >>= 1;
  }
  return cnt;
}


} /* namespace slankdev */


