#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#define HOVER 32	/* Signal that comes from the board representing a hover. */
#define PRESS 33	/* Signal that comes from the board representing a press. */
#define SIGNAL 27	/* Indicates that a data packet has been sent from the board. */
#define BYTES 8		/* Length in bytes that the boogie board's packet is. */

void capture(FILE *f, unsigned char *packet, int bytes, int *hover, int *vertical, int *horizontal);
int combine(short dollars, short cents);
void press(Display *d, Window w);
void release(Display *d, Window w);
void move(int vertical, int horizontal, Display *d, Window w);
int toPixel(int coordinateMin, int coordinateMax, int coordinate, int pixels);

int main(int argc, char *argv[]) {
	int vertical;
	int horizontal;
	int hover=0;
	int currentHover=0;
	int screenWidth;
	int screenHeight;

	/* Used to prevent the corners of the screen from being behind the physical plastic on the
	 * boogie board. */
	int coordinateVerticalMin=250;
	int coordinateVerticalMax=13800;
	int coordinateHorizontalMin=200;
	int coordinateHorizontalMax=20300;

	char *path = argv[1];
	unsigned char packet[BYTES];
	FILE *f = fopen(path, "rb");

	/* Find screen dimensions */
	Window root_window;
	Display* d = XOpenDisplay(NULL);
	Screen* sc = DefaultScreenOfDisplay(d);
	screenWidth=sc->width;
	screenHeight=sc->height;
	
	root_window = XRootWindow(d, 0);
	XSelectInput(d, root_window, KeyReleaseMask);

	/* The device will continually send data, is intended to be killed with ctrl+c. */
	while (1) {
		
		/* Read the whole data packet into an array. */
		fread(packet, sizeof(char), BYTES, f);
		
		if(packet[4] == SIGNAL) {
			capture(f, packet, BYTES, &hover, &vertical, &horizontal);
			if(horizontal == 0 && vertical == 0) {
				continue;
			}
			if(hover != currentHover) {
				currentHover = hover;
				if(hover == 1) {
					release(d, root_window);
				} else if (hover == 0) {
					press(d, root_window);
				}	
			}

			move(toPixel(coordinateVerticalMin, coordinateVerticalMax, vertical,
				screenHeight),toPixel(coordinateHorizontalMin,
				coordinateHorizontalMax, horizontal, screenWidth), d, root_window);
		}
	}
	return 0;
}

void capture(FILE *f, unsigned char *packet, int bytes, int *hover,int *vertical, int *horizontal) {

	short cents;
	short dollars;

	/* eat up two lines of useless information */
	fread(packet, sizeof(char), bytes, f);
	fread(packet, sizeof(char), bytes, f);

	fread(packet, sizeof(char), bytes, f);

	if(packet[4] == HOVER) {
		*hover=1;
	} else if (packet[4] == PRESS) {
		*hover=0;	
	}

	fread(packet, sizeof(char), bytes, f);
	cents = packet[4];
	
	fread(packet, sizeof(char), bytes, f);
	dollars = packet[4];
	
	*horizontal = combine(cents, dollars);

	fread(packet, sizeof(char), bytes, f);
	cents = packet[4];
	
	fread(packet, sizeof(char), bytes, f);
	dollars = packet[4];
	
	*vertical = combine(cents, dollars);
}

int combine(short dollars, short cents) {
	int combined = 0;
	combined = dollars + (cents << 8) ;
	return combined;
}

void move(int vertical, int horizontal, Display *d, Window w) {
	XTestFakeMotionEvent(d, 0, horizontal, vertical, CurrentTime);
	XFlush(d);
}

void press(Display *d, Window w) {
	XTestFakeButtonEvent(d, Button1, True, CurrentTime);
}

void release(Display *d, Window w) {
	XTestFakeButtonEvent(d, Button1, False, CurrentTime);
}

int toPixel(int coordinateMin, int coordinateMax, int coordinate, int pixels) {
	int coordinatesPerPixel=coordinateMax/pixels;
	int subtract = coordinateMin/coordinatesPerPixel;
	int pixel = coordinate/coordinatesPerPixel-subtract;
	if (pixel > pixels) {
		pixel = pixels;
	} else if (pixel < 1) {
		pixel = 1;
	}
	return pixel;
}

/* Example of what the data packet from the boogie board looks like. */

/*
00 00 06 00 1b 00 00 00 start
00 00 06 00 0e 00 00 00 //
00 00 06 00 01 00 00 00 //
00 00 06 00 20 00 00 00 press or hover
00 00 06 00 58 00 00 00 vertical cents
00 00 06 00 07 00 00 00 vertical dollars
00 00 06 00 a8 00 00 00 horizontal cents
00 00 06 00 15 00 00 00 horizontal dollars
00 00 06 00 00 00 00 00 //
00 00 06 00 00 00 00 00 //
00 00 06 00 20 00 00 00 //
00 00 06 00 00 00 00 00 //
00 00 06 00 00 00 00 00 //
00 00 06 00 00 00 00 00 //
00 00 06 00 00 00 00 00 //
*/
