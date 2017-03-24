
#include <stdarg.h>
#include <slankdev/string.h>
#include <slankdev/ncurses.h>
#include "ncurses.h"
#include <string>
#include <vector>

#include "TuiFrontend.h"


void func(TuiFrontend* fe)
{
  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "adfdffdfdfaaaaaaaaaaaaaaaaaa %zd tetste ", pad_y);
    fe->pane1->print(str);
  }

  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "slankdiafadfdpdk %zd tetste ", pad_y);
    fe->pane2->add_element(str);
  }

  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    char str[1000];
    sprintf(str, "sudo apt install dfadf %zd tetste ", pad_y);
    fe->pane3->print(str);
  }
}



int	main(int argc, char** argv)
{
  TuiFrontend fe;
  fe.init();

  func(&fe);

  fe.refresh();

	while (1) {

    char inc = ' ';
    char c = wgetch(stdscr);
    if (c == 0x1b) break;

    if (c == 'J') {
      fe.pane1->cursor_down();
    } else if (c == 'K') {
      fe.pane1->cursor_up();
    } else if (c == 'j') {
      fe.pane2->cursor_down();
    } else if (c == 'k') {
      fe.pane2->cursor_up();
    } else if (c == 'h') {
      fe.pane3->cursor_down();
    } else if (c == 'l') {
      fe.pane3->cursor_up();
    } else if (c == 'N') {
      fe.pane1->cursor_down();
      fe.pane2->cursor_down();
      fe.pane3->cursor_down();
    } else if (c == 'P') {
      fe.pane1->cursor_up();
      fe.pane2->cursor_up();
      fe.pane3->cursor_up();
    } else if (c == ' ') {
      ToggleList::toggle();
    } else {
      inc = c;
    }

    fe.sline->print("1[%zd] 2[%zd] 3[%zd] [%c] ",
        fe.pane1->cur(),
        fe.pane2->cur(),
        fe.pane3->cur(), inc);
    fe.refresh();
	}
	endwin();
}


