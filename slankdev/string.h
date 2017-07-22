

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
 * @file   slankdev/filename
 * @brief  file brief
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */

#pragma once

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>
#include <stdarg.h>


/**
 * @namespace slankdev
 */
namespace slankdev {


/**
 * @brief Craft format string like a python's string.format(...)
 * @param[out]      fmt   string format
 * @param[in]       args  arguments
 * @retval          std::string crafted format string as a std::string
 */
#if 0
template <class... ARGS>
inline std::string format(const char* fmt, ARGS... args)
{
  char str[1000];
  sprintf(str, fmt, args...);
  return str;
}
#else
inline std::string format(const char* fmt, ...)
{
  char str[1000];
  va_list args;
  va_start(args, fmt);
  vsprintf(str, fmt, args);
  va_end(args);
  return str;
}
#endif


/* for compatibility */
template <class... ARGS>
static inline std::string strfmt(const char* fmt, ARGS... args)
{ return format(fmt, args...); }


/* for compatibility */
template <class... ARGS>
static inline std::string fs(const char* fmt, ARGS... args)
{ return format(fmt, args...); }


inline std::vector<std::string> split(const std::string &str, char sep)
{
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string buffer;
  while( std::getline(ss, buffer, sep) ) {
    if (buffer.empty()) continue;
    v.push_back(buffer);
  }
  return v;
}


inline std::string get_progress_str(double percent, size_t maxlen=50)
{
  std::string s;
  size_t barlen = maxlen * percent;

  s += "[";
  for (size_t i=0; i<barlen; i++) s+="=";
  if (barlen != maxlen) s+=">";
  for (size_t i=0; i<maxlen-barlen; i++) s+=" ";
  s += "]";
  s += slankdev::format("%lf%", percent*100);
  return s;
}



} /* namespace slankdev */


