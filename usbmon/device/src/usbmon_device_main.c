//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8UB1_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
// $[Generated Includes]
#include "efm8_usb.h"
// [Generated Includes]$
#include <stdint.h>

void Delay(void) {
	int16_t x;

	for (x = 0; x < 500; x) {
		x++;
	}
}
void Delay10ms(void) {
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
	Delay();
}
uint32_t xdata dataStorage[16];
volatile uint8_t tx_size;
volatile bool tx_done;
volatile USBD_State_TypeDef usbState;
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
void usbmon(void);

unsigned int blinkDelay = 65000;
unsigned int blinkCnt = 0;
void toggleLED(void){
	P0 = P0 ^ 0x80; //toggle P0.7
}
void blink(void) {
	unsigned int i;
	toggleLED();
	for (i = 0; i < blinkDelay; i++)
		_nop_();
}
void waitUsbConfigured(void) {
	while (usbState != USBD_STATE_CONFIGURED) {
		if (blinkCnt++ == blinkDelay) {
			blinkCnt = 0;
			toggleLED();

		}
		_nop_();		//slow down a bit
		_nop_();		//slow down a bit
	}
}
int main(void) {
	volatile uint32_t cnt = 0;
	uint8_t* buf = (uint8_t*) dataStorage;
	usbState = USBD_STATE_NONE;
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();
	dataStorage[0] = 0;
	dataStorage[1] = 1;
	tx_done = true;
	waitUsbConfigured();

	while (1) {
		usbmon();
	}
	while (1) {
		unsigned int i;
// $[Generated Run-time code]
// [Generated Run-time code]$
		for (i = 0; i < 2; i++)
			dataStorage[i] = cnt++;

		waitUsbConfigured();
		while (!tx_done) {
			if (blinkCnt++ == blinkDelay / 2) {
				blinkCnt = 0;
				toggleLED();
			}
		}

		tx_done = false;
		USB_DisableInts();
		USBD_Write(1, buf, 64, true);
		USB_EnableInts();
	}
}

uint8_t xdata membuf[256];
void write_mem(uint32_t addr, const uint8_t * const dat, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		membuf[addr + i] = dat[i];
	}
}
void read_mem(uint32_t addr, uint8_t *dat, uint8_t len) {
	uint8_t i;
	for (i = 0; i < len; i++) {
		dat[i] = membuf[addr + i];
	}
}
typedef enum {
	CMD_NOP = 0x00, CMD_READ = 0x01, CMD_WRITE = 0x02, CMD_TEST_UNKNOWN = 0xFF
};
void usb_receive(uint8_t*combuf, uint8_t len) {
	unsigned int status;
	tx_done = false;

	status = USBD_Read(2, combuf, len, true);
	while (!tx_done)
		;
	if (USB_STATUS_OK != status) {
		while (1)
			;
	}
}

void usb_send(const uint8_t* const combuf, uint8_t len) {
	tx_done = false;
	USB_DisableInts();
	USBD_Write(1, combuf, len, true);
	USB_EnableInts();
	while (!tx_done)
		;
}
void usbmon(void) {
	uint32_t* combuf32 = dataStorage;
	uint8_t* combuf = (uint8_t*) dataStorage;
	uint8_t cmd;
	uint32_t len;
	while (1) {
		tx_done = false;
		while (1) {
			unsigned int status;
			status = USBD_Read(2, combuf, 8, true);
			while (!tx_done)
				;		//wait irq
			if (usbState != USBD_STATE_CONFIGURED)
				return;
			if (USB_STATUS_OK == status)
				break;
		}
		cmd = combuf[0];
		len = 0;
		switch (cmd) {
		case CMD_NOP:
			usb_send(combuf, 1);		//do nothing, just send ack
			break;
		case CMD_WRITE: {
			uint32_t addr = combuf32[1];
			uint8_t readLen = 64;
			usb_send(combuf, 1);		//send ack
			len = combuf32[0] & 0x00FFFFFF;
			while (len) {
				if (len < readLen)
					readLen = len;
				usb_receive(combuf, readLen);
				write_mem(addr, combuf, readLen);
				len -= readLen;
			}
			break;
		}
		case CMD_READ: {
			uint32_t addr = combuf32[1];
			uint8_t readLen = 64;
			usb_send(combuf, 1);		//send ack
			len = combuf32[0] & 0x00FFFFFF;
			while (len) {
				if (len < readLen)
					readLen = len;
				read_mem(addr, combuf, readLen);
				usb_send(combuf, readLen);
				len -= readLen;
			}
			break;
		}
		case CMD_TEST_UNKNOWN:		//fall through
		default: //unknown command
			combuf[1] = combuf[0];
			combuf[0] = CMD_TEST_UNKNOWN;
			usb_send(combuf, 2);
		}
	}
}
