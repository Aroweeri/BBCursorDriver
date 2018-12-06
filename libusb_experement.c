#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

int main (int argc, char *argv)
{
   libusb_device                    **devList = NULL;
   libusb_device                    *devPtr = NULL;
   libusb_device_handle             *devHandle = NULL;
   char bbindex = 0;
   struct libusb_device_descriptor  devDesc;

   unsigned char              strDesc[256];
   ssize_t                    numUsbDevs = 0;      // pre-initialized scalars
   ssize_t                    idx = 0;
   int                        retVal = 0;

		unsigned char endpoint;
		unsigned char* data;
		int length;
		int *transferred;
		unsigned int timeout;
		int i;
		struct libusb_context* context;

   //========================================================================
   // test out the libusb functions
   //========================================================================

   retVal = libusb_init (&context);

   //========================================================================
   // Get the list of USB devices visible to the system.
   //========================================================================

   numUsbDevs = libusb_get_device_list (NULL, &devList);

   //========================================================================
   // Loop through the list, looking for the device with the required values
   // for Manufacturer ("RockwellCollins") and Product ("TACDIS").
   //========================================================================

   while (idx < numUsbDevs)
   {

      //=====================================================================
      // Get next device pointer out of the list, use it to open the device.
      //=====================================================================

      devPtr = devList[idx];

      retVal = libusb_open (devPtr, &devHandle);
      if (retVal != LIBUSB_SUCCESS)
         break;

      //=====================================================================
      // Get the device descriptor for this device.
      //=====================================================================

      retVal = libusb_get_device_descriptor (devPtr, &devDesc);
      if (retVal != LIBUSB_SUCCESS)
         break;

      //=====================================================================
      // Get the string associated with iManufacturer index.
      //=====================================================================

      printf ("   iManufacturer = %d\n", devDesc.iManufacturer);
      if (devDesc.iManufacturer > 0)
      {
         retVal = libusb_get_string_descriptor_ascii
                  (devHandle, devDesc.iManufacturer, strDesc, 256);
         if (retVal < 0)
            break;

         printf ("   string = %s\n",  strDesc);
         if(strcmp(strDesc, "Improv") == 0) {
            bbindex = idx;
         }
      }

      //========================================================================
      // Get string associated with iProduct index.
      //========================================================================

      printf ("   iProduct = %d\n", devDesc.iProduct);
      if (devDesc.iProduct > 0)
      {
         retVal = libusb_get_string_descriptor_ascii
                  (devHandle, devDesc.iProduct, strDesc, 256);
         if (retVal < 0)
            break;

         printf ("   string = %s\n", strDesc);
      }

      //==================================================================
      // Get string associated with iSerialNumber index.
      //==================================================================

      printf ("   iSerialNumber = %d\n", devDesc.iSerialNumber);
      if (devDesc.iSerialNumber > 0)
      {
         retVal = libusb_get_string_descriptor_ascii
                  (devHandle, devDesc.iSerialNumber, strDesc, 256);
         if (retVal < 0)
            break;

         printf ("   string = %s\n", strDesc);
      }

      //========================================================================
      // Close and try next one.
      //========================================================================

      libusb_close (devHandle);
      devHandle = NULL;
      idx++;
   }  // end of while loop

   if (devHandle != NULL)
   {
      //========================================================================
      // Close device if left open due to break out of loop on error.
      //========================================================================

      libusb_close (devHandle);
   }   

   printf("bbindex: %d\n", bbindex);
   printf("so %d\n", libusb_open (devPtr, &devHandle));
   printf("so %d\n", devHandle = libusb_open_device_with_vid_pid(context,0x2047,0xffe7 ));

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
			usleep(1000000);
		}

		free(data);
		free(transferred);

   libusb_exit (NULL);

   printf ("\n*************************\n        Done\n*************************\n");
   return 0;
}

//==========================================
// EOF
//==========================================
