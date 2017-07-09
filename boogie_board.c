#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define HOVER 32
#define PRESS 33
#define SIGNAL 27

void capture(FILE *f, unsigned char *s, int bytes, short *vertical, short *horizontal);
short combine(char dollars, char cents);

int main() {
	int i;
	int counter = 0;
	int zero=1;
	short vertical;
	short horizontal;
	int bytes=8;
	unsigned char s[bytes];
	FILE *f = fopen("/dev/usb/hiddev0", "rb");

	while (1) {
		fread(s, sizeof(char), bytes, f);
		
		if(s[4] == 27) {
			capture(f, s, bytes, &vertical, &horizontal);
		}
	}

	return 0;
}

void capture(FILE *f, unsigned char *s, int bytes, short *vertical, short *horizontal) {

	int i;
	int hover=0;
	char cents;
	char dollars;

	//eat up two lines of useless information
	fread(s, sizeof(char), bytes, f);
	fread(s, sizeof(char), bytes, f);

	fread(s, sizeof(char), bytes, f);

	if(s[4] == HOVER) {
		hover=1;
	} else if (s[4] == PRESS) {
		hover=0;	
	}

	fread(s, sizeof(char), bytes, f);
	cents = s[4];
	
	fread(s, sizeof(char), bytes, f);
	dollars = s[4];
	
	printf("%d%d\n", dollars, cents);
	//*vertical = combine(cents, dollars);

	fread(s, sizeof(char), bytes, f);
	cents = s[4];
	
	fread(s, sizeof(char), bytes, f);
	dollars = s[4];
	
	printf("%0x%0x\n", dollars, cents);
	//*horizontal = combine(cents, dollars);

}

short combine(char dollars, char cents) {
	short combined = 0;
	return combined;
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
