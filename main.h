#include <X11/Xlib.h>

#ifndef main_h
#define main_h

void capture(unsigned char *data, int *hover, int *vertical, int *horizontal);
int combine(short dollars, short cents);
void press(Display *d, Window w);
void release(Display *d, Window w);
void move(int vertical, int horizontal, Display *d, Window w);
int toPixel(int coordinateMin, int coordinateMax, int coordinate, int pixels);

#endif
