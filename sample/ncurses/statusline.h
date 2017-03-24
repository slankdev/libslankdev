
#pragma once

#include <stdarg.h>
#include <slankdev/string.h>
#include <slankdev/ncurses.h>
#include "ncurses.h"
#include <string>
#include <vector>


class Statusline {
  WINDOW* win;
  const size_t x;
  const size_t y;
  const size_t w;
  std::string str;
 public:
  Statusline(size_t _x, size_t _y, size_t _w)
    : x(_x)
    , y(_y)
    , w(_w) {}
  void init(WINDOW* rw) { win = slankdev::subwin(rw, 1, w, y, x); }
  template<class... ARGS>
  void print(const char* fmt, ARGS... args)
  {
    str = slankdev::fs(fmt, args...);
  }
  void refresh()
  {
    static size_t cnt = 0;
    mvwprintw(win, 0, 0, "%zd: %s", cnt, str.c_str());
    cnt ++;
    wrefresh(win);
  }

};


