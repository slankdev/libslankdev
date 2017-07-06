

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
#include <vector>

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


inline void clear_screen()
{
  printf("\033[2J\n");
}


} /* namespace slankdev */


