

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


namespace slankdev {


/**
 * @brief Craft format string like a python's string.format(...)
 * @param[out]      fmt   string format
 * @param[in]       args  arguments
 * @retval          std::string crafted format string as a std::string
 */
template <class... ARGS>
static inline std::string format(const char* fmt, ARGS... args)
{
  char str[1000];
  sprintf(str, fmt, args...);
  return str;
}


/* for compatibility */
template <class... ARGS>
static inline std::string strfmt(const char* fmt, ARGS... args)
{ return format(fmt, args...); }


/* for compatibility */
template <class... ARGS>
static inline std::string fs(const char* fmt, ARGS... args)
{ return format(fmt, args...); }


static inline std::vector<std::string> split(const std::string &str, char sep)
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




} /* namespace slankdev */
