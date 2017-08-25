
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
 * @file   slankdev/log
 * @brief  log
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <slankdev/filefd.h>
#include <slankdev/exception.h>



namespace slankdev {



template <class Tag>
class log {
  log(const log&) = delete;
  log& operator=(const log&) = delete;

 private:
  static bool inited;
  slankdev::filefd fd;

  log() {}
  ~log() {}

 public:
  static log& instance()
  {
    static log l;
    return l;
  }
  void open(const char* path)
  {
    if (inited)
      throw slankdev::exception("Inited yet");
    inited = true;
    fd.fopen(path, "a");
  }
  void open_new(const char* path)
  {
    if (inited)
      throw slankdev::exception("Inited yet");
    inited = true;
    fd.fopen(path, "w");
  }
  template<typename ... ARG>
    void write(const char* fmt, const ARG&... arg)
    {
      if (!inited)
        throw slankdev::exception("Not inited yes");

      fd.fprintf(" ");
      fd.fprintf(fmt, arg...);
      fd.fprintf("\n");
      fd.fflush();
    }
  void export_to_file(const char* output_path)
  {
    if (!inited)
      throw slankdev::exception("Not inited yes");

    fd.fflush();
    slankdev::filefd read_fd;
    slankdev::filefd write_fd;
    read_fd.fopen(fd.get_name().c_str(), "r");
    write_fd.fopen(output_path, "w");

    char c;
    while (read_fd.fread(&c, sizeof(c), 1)!=0) {
      write_fd.fwrite(&c, sizeof(c), 1);
    }
  }
};
template <class Tag>
bool log<Tag>::inited = false;



} /* namespace slankdev */
// tsurai..

