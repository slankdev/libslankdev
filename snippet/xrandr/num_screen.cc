


#include <stdio.h>
#include <slankdev.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>	/* we share subpixel information */




void print_XRRMonitorInfo(XRRMonitorInfo* m)
{
    printf("XRRMon\n");
    printf(" - x        : %d\n", m->x        );
    printf(" - y        : %d\n", m->y        );
    printf(" - width    : %d\n", m->width    );
    printf(" - height   : %d\n", m->height   );
    // printf(" - mwidth   : %d\n", m->mwidth   );
    // printf(" - mheight  : %d\n", m->mheight  );
    // printf(" - outputs  : %p\n", m->outputs);
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
    for (int i=0; i<num_scrn; i++)
        print_XRRMonitorInfo(&m[i]);
    


    return 0;
}
