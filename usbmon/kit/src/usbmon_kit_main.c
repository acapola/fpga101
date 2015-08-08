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
volatile bool tx_done;
volatile USBD_State_TypeDef usbState;
//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main(void) {
	volatile uint32_t cnt = 0;
	uint8_t* buf = (uint8_t*) dataStorage;
	usbState = USBD_STATE_NONE;
	// Call hardware initialization routine
	enter_DefaultMode_from_RESET();
	/*
	 for (cnt = 0; cnt < 1000; cnt++) {
	 P1 = cnt;
	 Delay10ms();
	 }
	 P1 = 0xAA;
	 */
	//while(USB_STATUS_OK!=USBD_Read(2, buf, 8, false));
	dataStorage[0] = 0;
	dataStorage[1] = 1;
	tx_done=true;
	while (usbState != USBD_STATE_CONFIGURED);

	P1 += 0x10;

	while (1) {
		unsigned int i;
// $[Generated Run-time code]
// [Generated Run-time code]$
		//while(USB_STATUS_OK!=USBD_Read(2, buf, 8, false));
		for (i = 0; i < 2; i++)
			dataStorage[i] = cnt++;
		//P1 += 0x10;
		while (usbState != USBD_STATE_CONFIGURED)
			;
		while (!tx_done);

		tx_done = false;
		USB_DisableInts();
		USBD_Write(1, buf, 64, true);
		USB_EnableInts();
	}
}
