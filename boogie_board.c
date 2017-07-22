#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>

#define HOVER 32
#define PRESS 33
#define SIGNAL 27

void capture(FILE *f, unsigned char *s, int bytes, int *hover, int *vertical, int *horizontal);
int combine(short dollars, short cents);
void draw(int vertical, int horizontal, int hover, Display *d, Window w);
int toPixel(int coordinateMin, int coordinateMax, int coordinate, int pixels);

int main() {
	int i;
	int counter = 0;
	int zero=1;
	int vertical;
	int horizontal;
	int hover=0;
	int bytes=8;
	int screenWidth;
	int screenHeight;
	int coordinateVerticalMin=250;
	int coordinateVerticalMax=13800;
	int coordinateHorizontalMin=200;
	int coordinateHorizontalMax=20300;
	unsigned char s[bytes];
	FILE *f = fopen("/dev/usb/hiddev0", "rb");

	Display* d = XOpenDisplay(NULL);
	Screen* sc = DefaultScreenOfDisplay(d);
	screenWidth=sc->width;
	screenHeight=sc->height;
	
	Window root_window = XRootWindow(d, 0);
	XSelectInput(d, root_window, KeyReleaseMask);

	while (1) {
		fread(s, sizeof(char), bytes, f);
		
		if(s[4] == SIGNAL) {
			capture(f, s, bytes, &hover, &vertical, &horizontal);
			//printf("%d-%02d,%02d\n", hover, horizontal, vertical);
			//draw(toPixel(vertical, screenHeight), toPixel(horizontal, screenWidth), hover);
			//printf("%d:%d, %d\n", hover, toPixel(coordinateHorizontalMin, coordinateHorizontalMax, horizontal, screenWidth), toPixel(coordinateVerticalMin, coordinateVerticalMax, vertical, screenHeight));
			draw(toPixel(coordinateVerticalMin, coordinateVerticalMax, vertical, screenHeight),toPixel(coordinateHorizontalMin, coordinateHorizontalMax, horizontal, screenWidth),hover,d, root_window);
		}
	}

	return 0;
}

void capture(FILE *f, unsigned char *s, int bytes, int *hover,int *vertical, int *horizontal) {

	int i;
	short cents;
	short dollars;

	//eat up two lines of useless information
	fread(s, sizeof(char), bytes, f);
	fread(s, sizeof(char), bytes, f);

	fread(s, sizeof(char), bytes, f);

	if(s[4] == HOVER) {
		*hover=1;
	} else if (s[4] == PRESS) {
		*hover=0;	
	}

	fread(s, sizeof(char), bytes, f);
	cents = s[4];
	
	fread(s, sizeof(char), bytes, f);
	dollars = s[4];
	
	*horizontal = combine(cents, dollars);

	fread(s, sizeof(char), bytes, f);
	cents = s[4];
	
	fread(s, sizeof(char), bytes, f);
	dollars = s[4];
	
	*vertical = combine(cents, dollars);

}

int combine(short dollars, short cents) {
	int combined = 0;
	combined = dollars + (cents << 8) ;
	return combined;
}

void draw(int vertical, int horizontal, int hover, Display *d, Window w) {
	XWarpPointer(d, None, w, 0,0,0,0, horizontal, vertical);
	XFlush(d);
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


//00 00 06 00 1b 00 00 00 start
//00 00 06 00 0e 00 00 00 //
//00 00 06 00 01 00 00 00 //
//00 00 06 00 20 00 00 00 press or hover
//00 00 06 00 58 00 00 00 vertical cents
//00 00 06 00 07 00 00 00 vertical dollars
//00 00 06 00 a8 00 00 00 horizontal cents
//00 00 06 00 15 00 00 00 horizontal dollars
//00 00 06 00 00 00 00 00 //
//00 00 06 00 00 00 00 00 //
//00 00 06 00 20 00 00 00 //
//00 00 06 00 00 00 00 00 //
//00 00 06 00 00 00 00 00 //
//00 00 06 00 00 00 00 00 //
//00 00 06 00 00 00 00 00 //
