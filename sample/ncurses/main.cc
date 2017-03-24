


#include <string>
#include <slankdev/string.h>
#include <slankdev/poll.h>
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

  struct pollfd fds[2];
  memset(fds, 0, sizeof(fds));
  fds[0].fd = fileno(stdin);
  fds[0].events = POLLIN;
  fds[1].fd = -1;
  fds[1].events = POLLIN;

	while (1) {
    slankdev::poll(fds, sizeof(fds)/sizeof(fds[0]), 100);
    if (fds[0].revents & POLLIN) {
      fe.key_input(wgetch(stdscr));
      fe.refresh();
    }
	}
	endwin();
}


