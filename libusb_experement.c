#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

int main (int argc, char *argv) {
	libusb_device_handle             *devHandle = NULL;

	unsigned char endpoint;
	unsigned char* data;
	int length;
	int *transferred;
	unsigned int timeout;
	int i;
	struct libusb_context* context;

	libusb_init (&context);

	devHandle = libusb_open_device_with_vid_pid(context,0x2047,0xffe7 );

	endpoint = 0x83; //found from lsusb -v
	data = (char*)malloc(sizeof(char)*64);
	length = 64; //found from lsusb -v
	transferred = (int*)malloc(sizeof(int));
	*transferred = 0;
	timeout = 0;
	for(i = 0;i<64;i++) {
		data[i] = 0;
	}
	printf("detach kernel driver returns: %d\n", libusb_detach_kernel_driver(devHandle, 1));
	printf("claim_interface returns: %d\n", libusb_claim_interface(devHandle, 2));

	while(1) {
		printf("Returns: %d\n", libusb_interrupt_transfer(devHandle, endpoint, data, length, transferred, timeout));
		printf("Data: transferred = %d\n", *transferred);
		for(i = 0;i<64;i++) {
			if(i%8 == 0 && i != 0) {
				printf("\n");
			}
			printf("%02x ", data[i]);
		}
		printf("\n");
		//usleep(1000000);
	}

	free(data);
	free(transferred);

	libusb_exit (NULL);

	return 0;
}
