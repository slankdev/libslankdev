


#include <stdio.h>
#include <slankdev.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>	/* we share subpixel information */





int main()
{
    static Display *dpy;
    int screen;
    int cnt_screen;
    char* display_name = NULL;
    Window root;
    int minWidth;
    int minHeight;
    int maxWidth;
    int maxHeight;
    XEvent event;

    dpy = XOpenDisplay(display_name);
    if (dpy == NULL) {
	    fprintf (stderr, "Can't open display %s\n", XDisplayName(display_name));
        return -1;
    }

    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);

    XRRGetScreenSizeRange(dpy, root, &minWidth, &minHeight, &maxWidth, &maxHeight);
    printf("Screen %d: ", DefaultScreen(dpy));
    printf("minimum %d x %d, ", minWidth, minHeight);
    printf("current %d x %d, ", DisplayWidth(dpy, screen), DisplayHeight(dpy, screen));
    printf("maximum %d x %d, ", maxWidth, maxHeight);
    printf("\n");
 
    Window win0 = XCreateSimpleWindow(dpy, root, 100, 100, 500, 300, 1, 
            BlackPixel(dpy, screen), WhitePixel(dpy, screen));

    Window win1 = XCreateSimpleWindow(dpy, root, 100, 100, 500, 300, 1, 
            BlackPixel(dpy, screen), WhitePixel(dpy, screen));

    XSelectInput(dpy, win0, KeyPressMask);
    XSelectInput(dpy, win1, KeyPressMask);
    XMapWindow(dpy, win0);
    XMapWindow(dpy, win1);
    while (1) {
        XNextEvent(dpy, &event);
    }


    return 0;
}
