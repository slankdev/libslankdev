


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
 * @file   slankdev/filefd.h
 * @brief  FILE wrapper
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>

namespace slankdev {

class filefd {
  FILE* fp;
  std::string name;

 public:
  filefd();
  ~filefd();
  FILE* getfp();
  const std::string& get_name() const;

  void fopen(const char* path, const char* mode);
  void fclose();
  void fwrite(const void* ptr, size_t size, size_t nmemb);
  char* fgets(char* s, int size);
  size_t fread(void* ptr, size_t size, size_t nmemb);
  void fflush();
  template<typename... ARG> int fprintf(const char* fmt, const ARG&... arg);
};

} /* namespace slankdev */







/*
 * Function Implementation
 */

namespace slankdev {

inline filefd::filefd() : fp(nullptr) {}
inline filefd::~filefd() { fclose(); }
inline FILE* filefd::getfp() { return fp; }
inline const std::string& filefd::get_name() const { return name; }

inline void filefd::fopen(const char* path, const char* mode)
{
  if (fp)
    fclose();

  fp = ::fopen(path, mode);
  if (!fp) {
    perror("fopen");
    exit(-1);
  }
  name = path;
}

inline void filefd::fclose()
{
  if (fp != nullptr) {
    ::fclose(fp);
    fp = nullptr;
    name = "";
  }
}
inline void filefd::fwrite(const void* ptr, size_t size, size_t nmemb)
{
  size_t res = ::fwrite(ptr, size, nmemb, fp);
  if (res != nmemb) {
    perror("fwrite");
    exit(-1);
  }
}

inline char* filefd::fgets(char* s, int size)
{
  char* res = ::fgets(s, size, fp);
  return res;
}

inline size_t filefd::fread(void* ptr, size_t size, size_t nmemb)
{
  size_t res = ::fread(ptr, size, nmemb, fp);
  if (res != nmemb) {
    if (errno == 0)
      return res;
    else
      perror("fread");
  }
  return res;
}

inline void filefd::fflush()
{
  int res = ::fflush(fp);
  if (res == EOF) {
    perror("fflush");
    exit(-1);
  }
}
template<typename... ARG>
inline int filefd::fprintf(const char* fmt, const ARG&... arg)
{ return ::fprintf(fp, fmt, arg...); }


} /* namespace slankdev */


