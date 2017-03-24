


#include <string>
#include <slankdev/string.h>
#include "TuiFrontend.h"


void func(TuiFrontend& fe)
{
  for(size_t pad_y = 0; pad_y < 40; pad_y++){
    std::string s = slankdev::fs("packet (%zd)", pad_y);
    fe.pane1.add_packet(s);
  }
}


int	main(int argc, char** argv)
{
  TuiFrontend::init();
  TuiFrontend fe;

  func(fe);

  fe.refresh();
	while (1) {
    fe.key_input(wgetch(stdscr));
    fe.refresh();
	}
	endwin();
}


