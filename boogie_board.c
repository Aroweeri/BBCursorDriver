#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HOVER 32
#define PRESS 33
#define SIGNAL 27

void capture(FILE *f, unsigned char *s, int bytes);

int main() {
	int i;
	int counter = 0;
	int zero=1;
	int bytes=8;
	unsigned char s[bytes];
	FILE *f = fopen("/dev/usb/hiddev0", "rb");

	while (1) {
		fread(s, sizeof(char), bytes, f);
		
		if(s[4] == 27) {
			capture(f, s, bytes);
		}
	}

	return 0;
}

void capture(FILE *f, unsigned char *s, int bytes) {

	int i;
	int hover=0;

	//eat up two lines of useless information
	fread(s, sizeof(char), bytes, f);
	fread(s, sizeof(char), bytes, f);

	fread(s, sizeof(char), bytes, f);

	if(s[4] == HOVER) {
		hover=1;
	} else if (s[4] == PRESS) {
		hover=0;	
	}
	
	
	fflush(stdout);
}


//00 00 06 00 1b 00 00 00 
//00 00 06 00 0e 00 00 00 
//00 00 06 00 01 00 00 00 
//00 00 06 00 20 00 00 00 
//00 00 06 00 58 00 00 00 
//00 00 06 00 07 00 00 00 
//00 00 06 00 a8 00 00 00 
//00 00 06 00 15 00 00 00 
//00 00 06 00 00 00 00 00 
//00 00 06 00 00 00 00 00 
//00 00 06 00 20 00 00 00 
//00 00 06 00 00 00 00 00 
//00 00 06 00 00 00 00 00 
//00 00 06 00 00 00 00 00 
//00 00 06 00 00 00 00 00
