
#pragma once

#include <stdint.h>
#include <stddef.h>

#include <string>

#include "ncurses.h"
#include "pane.h"


class TextPane : public PaneInterface {
  Lines lines;
  size_t cursor;
  size_t start_idx;
 public:
  TextPane(size_t _x, size_t _y, size_t _w, size_t _h);
  virtual ~TextPane() {}

  template<class... ARGS> void print(const char* fmt, ARGS... args);
  virtual void refresh() override;
  virtual void key_input(char c) override;
  void cursor_down();
  void cursor_up();

 private:
  void scroll_down() { start_idx++; }
  void scroll_up()   { start_idx--; }

 /* Debug Functions */
 public:
  size_t cur() const { return cursor; }
  WINDOW* ww() { return win; }
};




/*
 * Class Member Function Implementation
 */

void TextPane::key_input(char c)
{
  if (c == 'j') {
    cursor_down();
  } else if (c == 'k') {
    cursor_up();
  }
}
TextPane::TextPane(size_t _x, size_t _y, size_t _w, size_t _h)
  : PaneInterface(_x, _y, _w, _h), cursor(0), start_idx(0) {}
template<class... ARGS>
void TextPane::print(const char* fmt, ARGS... args)
{
  std::string s = slankdev::fs(fmt, args...);
  lines.addline(s);
}
void TextPane::refresh()
{
  for (size_t i=start_idx, count=0; i<lines.size() && count<h; i++, count++) {
    if (i == cursor) {
      wattron(win, A_REVERSE);
      std::string s = lines[i];
      while (s.size() < this->w) s += ' ';
      mvwprintw(win, count, 0, "%s", s.c_str());
      wattroff(win, A_REVERSE);
    } else {
      std::string s = lines[i];
      while (s.size() < this->w) s += ' ';
      mvwprintw(win, count, 0, "%s", s.c_str());
    }
    clrtoeol();
  }
  wrefresh(win);
}
void TextPane::cursor_down()
{
  if (cursor + 1 < lines.size()) {
    if (cursor - start_idx + 2 > h) scroll_down();
    cursor++;
  }
}
void TextPane::cursor_up()
{
  if (cursor > 0) {
    if (cursor - 1 < start_idx) scroll_up();
    cursor--;
  }
}



