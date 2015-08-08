#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdint>

#include <omp.h>
#include <libusb-1.0/libusb.h>

	
#define IDVENDOR 	0x10C4
#define IDPRODUCT	0xFE01
	
#define BULK_EP_OUT     0x01   
#define BULK_EP_IN      0x81    
	
using namespace std;

/**
    Return device ID strings from the usb device.

    The parameters manufacturer, description and serial may be NULL
    or pointer to buffers to store the fetched strings.

    \note Use this function only in combination with ftdi_usb_find_all()
          as it closes the internal "usb_dev" after use.

    \param dev libusb usb_dev to use
    \param manufacturer Store manufacturer string here if not NULL
    \param mnf_len Buffer size of manufacturer string
    \param description Store product description string here if not NULL
    \param desc_len Buffer size of product description string
    \param serial Store serial string here if not NULL
    \param serial_len Buffer size of serial string

    \retval   0: all fine
    \retval  -1: wrong arguments
    \retval  -4: unable to open device
    \retval  -7: get product manufacturer failed
    \retval  -8: get product description failed
    \retval  -9: get serial number failed
    \retval -11: libusb_get_device_descriptor() failed
*/

int usb_get_strings(struct libusb_device * dev,
                         char * manufacturer, int mnf_len, char * description, int desc_len, char * serial, int serial_len){
    struct libusb_device_descriptor desc;
	libusb_device_handle* usb_dev;
	if (libusb_open(dev, &usb_dev) < 0)
		throw new std::string("libusb_open() failed");

    if (libusb_get_device_descriptor(dev, &desc) < 0)
        throw new std::string("libusb_get_device_descriptor() failed");

    if (manufacturer != NULL){
        if (libusb_get_string_descriptor_ascii(usb_dev, desc.iManufacturer, (unsigned char *)manufacturer, mnf_len) < 0){
            libusb_close(usb_dev);
            throw new std::string("libusb_get_string_descriptor_ascii() failed");
        }
    }

    if (description != NULL){
        if (libusb_get_string_descriptor_ascii(usb_dev, desc.iProduct, (unsigned char *)description, desc_len) < 0){
            libusb_close(usb_dev);
            throw new std::string("libusb_get_string_descriptor_ascii() failed");
        }
    }

    if (serial != NULL){
        if (libusb_get_string_descriptor_ascii(usb_dev, desc.iSerialNumber, (unsigned char *)serial, serial_len) < 0){
            libusb_close(usb_dev);
            throw new std::string("libusb_get_string_descriptor_ascii() failed");
        }
    }
	libusb_close(usb_dev);
    return 0;
}

int err;
	
void readWriteLoop(libusb_device_handle *handle){
	for(int j=0;j<10;j++){
		try{
			unsigned char data[8];// = {0x12,0x34,0x56,0x78};
			for(int i=0;i<sizeof(data);i++) data[i]+=i;
			int actual_length;
			err = libusb_bulk_transfer(handle, BULK_EP_OUT, data, sizeof(data), &actual_length, 0);
			if(err || (actual_length!=sizeof(data))) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");
			
			for(int i=0;i<sizeof(data);i++) data[i]=0;
			libusb_bulk_transfer(handle, BULK_EP_IN, data, sizeof(data), &actual_length, 0);
			if(err || (actual_length!=sizeof(data))) throw new std::string("ERROR: libusb_bulk_transfer() IN failed");
			// results of the transaction can now be found in the data buffer
			// parse them here and report button press
			for(int i=0;i<actual_length;i++) printf("%02X ",data[i]);
			printf("\n");
			
		}catch(std::string *s){
			printf("%s\nerr=0x%08X\n",s->c_str(),err);
		}
	}
}


void readBenchmark(libusb_device_handle *handle){
	volatile uint32_t cnt = 0;
	uint32_t dataStorage[8];
	uint32_t data[256*1024];
	int actual_length;
	//err = libusb_bulk_transfer(handle, BULK_EP_OUT, (uint8_t*)data, sizeof(data), &actual_length, 0);
	//if(err || (actual_length!=sizeof(data))) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");
	
	dataStorage[0]=0;
	dataStorage[1]=1;
	// Starting the time measurement
	double start = omp_get_wtime();
	int len = 1024*1014*10;				
	for(int j=0;j<len/sizeof(data);j++){
		try{
			//for(int i=0;i<2;i++) dataStorage[i]=cnt++;
			//data[0]=0;data[1]=0;
			err = libusb_bulk_transfer(handle, BULK_EP_IN, (uint8_t*)data, sizeof(data), &actual_length, 0);
			if(err || (actual_length!=sizeof(data))) throw new std::string("ERROR: libusb_bulk_transfer() IN failed");
			std::cout<<"."<<std::flush;
			/*for(int i=0;i<2;i++) {
				data[i] = (data[i]>>24)|(0xFF00 & (data[i]>>8))|(0xFF0000 & (data[i]<<8))|(0xFF000000 & (data[i]<<24));
				if(dataStorage[i]!=data[i]) {
					printf("Communication error: %08X expected, got %08X\n",dataStorage[i],data[i]);
					cnt=data[i]+1;
					//throw new std::string("ERROR: comm failed");
				}else{
					//printf("rx %08X\n",data[i]);
				}
			}*/
			
		}catch(std::string *s){
			printf("%s\nerr=0x%08X\n",s->c_str(),err);
		}
	}
	// Measuring the elapsed time
	double end = omp_get_wtime();
	
	// Time calculation (in seconds)	
	double exec_time = end-start;
	
	double mbits = ((double)len)/(1024*1024/8);
	double mbits_per_sec = mbits / exec_time;
	std::cout << "Reply time is: " << exec_time <<"s, "<< mbits_per_sec <<"MBits/s ("<< mbits_per_sec/8 <<"MBytes/s)\n";
	
}

typedef enum {
	CMD_NOP = 0x00, CMD_READ = 0x01, CMD_WRITE = 0x02, CMD_UNKNOWN = 0xFF
};

uint8_t membuf[256];
	
void write_mem(libusb_device_handle *handle,uint32_t addr, const uint8_t * const dat, uint32_t len){
	uint32_t i;
	for(i=0;i<len;i++){
		membuf[addr+i]=dat[i];
	}
	uint8_t cmd[8];
	cmd[0]=CMD_WRITE;
	cmd[1]=len>>16;
	cmd[2]=len>>8;
	cmd[3]=len;
	cmd[4]=addr>>24;
	cmd[5]=addr>>16;
	cmd[6]=addr>>8;
	cmd[7]=addr;
	int actual_length;
	//send cmd
	err = libusb_bulk_transfer(handle, BULK_EP_OUT, cmd, sizeof(cmd), &actual_length, 0);
	if(err || (actual_length!=sizeof(cmd))) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");
	//get ack
	uint8_t ack[2];
	err = libusb_bulk_transfer(handle, BULK_EP_IN, ack, 2, &actual_length, 0);
	if(err) throw new std::string("ERROR: libusb_bulk_transfer() IN failed");
	if(ack[0]!=cmd[0]){
		printf("%d %02X %02X\n",actual_length,ack[0],ack[1]);
		throw new std::string("ERROR: unexpected ack");
	}
	//send data
	err = libusb_bulk_transfer(handle, BULK_EP_OUT, (uint8_t*)dat, len, &actual_length, 0);
	if(err || (actual_length!=len)) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");
}
void read_mem(libusb_device_handle *handle,uint32_t addr, uint8_t *dat, uint8_t len){
	uint8_t cmd[8];
	cmd[0]=CMD_READ;
	cmd[1]=len>>16;
	cmd[2]=len>>8;
	cmd[3]=len;
	cmd[4]=addr>>24;
	cmd[5]=addr>>16;
	cmd[6]=addr>>8;
	cmd[7]=addr;
	int actual_length;
	//send cmd
	err = libusb_bulk_transfer(handle, BULK_EP_OUT, cmd, sizeof(cmd), &actual_length, 0);
	if(err || (actual_length!=sizeof(cmd))) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");
	//get ack
	uint8_t ack[2];
	err = libusb_bulk_transfer(handle, BULK_EP_IN, ack, 2, &actual_length, 0);
	if(err) throw new std::string("ERROR: libusb_bulk_transfer() IN failed");
	if(ack[0]!=cmd[0]){
		printf("%d %02X %02X\n",actual_length,ack[0],ack[1]);
		throw new std::string("ERROR: unexpected ack");
	}
	//read data
	err = libusb_bulk_transfer(handle, BULK_EP_IN, (uint8_t*)dat, len, &actual_length, 0);
	if(err || (actual_length!=len)) throw new std::string("ERROR: libusb_bulk_transfer() OUT failed");

	uint8_t i;
	
	for(i=0;i<len;i++){
		if(dat[i]!=membuf[addr+i]){
			printf("ERROR: unexpected read value at offset %d, expected %02X, got %02X\n",i,membuf[addr+i],dat[i]);
		} else {
			printf("%02X ",dat[i]);
		}
	}
	printf("\n");
}
void usbmonTest(libusb_device_handle *handle){
	uint8_t dat[256];
	for(int i=0;i<8;i++) dat[i]=i;
	write_mem(handle,0,dat,8);
	read_mem(handle,0,dat,8);
}


int usb_find_all(void){
    libusb_device **devs;
	libusb_device *dev;
    int count = 0;
    int i = 0;
	libusb_init(NULL);
    if (libusb_get_device_list(NULL, &devs) < 0){
        throw new std::string("beurk");
		//ERROR: libusb_get_device_list() failed");
	}
    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;

        if (libusb_get_device_descriptor(dev, &desc) < 0)
            throw new std::string("ERROR: libusb_get_device_descriptor() failed");
//desc.idVendor == vendor && desc.idProduct == product
        if (desc.idVendor == IDVENDOR && desc.idProduct == IDPRODUCT){
			char manufacturer[1024];
            char description[1024];
            char serial[1024];
            libusb_ref_device(dev);
			usb_get_strings(dev, manufacturer, sizeof(manufacturer),description,sizeof(description),serial,sizeof(serial));
			printf("manufacturer: *%s*\n",manufacturer);
            printf("description: *%s*\n",description);
            printf("serial: *%s*\n",serial);
			
			printf("device speed code: 0x%08X\n",libusb_get_device_speed(dev)); 

			libusb_device_handle *handle;
			err = libusb_open(dev, &handle);	
			if(err) throw new std::string("ERROR: libusb_open() failed");
			
			
			printf("libusb_open() OK\n");
			
			//readBenchmark(handle);
			usbmonTest(handle);
			
			printf("libusb_close()\n");
			libusb_close(handle);
            count++;
        }
    }
    libusb_free_device_list(devs,1);
	libusb_exit(NULL);
	return count;
}


int main(int argc, char **argv){
	try{
	int dev_count = usb_find_all();
	printf("found %d devices\n",dev_count);
	}catch(std::string *s){
		printf("%s\nerr=0x%08X\n",s->c_str(),err);
	}
	printf("press enter to exit\n");
	getchar();
	return 0;
}

/*


#include <stdio.h>    
#include <stdlib.h>    
#include <sys/types.h>    
#include <string.h>    
#include </usr/local/include/libusb-1.0/libusb.h>    



int interface_ref = 0;    
int alt_interface,interface_number;    

int print_configuration(struct libusb_device_handle *hDevice,struct libusb_config_descriptor *config)    
{    
    char *data;    
    int index;    

    data = (char *)malloc(512);    
    memset(data,0,512);    

    index = config->iConfiguration;    

    libusb_get_string_descriptor_ascii(hDevice,index,(unsigned char *)data,512);    

    printf("\nInterface Descriptors: ");    
    printf("\n\tNumber of Interfaces : %d",config->bNumInterfaces);    
    printf("\n\tLength : %d",config->bLength);    
    printf("\n\tDesc_Type : %d",config->bDescriptorType);    
    printf("\n\tConfig_index : %d",config->iConfiguration);    
    printf("\n\tTotal length : %lu",config->wTotalLength);    
    printf("\n\tConfiguration Value  : %d",config->bConfigurationValue);    
    printf("\n\tConfiguration Attributes : %d",config->bmAttributes);    
    printf("\n\tMaxPower(mA) : %d\n",config->MaxPower);    

    free(data);    
    data = NULL;    
    return 0;    
}    

struct libusb_endpoint_descriptor* active_config(struct libusb_device *dev,struct libusb_device_handle *handle)    
{    
    struct libusb_device_handle *hDevice_req;    
    struct libusb_config_descriptor *config;    
    struct libusb_endpoint_descriptor *endpoint;    
    int altsetting_index,interface_index=0,ret_active;    
    int i,ret_print;    

    hDevice_req = handle;    

    ret_active = libusb_get_active_config_descriptor(dev,&config);    
    ret_print = print_configuration(hDevice_req,config);    

    for(interface_index=0;interface_index<config->bNumInterfaces;interface_index++)    
    {    
        const struct libusb_interface *iface = &config->interface[interface_index];    
        for(altsetting_index=0;altsetting_index<iface->num_altsetting;altsetting_index++)    
        {    
            const struct libusb_interface_descriptor *altsetting = &iface->altsetting[altsetting_index];    

            int endpoint_index;    
            for(endpoint_index=0;endpoint_index<altsetting->bNumEndpoints;endpoint_index++)    
            {    
                //const struct libusb_endpoint_desriptor *ep = (const struct libusb_endpoint_desriptor *)&altsetting->endpoint[endpoint_index];    
                //endpoint = (struct libusb_endpoint_desriptor *)ep;   
				struct libusb_endpoint_desriptor *ep = (struct libusb_endpoint_desriptor *)&altsetting->endpoint[endpoint_index];    
                endpoint =  ep;
                alt_interface = altsetting->bAlternateSetting;    
                interface_number = altsetting->bInterfaceNumber;    
            }    

            printf("\nEndPoint Descriptors: ");    
            printf("\n\tSize of EndPoint Descriptor : %d",endpoint->bLength);    
            printf("\n\tType of Descriptor : %d",endpoint->bDescriptorType);    
            printf("\n\tEndpoint Address : 0x0%x",endpoint->bEndpointAddress);    
            printf("\n\tMaximum Packet Size: %x",endpoint->wMaxPacketSize);    
            printf("\n\tAttributes applied to Endpoint: %d",endpoint->bmAttributes);    
            printf("\n\tInterval for Polling for data Tranfer : %d\n",endpoint->bInterval);    
        }    
    }    
    libusb_free_config_descriptor(NULL);    
    return endpoint;    
}    

int main(void)    
{    
    int r = 1;    
    struct libusb_device **devs;    
    struct libusb_device_handle *handle = NULL, *hDevice_expected = NULL;    
    struct libusb_device *dev,*dev_expected;    

    struct libusb_device_descriptor desc;    
    struct libusb_endpoint_descriptor *epdesc;    
    struct libusb_interface_descriptor *intdesc;    

    ssize_t cnt;    
    int e = 0,config2;    
    int i = 0,index;    
    char str1[64], str2[64];    
    char found = 0;    

// Init libusb     
    r = libusb_init(NULL);    
    if(r < 0)    
    {    
        printf("\nfailed to initialise libusb\n");    
        return 1;    
    }    
    else    
        printf("\nInit Successful!\n");    

// Get a list os USB devices    
    cnt = libusb_get_device_list(NULL, &devs);    
    if (cnt < 0)    
    {    
        printf("\nThere are no USB devices on bus\n");    
        return -1;    
    }    
    printf("\nDevice Count : %d\n-------------------------------\n",cnt);    

    while ((dev = devs[i++]) != NULL)    
    {    
        r = libusb_get_device_descriptor(dev, &desc);    
        if (r < 0)    
            {    
            printf("failed to get device descriptor\n");    
            libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            break;    
        }    

        e = libusb_open(dev,&handle);    
        if (e < 0)    
        {    
            printf("error opening device\n");    
            libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            break;    
        }    

        printf("\nDevice Descriptors: ");    
        printf("\n\tVendor ID : %x",desc.idVendor);    
        printf("\n\tProduct ID : %x",desc.idProduct);    
        printf("\n\tSerial Number : %x",desc.iSerialNumber);    
        printf("\n\tSize of Device Descriptor : %d",desc.bLength);    
        printf("\n\tType of Descriptor : %d",desc.bDescriptorType);    
        printf("\n\tUSB Specification Release Number : %d",desc.bcdUSB);    
        printf("\n\tDevice Release Number : %d",desc.bcdDevice);    
        printf("\n\tDevice Class : %d",desc.bDeviceClass);    
        printf("\n\tDevice Sub-Class : %d",desc.bDeviceSubClass);    
        printf("\n\tDevice Protocol : %d",desc.bDeviceProtocol);    
        printf("\n\tMax. Packet Size : %d",desc.bMaxPacketSize0);    
        printf("\n\tNo. of Configuraions : %d\n",desc.bNumConfigurations);    

        e = libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, (unsigned char*) str1, sizeof(str1));    
        if (e < 0)    
        {    
        libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            break;    
        }    
        printf("\nManufactured : %s",str1);    

        e = libusb_get_string_descriptor_ascii(handle, desc.iProduct, (unsigned char*) str2, sizeof(str2));    
        if(e < 0)    
        {    
        libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            break;    
        }    
        printf("\nProduct : %s",str2);    
        printf("\n----------------------------------------");    

        if(desc.idVendor == IDVENDOR && desc.idProduct == IDPRODUCT)    
        {    
        found = 1;    
        break;    
        }    
    }//end of while    
    if(found == 0)    
    {    
        printf("\nDevice NOT found\n");    
        libusb_free_device_list(devs,1);    
        libusb_close(handle);    
        return 1;    
    }    
    else    
    {    
        printf("\nDevice found");    
        dev_expected = dev;    
        hDevice_expected = handle;    
    }    

    e = libusb_get_configuration(handle,&config2);    
    if(e!=0)    
    {    
        printf("\n***Error in libusb_get_configuration\n");    
        libusb_free_device_list(devs,1);    
        libusb_close(handle);    
        return -1;    
    }    
    printf("\nConfigured value : %d",config2);    

    if(config2 != 1)    
    {    
        libusb_set_configuration(handle, 1);    
        if(e!=0)    
        {    
            printf("Error in libusb_set_configuration\n");    
            libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            return -1;    
        }    
        else    
            printf("\nDevice is in configured state!");    
    }    

    libusb_free_device_list(devs, 1);    

    if(libusb_kernel_driver_active(handle, 0) == 1)    
    {    
        printf("\nKernel Driver Active");    
        if(libusb_detach_kernel_driver(handle, 0) == 0)    
            printf("\nKernel Driver Detached!");    
        else    
        {    
            printf("\nCouldn't detach kernel driver!\n");    
            libusb_free_device_list(devs,1);    
            libusb_close(handle);    
            return -1;    
        }    
    }    

    e = libusb_claim_interface(handle, 0);    
    if(e < 0)    
    {    
        printf("\nCannot Claim Interface");    
        libusb_free_device_list(devs,1);    
        libusb_close(handle);    
        return -1;    
    }    
    else    
        printf("\nClaimed Interface\n");    

    active_config(dev_expected,hDevice_expected);    

    //   Communicate     

    char *my_string, *my_string1;    
    int transferred = 0;    
    int received = 0;    
    int length = 0;    

    my_string = (char *)malloc(nbytes + 1);    
    my_string1 = (char *)malloc(nbytes + 1);    

    memset(my_string,'\0',64);    
    memset(my_string1,'\0',64);    

    strcpy(my_string,"prasad divesd");    
    length = strlen(my_string);    

    printf("\nTo be sent : %s",my_string);    

    e = libusb_bulk_transfer(handle,BULK_EP_IN,my_string,length,&transferred,0);    
    if(e == 0 && transferred == length)    
    {    
        printf("\nWrite successful!");    
        printf("\nSent %d bytes with string: %s\n", transferred, my_string);    
    }    
    else    
        printf("\nError in write! e = %d and transferred = %d\n",e,transferred);    

    sleep(3);    
    i = 0;    

    for(i = 0; i < length; i++)    
    {    
        e = libusb_bulk_transfer(handle,BULK_EP_OUT,my_string1,64,&received,0);  //64 : Max Packet Lenght    
        if(e == 0)    
        {    
            printf("\nReceived: ");    
            printf("%c",my_string1[i]);    //will read a string from lcp2148
            sleep(1);    
        }    
        else    
        {    
            printf("\nError in read! e = %d and received = %d\n",e,received);    
            return -1;    
        }    
    }    


    e = libusb_release_interface(handle, 0);    

    libusb_close(handle);    
    libusb_exit(NULL);    

    printf("\n");    
    return 0;    
}  
*/
