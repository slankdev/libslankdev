
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
 * @file   slankdev/net/telnet.h
 * @brief  telnet operation
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/telnet.h>


namespace slankdev {

namespace {
  void safe_write(int fd, const void* buf, size_t nbuf)
  {
    int ret = write(fd, buf, nbuf);
    if (ret < 0) {
      throw slankdev::exception("write");
    }
  }
} /* namespace */

/* Send WILL TELOPT_ECHO to remote server. */
inline void vty_will_echo (int fd)
{
  unsigned char cmd[] = { IAC, WILL, TELOPT_ECHO, '\0' };
  safe_write(fd, cmd, sizeof(cmd));
}

/* Make suppress Go-Ahead telnet option. */
inline void vty_will_suppress_go_ahead (int fd)
{
  unsigned char cmd[] = { IAC, WILL, TELOPT_SGA, '\0' };
  safe_write(fd, cmd, sizeof(cmd));
}

/* Make don't use linemode over telnet. */
inline void vty_dont_linemode (int fd)
{
  unsigned char cmd[] = { IAC, DONT, TELOPT_LINEMODE, '\0' };
  safe_write(fd, cmd, sizeof(cmd));
}

/* Use window size. */
inline void vty_do_window_size (int fd)
{
  unsigned char cmd[] = { IAC, DO, TELOPT_NAWS, '\0' };
  safe_write(fd, cmd, sizeof(cmd));
}


} /* namespace slankdev */

