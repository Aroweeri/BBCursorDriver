#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void capture(FILE *f);

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
			counter++;
			printf("%d\n", counter);
		}
		
		for(i=0;i<bytes;i++) {
			if(i%8 == 0) {
				//printf("\n");
			}
			//printf("%02x ", s[i]);
		}
		//printf("\n");
	}
	return 0;
}

void capture(FILE *f) {
	
}
