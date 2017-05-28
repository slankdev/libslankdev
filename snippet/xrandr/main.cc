
#include <stdio.h>
#include <stdint.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>



struct Monitor {
	char ltsymbol[16];
	float mfact;
	size_t nmaster;
	int num;
	size_t by;               /* bar geometry */
	size_t mx, my, mw, mh;   /* screen size */
	size_t wx, wy, ww, wh;   /* window area  */
	uint32_t seltags;
	uint32_t sellt;
	uint32_t tagset[2];
	bool showbar;
	bool topbar;
	// Client *clients;
	// Client *sel;
	// Client *stack;
	Monitor *next;
	Window barwin;
	// const Layout *lt[2];
};



void print_XRRMonitorInfo(XRRMonitorInfo* m, int n)
{
    for (int i=0; i<n; i++) {
        printf("XRRMon %d\n", i);
        printf(" - x        : %d\n", m[i].x        );
        printf(" - y        : %d\n", m[i].y        );
        printf(" - width    : %d\n", m[i].width    );
        printf(" - height   : %d\n", m[i].height   );
    }
}


void *ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		perror(NULL);
	return p;
}




Monitor* alloc_Monitor()
{
    Monitor* m = (Monitor*)ecalloc(1, sizeof(Monitor));
    return m;
}

int get_index_now_pointing(XRRMonitorInfo* mons, int num_scrn, int x, int y)
{
    int index;
    for (index=0; index<num_scrn; index++) {
        int x_begin = mons[index].x;
        int x_end   = mons[index].x + mons[index].width;
        int y_begin = mons[index].y;
        int y_end   = mons[index].y + mons[index].height;

        if (x_begin <= x && x < x_end && y_begin <= y && y < y_end)
            break;
    }
    if (index < num_scrn)
        return index;
    else
        return -1;
}



int main()
{
    char* display_name = NULL;
    Display *dpy = XOpenDisplay(display_name);
    if (dpy == NULL) {
	    fprintf (stderr, "Can't open display %s\n", XDisplayName(display_name));
        return -1;
    }

    int screen = DefaultScreen(dpy);
    Window root = RootWindow(dpy, screen);

    int num_scrn;
    XRRMonitorInfo* m = XRRGetMonitors(dpy, root, true, &num_scrn);
    print_XRRMonitorInfo(m, num_scrn);

    int root_x, root_y, win_x, win_y;
    Window window_returned;
    uint32_t mask_return;
    bool res = XQueryPointer(dpy, root, &window_returned, &window_returned,
            &root_x, &root_y, &win_x, &win_y, &mask_return);
    if (!res) {
        fprintf(stderr, "ERROR \n");
        return -1;
    }
    printf("Mouse(%d, %d)(%d, %d) \n", root_x, root_y, win_x, win_y);

    printf("\n");
    int num = get_index_now_pointing(m, num_scrn, win_x, win_y);
    printf("Now at Monitor%d \n", num);



    return 0;
}
