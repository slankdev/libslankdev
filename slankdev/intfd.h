
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
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <slankdev/exception.h>
#include <sys/mman.h>


namespace slankdev {

/*
 * This class is not close fd in Destructor
 */
class intfd final {
  int fd;
  bool close_in_destructor;
 public:

  intfd() : fd(-1), close_in_destructor(true) {}
  intfd(const char* path, int flags) : intfd() { this->open(path, flags); }

  virtual ~intfd()
  {
    if (close_in_destructor) close();
  }

  void set_close_in_destructor(bool b) { close_in_destructor = b; }

  void open(const char* path, int flags)
  {
    fd = ::open(path, flags);
    if (fd < 0) {
      perror("open");
      throw slankdev::exception("open");
    }
  }

  void close()
  {
    if (fd >= 0)
      ::close(fd);
  }

  void write(const void* buffer, size_t bufferlen)
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

  size_t read(void* buffer, size_t bufferlen)
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

  void pwrite(const void* buffer, size_t bufferlen, size_t offset)
  {
    ssize_t res = ::pwrite(fd, buffer, bufferlen, offset);
    if (static_cast<size_t>(res) != bufferlen) {
      if (res < 0) {
        perror("pwrite");
        throw slankdev::exception("pwrite");
      } else {
        fprintf(stderr, "pwrite could not send all.\n");
      }
    }
  }

  size_t pread(void* buffer, size_t bufferlen, size_t offset)
  {
    ssize_t res = ::pread(fd, buffer, bufferlen, offset);
    if (res < 0) {
      perror("pread");
      throw slankdev::exception("pread");
    }
    return static_cast<size_t>(res);
  }

  int get_fd() const { return fd; }

  void set_fd(int fd_)
  {
    if (fd >= 0)
      close();
    fd = fd_;
  }

  void *mmap(void *start, size_t length, int prot, int flags, off_t offset)
  {
    void* ret = ::mmap(start, length, prot, flags, fd, offset);
    return ret;
  }

}; /* class intfd */



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
  void pwrite(const void* buffer, size_t bufferlen, size_t offset);
  size_t read(void* buffer, size_t bufferlen);
  size_t pread(void* buffer, size_t bufferlen, size_t offset);
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
inline void safe_intfd::pwrite(const void* buffer, size_t bufferlen, size_t offset)
{
  ssize_t res = ::pwrite(fd, buffer, bufferlen, offset);
  if (static_cast<size_t>(res) != bufferlen) {
    if (res < 0) {
      perror("pwrite");
      throw slankdev::exception("pwrite");
    } else {
      fprintf(stderr, "pwrite could not send all.\n");
    }
  }
}
inline size_t safe_intfd::pread(void* buffer, size_t bufferlen, size_t offset)
{
  ssize_t res = ::pread(fd, buffer, bufferlen, offset);
  if (res < 0) {
    perror("pread");
    throw slankdev::exception("pread");
  } else if (res == EINTR) {
    return pread(buffer, bufferlen, offset);
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
