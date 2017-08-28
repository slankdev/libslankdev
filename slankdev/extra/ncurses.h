
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
 * @file   slankdev/ncurses.h
 * @brief  ncurses5 wrapper
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <ncurses.h>
#include <slankdev/string.h>
#include <slankdev/exception.h>


namespace slankdev {

inline WINDOW* newwin(size_t nlines, size_t ncols, size_t begin_y, size_t begin_x)
{
  WINDOW* win = ::newwin(nlines, ncols, begin_y, begin_x);
  if (win == NULL) {
    std::string errstr = slankdev::fs(
                      "newwin(nlines=%zd,ncols=%zd,y=%zd,x=%zd)",
                                nlines, ncols, begin_y, begin_x);
    throw slankdev::exception(errstr.c_str());
  }
  return win;
}

inline WINDOW* subwin(WINDOW* win, size_t nlines, size_t ncols, size_t begin_y, size_t begin_x)
{
  WINDOW* sub = ::subwin(win, nlines, ncols, begin_y, begin_x);
  if (sub == NULL) {
    std::string errstr = slankdev::fs(
                      "subwin(nlines=%zd,ncols=%zd,y=%zd,x=%zd)",
                                nlines, ncols, begin_y, begin_x);
    throw slankdev::exception(errstr.c_str());
  }
  return sub;
}



} // namespace



