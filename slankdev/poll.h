

#pragma once

#include <poll.h>
#include <slankdev/exception.h>

namespace slankdev {

inline bool poll(struct ::pollfd* fds, size_t nfds, int timeout)
{
  int res = ::poll(fds, nfds, timeout);
  if (res < 0) {
    throw slankdev::exception("poll");
  }
  return res > 0;
}

} /* namespace slankdev */
