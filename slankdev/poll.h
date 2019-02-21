
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
 * @file   slankdev/poll.h
 * @brief  poll wrapper
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */



#pragma once

#include <poll.h>
#include <slankdev/exception.h>

namespace slankdev {

#if 0
inline bool poll(struct ::pollfd* fds, size_t nfds, int timeout)
{
  int res = ::poll(fds, nfds, timeout);
  if (res < 0) {
    throw slankdev::exception("poll");
  }
  return res > 0;
}
#endif

struct pollfd {
  std::vector<struct ::pollfd> fds;
  pollfd() {}
  int get_fd(size_t idx) const { return fds[idx].fd; }
  int16_t get_revents(size_t idx) const { return fds[idx].revents; }
  size_t n_fds() const { return fds.size(); }
  void append_fd(int32_t fd, int16_t events)
  {
    struct ::pollfd pd;
    memset(&pd, 0, sizeof(struct ::pollfd));
    pd.fd = fd;
    pd.events = events;
    fds.push_back(pd);
  }
  int poll(int timeout)
  {
    int ret = ::poll(fds.data(), fds.size(), timeout);
    if (ret < 0)
      throw slankdev::exception("poll");
    return ret;
  }
};

} /* namespace slankdev */


