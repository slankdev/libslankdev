
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
 * @file   slankdev/vty.h
 * @brief  vty
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <stdint.h>
#include <stddef.h>
#include <vector>

#include <slankdev/vty_command.h>
#include <slankdev/vty_keyfunc.h>
#include <slankdev/vty_shell.h>


namespace slankdev {


class vty {
  const uint16_t          port;
  const std::string       bootmsg;
  const std::string       prompt;
  int                     server_fd;
  std::vector<shell>      shells;
  std::vector<command*>   commands;
  std::vector<key_func*>  keyfuncs;

  int get_server_sock();
 public:
  void* user_ptr;

  vty(uint16_t p, const char* msg, const char* prmpt);
  virtual ~vty();

  void install_keyfunction(key_func* kf);
  void install_command(command* cmd);
  void add_default_keyfunctions();
  void init_default_keyfunc();
  void dispatch();
};


} /* namespace slankdev */







/*
 * vty class: Function Implementation
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <slankdev/socketfd.h>
#include <slankdev/net/telnet.h>
#include <slankdev/poll.h>

namespace slankdev {

inline vty::vty(uint16_t p, const char* msg, const char* prmpt)
  : port(p),
  bootmsg(msg),
  prompt(prmpt),
  server_fd(get_server_sock()) { add_default_keyfunctions(); }
inline vty::~vty()
{
  for (command* c : commands) delete c;
  for (key_func* f : keyfuncs) delete f;
}
inline void vty::install_keyfunction(key_func* kf) { keyfuncs.push_back(kf); }
inline void vty::install_command(command* cmd) { commands.push_back(cmd); }
inline int vty::get_server_sock()
{
  slankdev::socketfd server_sock;
  server_sock.noclose_in_destruct = true;
  server_sock.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  int option = 1;
  server_sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  server_sock.bind((sockaddr*)&addr, sizeof(addr));
  server_sock.listen(5);
  return server_sock.get_fd();
}
inline void vty::dispatch()
{
  while (true) {
    struct Pollfd : public pollfd {
      Pollfd(int ifd, short ievents)
      {
        fd = ifd;
        events = ievents;
      }
    };
    std::vector<struct Pollfd> fds;
    fds.push_back(Pollfd(server_fd, POLLIN));
    for (const shell& sh : shells) fds.emplace_back(Pollfd(sh.fd, POLLIN));

    if (slankdev::poll(fds.data(), fds.size(), 1000)) {
      if (fds[0].revents & POLLIN) {
        /*
         * Server Accept Process
         */
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int fd = accept(fds[0].fd, (sockaddr*)&client, &client_len);

        slankdev::socketfd client_sock(fd);
        uint32_t on = 1;
        client_sock.setsockopt(IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));
        int flags = client_sock.fcntl(F_GETFL);
        client_sock.fcntl(F_SETFL, (flags | O_NONBLOCK));
        client_sock.noclose_in_destruct = true;
        slankdev::vty_will_echo (fd);
        slankdev::vty_will_suppress_go_ahead (fd);
        slankdev::vty_dont_linemode (fd);
        slankdev::vty_do_window_size (fd);

        shells.push_back(
            shell(
              fd,
              bootmsg.c_str(),
              prompt.c_str(),
              &commands,
              &keyfuncs,
              user_ptr
              )
            );
      }

      /*
       * Client Read Process
       */
      for (size_t i=1; i<fds.size(); i++) {
        if (fds[i].revents & POLLIN) {
          shells[i-1].process();
          if (shells[i-1].closed) {
            close(fds[i].fd);
            shells.erase(shells.begin() + i);
            continue;
          }
        }
      }
    }
  }
}
inline void vty::add_default_keyfunctions()
{
  using namespace slankdev;

  uint8_t up   [] = {AC_ESC, '[', AC_A};
  uint8_t down [] = {AC_ESC, '[', AC_B};
  uint8_t right[] = {AC_ESC, '[', AC_C};
  uint8_t left [] = {AC_ESC, '[', AC_D};
  install_keyfunction(new KF_hist_search_deep   (up   , sizeof(up   )));
  install_keyfunction(new KF_hist_search_shallow(down , sizeof(down )));
  install_keyfunction(new KF_right(right, sizeof(right)));
  install_keyfunction(new KF_left (left , sizeof(left )));

  uint8_t ctrlP[] = {AC_Ctrl_P};
  uint8_t ctrlN[] = {AC_Ctrl_N};
  uint8_t ctrlF[] = {AC_Ctrl_F};
  uint8_t ctrlB[] = {AC_Ctrl_B};
  install_keyfunction(new KF_hist_search_deep   (ctrlP, sizeof(ctrlP)));
  install_keyfunction(new KF_hist_search_shallow(ctrlN, sizeof(ctrlN)));
  install_keyfunction(new KF_right(ctrlF, sizeof(ctrlF)));
  install_keyfunction(new KF_left (ctrlB, sizeof(ctrlB)));

  uint8_t ctrlA[] = {AC_Ctrl_A};
  uint8_t ctrlE[] = {AC_Ctrl_E};
  install_keyfunction(new KF_cursor_top(ctrlA, sizeof(ctrlA)));
  install_keyfunction(new KF_cursor_end(ctrlE, sizeof(ctrlE)));

  uint8_t question[] = {'?'};
  install_keyfunction(new KF_help(question, sizeof(question)));

  uint8_t tab[] = {'\t'};
  install_keyfunction(new KF_completion(tab, sizeof(tab)));

  uint8_t ret[] = {'\r', '\0'};
  install_keyfunction(new KF_return  (ret, sizeof(ret)));

  uint8_t backspace[] = {0x7f};
  install_keyfunction(new KF_backspace  (backspace, sizeof(backspace)));
}



} /* namespace slankdev */



