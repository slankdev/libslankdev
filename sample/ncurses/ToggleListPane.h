

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



class ToggleListPane : public PaneInterface {
  std::vector<ToggleList> lines;
  size_t cursor;
  size_t start_idx;
 public:
  ToggleListPane(size_t _x, size_t _y, size_t _w, size_t _h);
  virtual ~ToggleListPane() {}

  template <class... ARGS>
  void add_element(const std::string s) { lines.push_back(ToggleList(s)); }
  size_t cur() { return 0; }
  virtual void refresh() override;
  void cursor_down();
  void cursor_up();

 private:
  void scroll_down() { start_idx++; }
  void scroll_up()   { start_idx--; }
};





/*
 * ToggleList class Function
 */
bool ToggleList::is_close = true;
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




/*
 * ToggleListPane class Function
 */
ToggleListPane::ToggleListPane(size_t _x, size_t _y, size_t _w, size_t _h)
  : PaneInterface(_x, _y, _w, _h), cursor(0), start_idx(0) {}
void ToggleListPane::refresh()
{
  for (size_t i=start_idx, count=0; i<lines.size() && count<h; i++, count++) {
    if (i == cursor) wattron(win, A_REVERSE);

    std::string s = lines[i].to_str();
    while (s.size() < this->w) s += ' ';
    mvwprintw(win, count, 0, "%s", s.c_str());
    clrtoeol();

    if (i == cursor) wattroff(win, A_REVERSE);

    if (lines[i].is_close == false) {
      for (size_t j=0; j<lines[i].lines.size(); j++) {
        count++;
        std::string s = lines[i].lines[j];
        while (s.size() < this->w) s += ' ';
        mvwprintw(win, count, 0, "  %s", s.c_str());
      }
    }
  }
  wrefresh(win);
}
void ToggleListPane::cursor_down()
{
  if (cursor + 1 < lines.size()) {
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
