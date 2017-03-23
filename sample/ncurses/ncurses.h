
#pragma once

#include <ncurses.h>
#include <slankdev/exception.h>
#include "ncurses.h"


namespace slankdev {

inline WINDOW* newwin(size_t nlines, size_t ncols, size_t begin_y, size_t begin_x)
{
  WINDOW* win = ::newwin(nlines, ncols, begin_y, begin_x);
  if (win == NULL) {
    throw slankdev::exception("newwin");
  }
  return win;
}

inline WINDOW* subwin(WINDOW* win, size_t nlines, size_t ncols, size_t begin_y, size_t begin_x)
{
  WINDOW* sub = ::subwin(win, nlines, ncols, begin_y, begin_x);
  if (sub == NULL) {
    throw slankdev::exception("subwin");
  }
  return sub;
}



} // namespace



