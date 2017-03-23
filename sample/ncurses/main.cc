
#include <stdarg.h>
#include <slankdev/string.h>
#include <slankdev/ncurses.h>
#include "ncurses.h"
#include <string>
#include <vector>


class Lines {
  std::vector<std::string> lines_;
 public:
  void addline(std::string s) { lines_.push_back(s); }
  std::string& operator[](size_t n) { return lines_.at(n); }
  size_t size() const { return lines_.size(); }
};


class Buf {
  WINDOW* win;
  const size_t x_pos;
  const size_t y_pos;
  const size_t w;
  std::string str;
 public:
  Buf(WINDOW* rootwin, size_t ix_pos, size_t iy_pos, size_t iw)
    : x_pos(ix_pos),
    y_pos(iy_pos),
    w    (iw    )
  {
    win = slankdev::subwin(rootwin, 1, w, y_pos, x_pos);
  }
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

class Pane {
  Lines lines;
  size_t cursor;
  size_t start_idx;
  WINDOW* win;
  const size_t x_pos;
  const size_t y_pos;
  const size_t w;
  const size_t h;
 public:
  Pane(WINDOW* rootwin, size_t ix_pos, size_t iy_pos, size_t iw, size_t ih)
    : cursor(0),
    start_idx(0),
    x_pos(ix_pos),
    y_pos(iy_pos),
    w    (iw    ),
    h    (ih    )
  {
    win = slankdev::subwin(rootwin, h, w, y_pos, x_pos);
  }
  template<class... ARGS>
  void print(const char* fmt, ARGS... args)
  {
    std::string s = slankdev::fs(fmt, args...);
    lines.addline(s);
  }
  void refresh()
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
  void cursor_down()
  {
    if (cursor + 1 < lines.size()) {
      if (cursor - start_idx + 2 > h) scroll_down();
      cursor++;
    }
  }
  void cursor_up()
  {
    if (cursor > 0) {
      if (cursor - 1 < start_idx) scroll_up();
      cursor--;
    }
  }
 private:
  void scroll_down() { start_idx++; }
  void scroll_up()   { start_idx--; }

 /* Debug Functions */
 public:
  size_t cur() const { return cursor; }
  WINDOW* ww() { return win; }
};



int	main(int argc, char** argv)
{
	initscr();
	noecho();
  scrollok(stdscr, false);

  Lines lines;

  size_t sublines = LINES/3-1;
  Pane pane1(stdscr, 0, sublines*0+1, COLS, sublines-1);
  Pane pane2(stdscr, 0, sublines*1, COLS, sublines);
  Pane pane3(stdscr, 0, sublines*2, COLS, sublines);
  Buf  buf  (stdscr, 0, sublines*3+1, COLS);

  // waddstr(sub4, "Status LINE");
  // waddstr(sub4, "Status LINE");

  pane1.print("SLNKDEV");
  pane1.print("ssssssssssdfdfdfdd");

  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "adfdffdfdfaaaaaaaaaaaaaaaaaa %zd tetste ", pad_y);
    pane1.print(str);
  }

  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "slankdiafadfdpdk %zd tetste ", pad_y);
    pane2.print(str);
  }

  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "sudo apt install dfadf %zd tetste ", pad_y);
    pane3.print(str);
  }

  // box(pane1.ww(), '|', '-');
  // box(pane2.ww(), '|', '-');

  pane1.refresh();
  pane2.refresh();
  pane3.refresh();
  buf.refresh();

  std::string sss;
	while (1) {

    char inc = ' ';
    char c = wgetch(stdscr);
    if (c == 0x1b) break;

    if (c == 'J') {
      pane1.cursor_down();
    } else if (c == 'K') {
      pane1.cursor_up();
    } else if (c == 'j') {
      pane2.cursor_down();
    } else if (c == 'k') {
      pane2.cursor_up();
    } else if (c == 'h') {
      pane3.cursor_down();
    } else if (c == 'l') {
      pane3.cursor_up();
    } else if (c == 'N') {
      pane1.cursor_down();
      pane2.cursor_down();
      pane3.cursor_down();
    } else if (c == 'P') {
      pane1.cursor_up();
      pane2.cursor_up();
      pane3.cursor_up();
    } else {
      inc = c;
    }

    buf.print("1[%zd] 2[%zd] 3[%zd] [%c] ", pane1.cur(), pane2.cur(), pane3.cur(), inc);

    pane1.refresh();
    pane2.refresh();
    pane3.refresh();
    buf.refresh();
	}

	endwin();
}


