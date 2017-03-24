
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

    char c = wgetch(stdscr);
    if (c == 0x1b) break;

    fe.key_input(c);
    fe.refresh();

	}
	endwin();
}


