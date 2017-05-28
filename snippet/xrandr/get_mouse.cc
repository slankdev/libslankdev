


#include <stdio.h>
#include <slankdev.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>




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


    int minWidth;
    int minHeight;
    int maxWidth;
    int maxHeight;
    XRRGetScreenSizeRange(dpy, root, &minWidth, &minHeight, &maxWidth, &maxHeight);
    printf("Screen %d: ", DefaultScreen(dpy));
    printf("minimum %d x %d, ", minWidth, minHeight);
    printf("current %d x %d, ", DisplayWidth(dpy, screen), DisplayHeight(dpy, screen));
    printf("maximum %d x %d, ", maxWidth, maxHeight);
    printf("\n");
 


    int root_x, root_y, win_x, win_y;
    Window window_returned;
    uint32_t mask_return;

    while (1) {
        bool res = XQueryPointer(dpy, root, &window_returned, &window_returned,
                &root_x, &root_y, &win_x, &win_y, &mask_return);
        if (!res) {
            fprintf(stderr, "ERROR \n");
            return -1;
        }
        printf("Mouse(%d, %d)(%d, %d) \n", root_x, root_y, win_x, win_y);
    }


    return 0;
}
