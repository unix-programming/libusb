#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <poll.h>
#include <libusb.h>


int init_usb();

int main()
{
	libusb_device **list;
	ssize_t cnt;
        ssize_t i = 0;
        libusb_device *device;
	struct libusb_device_descriptor desc;
	int ret;
	int j;

	ret = init_usb();
	if(ret !=0)
		return -1;

	//find usb device list
	cnt = libusb_get_device_list(NULL, &list);

        if (cnt < 0)
                goto error0;

        for (i = 0; i < cnt; i++) {
                device = list[i];
                int r;

                r = libusb_get_device_descriptor(device, &desc);
                if (r < 0)
                        continue;

                if (desc.bDeviceClass == LIBUSB_CLASS_PER_INTERFACE) {
			printf("Class is defined By Interface\n");
                        struct libusb_config_descriptor *current_config;
                        int r;

                        r = libusb_get_active_config_descriptor(device,
                                                                &current_config);
                        if (r < 0)
                                continue;

                        for (j = 0; j < current_config->bNumInterfaces; j++) {
                                int k;
                                for (k = 0;k <current_config->interface[j].num_altsetting; k++) {
					if( current_config->interface[j].altsetting[k].bInterfaceClass == LIBUSB_CLASS_HID)
						printf("this interface is HID\n");
					else if ( current_config->interface[j].altsetting[k].bInterfaceClass == LIBUSB_CLASS_MASS_STORAGE)
						printf("this interface is STORAGE\n"); 
					else if ( current_config->interface[j].altsetting[k].bInterfaceClass == LIBUSB_CLASS_VENDOR_SPEC)
						printf("this interface is VENDOR\n");
					else
						printf("this interface is %x\n",current_config->interface[j].altsetting[k].bInterfaceClass);
					//printf("endpoint address %d\n",current_config->interface[j].altsetting[k].endpoint[0].bEndpointAddress);
                                }
                        }
                        libusb_free_config_descriptor(current_config);
                } else if (desc.bDeviceClass == LIBUSB_CLASS_HUB) {
			//printf("This usb is Hub\n");
		} else {
			printf("This usb is %d\n",desc.bDeviceClass);
		}

	}

error0:
        libusb_free_device_list(list, 1);
	libusb_exit(NULL);
        return -1;
}

int init_usb()
{
	int ret;
        /* Initializing libusb */
        ret = libusb_init(NULL);
        if (ret != 0) {
                printf("libusb init failed: %d\n", ret);
                return ret;
        }
	return 0;
}
