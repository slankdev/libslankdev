
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>

#include "TextPane.h"

class Packet {
  uint8_t buf[5000];
  size_t  len;

  std::string str;
 public:
  std::vector<ToggleList_Element>  details;
  std::vector<std::string> binarys;

  Packet(const std::string s);
  Packet(const void* p, size_t l);
  std::string to_str() const;
};
size_t cnt=0;
Packet::Packet(const void* p, size_t l) : Packet("get by binary" + std::to_string(cnt++))
{
  /*
   * Init Detail Pane Information
   */



  /*
   * Init Binary Pane Information
   */
  binarys.clear();
  using namespace slankdev;

  const void* buffer = p;
  ssize_t bufferlen  = l;

  const uint8_t *data = reinterpret_cast<const uint8_t*>(buffer);
  size_t row = 0;

  std::string line;
  while (bufferlen > 0) {
    line.clear();

    line += fs(" %04zx   ", row);

    size_t n = std::min(bufferlen, ssize_t(16));

    for (size_t i = 0; i < n; i++) {
      if (i == 8) { line += " "; }
      line += fs(" %02x", data[i]);
    }
    for (size_t i = n; i < 16; i++) { line += "   "; }
    line += "   ";
    for (size_t i = 0; i < n; i++) {
      if (i == 8) { line += "  "; }
      uint8_t c = data[i];
      if (!(0x20 <= c && c <= 0x7e)) c = '.';
      line += fs("%c", c);
    }
    binarys.push_back(line);
    bufferlen -= n;
    data += n;
    row  += n;
  }
}
Packet::Packet(const std::string s) : str(s)
{
  /*
   * Test Init Details
   */
  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "%s: slankdiafadfdpdk %zd tetste ", this->str.c_str(), pad_y);
    details.push_back(ToggleList_Element(str));
  }
}


class PacketListPane : public PaneInterface {
  size_t cursor;
  size_t start_idx;
 public:
  std::vector<Packet*> packets;
  PacketListPane(size_t _x, size_t _y, size_t _w, size_t _h);

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
std::string Packet::to_str() const
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


