
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>

#include "TextPane.h"

class PacketList {
  std::string str;
 public:
  std::vector<ToggleList>  details;
  std::vector<std::string> binarys;
  PacketList(const std::string s);
  std::string to_str() const;
};


class PacketListPane : public PaneInterface {
  size_t cursor;
  size_t start_idx;
 public:
  std::vector<PacketList*> packets;
  PacketListPane(size_t _x, size_t _y, size_t _w, size_t _h);
  void add_packet(const std::string s)
  {
    packets.push_back(new PacketList(s));
  }

 public:
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
PacketList::PacketList(const std::string s) : str(s)
{

  /*
   * Test Init Details
   */
  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "%s: slankdiafadfdpdk %zd tetste ", this->str.c_str(), pad_y);
    details.push_back(ToggleList(str));
  }

  /*
   * Test Init Binary
   */
  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "%s: sudo apt install dfadf %zd tetste ", this->str.c_str(), pad_y);
    binarys.push_back(str);
  }
}
std::string PacketList::to_str() const
{
  return str;
}


void PacketListPane::key_input(char c)
{
  if (c == 'j') {
    cursor_down();
  } else if (c == 'k') {
    cursor_up();
  }
}

PacketListPane::PacketListPane(size_t _x, size_t _y, size_t _w, size_t _h)
  : PaneInterface(_x, _y, _w, _h), cursor(0), start_idx(0) {}


void PacketListPane::refresh()
{
  for (size_t i=start_idx, count=0; i<packets.size() && count<h; i++, count++) {
    if (i == cursor) wattron(win, A_REVERSE);

    std::string s = packets[i]->to_str();
    while (s.size() < this->w) s += ' ';
    mvwprintw(win, count, 0, "%s", s.c_str());

    if (i == cursor) wattroff(win, A_REVERSE);
    clrtoeol();
  }
  wrefresh(win);
}
void PacketListPane::cursor_down()
{
  if (cursor + 1 < packets.size()) {
    if (cursor - start_idx + 2 > h) scroll_down();
    cursor++;
  }
}
void PacketListPane::cursor_up()
{
  if (cursor > 0) {
    if (cursor - 1 < start_idx) scroll_up();
    cursor--;
  }
}


