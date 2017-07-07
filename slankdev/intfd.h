
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
 * @file   slankdev/intfd.h
 * @brief  int file discriptor
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once


#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


namespace slankdev {


class safe_intfd {
 protected:
  int fd;
 public:
  bool noclose_in_destruct;

  safe_intfd();
  ~safe_intfd();
  void open(const char* path, int flags);
  void open(const char* path, int flags, mode_t mode);
  void close();
  int fcntl(int cmd);
  void fcntl(int cmd, long arg);
  void ioctl(unsigned long l, void* arg);
  void write(const void* buffer, size_t bufferlen);
  size_t read(void* buffer, size_t bufferlen);
  int get_fd() const;
  void set_fd(int f);

};

} /* namespace slankdev */





/*
 * Function Implementation
 */


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <slankdev/exception.h>



namespace slankdev {

inline safe_intfd::safe_intfd() : fd(-1), noclose_in_destruct(false) {}
inline safe_intfd::~safe_intfd() { if (!noclose_in_destruct) close(); }
inline int safe_intfd::get_fd() const { return fd; }
inline void safe_intfd::open(const char* path, int flags)
{
  fd = ::open(path, flags);
  if (fd < 0) {
    perror("open");
    throw slankdev::exception("open");
  }
}
inline void safe_intfd::open(const char* path, int flags, mode_t mode)
{
  fd = ::open(path, flags, mode);
  if (fd < 0) {
    perror("open");
    throw slankdev::exception("open");
  }
}
inline void safe_intfd::close()
{
  if (fd >= 0)
    ::close(fd);
}
inline int safe_intfd::fcntl(int cmd)
{
  int res = ::fcntl(fd, cmd);
  if (res < 0) {
    perror("fcntl");
    throw slankdev::exception("fcntl");
  }
  return res;
}
inline void safe_intfd::fcntl(int cmd, long arg)
{
  int res = ::fcntl(fd, cmd, arg);
  if (res < 0) {
    perror("fcntl");
    throw slankdev::exception("fcntl");
  }
}
inline void safe_intfd::ioctl(unsigned long l, void* arg)
{
  int res = ::ioctl(fd, l, arg);
  if (res < 0) {
    perror("ioctl");
    throw slankdev::exception("ioctl");
  }
}
inline void safe_intfd::write(const void* buffer, size_t bufferlen)
{
  ssize_t res = ::write(fd, buffer, bufferlen);
  if (static_cast<size_t>(res) != bufferlen) {
    if (res < 0) {
      perror("write");
      throw slankdev::exception("write");
    } else {
      fprintf(stderr, "write could not send all.\n");
    }
  }
}
inline size_t safe_intfd::read(void* buffer, size_t bufferlen)
{
  ssize_t res = ::read(fd, buffer, bufferlen);
  if (res < 0) {
    perror("read");
    throw slankdev::exception("read");
  } else if (res == EINTR) {
    return read(buffer, bufferlen);
  }
  return static_cast<size_t>(res);
}
inline void safe_intfd::set_fd(int f)
{
  if (f >= 0)
    close();
  fd = f;
}




} /* namespace slankdev */
