

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>

#include "ncurses.h"
#include "pane.h"


class ToggleList {
  std::string str;
 public:
  Lines lines;
  static bool is_close;
  static void toggle();
  ToggleList(std::string s);
  std::string to_str() const;
};
bool ToggleList::is_close = true;



class ToggleListPane : public PaneInterface {
  std::vector<ToggleList>* lines;
  size_t cursor;
  size_t start_idx;

  void scroll_down() { start_idx++; }
  void scroll_up()   { start_idx--; }
 public:
  ToggleListPane(size_t _x, size_t _y, size_t _w, size_t _h);
  virtual ~ToggleListPane() {}
  void set_content(std::vector<ToggleList>* l) { lines = l; }

  size_t cur() { return cursor; }
  void cursor_down();
  void cursor_up();
  virtual void refresh() override;
  virtual void key_input(char c) override;
};



/*
 * Function Implementations
 */

void ToggleListPane::key_input(char c)
{
  if (c == 'j') {
    cursor_down();
  } else if (c == 'k') {
    cursor_up();
  } else if (c == ' ') {
    ToggleList::toggle();
  }
}

void ToggleList::toggle()
{
  if (is_close) is_close = false;
  else          is_close = true;
}

ToggleList::ToggleList(std::string s) : str(s)
{
  lines.addline("child1");
  lines.addline("child2");
}
std::string ToggleList::to_str() const
{
  std::string s = str;
  s += is_close?" close":" opne";
  return s;
}

ToggleListPane::ToggleListPane(size_t _x, size_t _y, size_t _w, size_t _h)
  : PaneInterface(_x, _y, _w, _h), lines(nullptr), cursor(0), start_idx(0) {}

void ToggleListPane::refresh()
{
  if (lines == nullptr) return ;

  for (size_t i=start_idx, count=0; i<lines->size() && count<h; i++, count++) {
    if (i == cursor) wattron(win, A_REVERSE);

    std::string s = lines->at(i).to_str();
    while (s.size() < this->w) s += ' ';
    mvwprintw(win, count, 0, "%s", s.c_str());
    clrtoeol();

    if (i == cursor) wattroff(win, A_REVERSE);

    if (lines->at(i).is_close == false) {
      for (size_t j=0; j<lines->at(i).lines.size(); j++) {
        count++;
        std::string s = lines->at(i).lines[j];
        while (s.size() < this->w) s += ' ';
        mvwprintw(win, count, 0, "  %s", s.c_str());
      }
    }
  }
  wrefresh(win);
}
void ToggleListPane::cursor_down()
{
  if (cursor + 1 < lines->size()) {
    if (cursor - start_idx + 2 > h) scroll_down();
    cursor++;
  }
}
void ToggleListPane::cursor_up()
{
  if (cursor > 0) {
    if (cursor - 1 < start_idx) scroll_up();
    cursor--;
  }
}



