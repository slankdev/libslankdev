
#include <stdarg.h>
#include <slankdev/string.h>
#include <slankdev/ncurses.h>
#include "ncurses.h"
#include <string>
#include <vector>

#include "statusline.h"
#include "pane.h"


// class TuiFrontend {
//  private:
//
//  public:
//   TuiFrontend()
//   {
//   }
//   virtual ~TuiFrontend() {}
// };



int	main(int argc, char** argv)
{
  // TuiFrontend frontend;

	initscr();
	noecho();
  scrollok(stdscr, false);

  Lines lines;

  size_t sublines = LINES/3-1;
  Pane pane1(0, sublines*0+1, COLS, sublines-1);
  Pane pane2(0, sublines*1  , COLS, sublines);
  Pane pane3(0, sublines*2  , COLS, sublines);
  pane1.init(stdscr);
  pane2.init(stdscr);
  pane3.init(stdscr);
  Statusline  buf(stdscr, 0, sublines*3+1, COLS);

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


