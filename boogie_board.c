#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int isZero(unsigned char *s,int bytes);

int main() {
	int i;
	int zero=1;
	int bytes=120;
	unsigned char s[bytes];
	FILE *f = fopen("/dev/usb/hiddev0", "rb");

	for(i=0;i<120;i++) {
		s[i] = 0;
	}

	while (1) {

		while(isZero(s, bytes) == 1) {
			fread(s, sizeof(char), 120, f);
		}
		
		for(i=0;i<bytes;i++) {
			if(i%8 == 0) {
				printf("\n");
			}
			printf("%02x ", s[i]);
		}
		printf("\n");
		fseek(f, 0, SEEK_END);
	}
	return 0;
}

int isZero(unsigned char *s, int bytes) {
	int i;
	int zero = 1;
	for(i=0;i<bytes;i++) {
		if(s[i] != 0) {
			zero=0;
		}
	}
	return zero;
}
