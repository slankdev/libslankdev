
#include <stdarg.h>
#include <slankdev/ncurses.h>



int main()
{
    slankdev::ncurses screen;
    screen.mvprintw(9 , 20, "screen size %d x %d \n" , screen.getw(), screen.geth());
	screen.mvprintw(10, 20, "Hello World position %d, %d", 10, 20);
    screen.mvprintw(11, 20, "slankdev");
	screen.refresh();
    screen.getchar();
}
