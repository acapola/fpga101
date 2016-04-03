#ifndef __SFR_READ_CMD_H__
#define __SFR_READ_CMD_H__

#include <stdint.h>
uint8_t SFR_READ(uint8_t sfr_addr);
uint8_t SFR_WRITE(uint8_t sfr_addr, uint8_t value);
uint8_t sfr_cmd_invalid_header(uint8_t* combuf,uint8_t unit){
	if(combuf[3]%unit) return 1;//need an integer number of instruction in the command
	if(combuf[1]|combuf[2]) return 1;//max len is 254
	return 0;
}

void send_nack(uint8_t *combuf, uint8_t err_code){
	combuf[0] = ~combuf[0];
	combuf[1] = err_code;
	usb_send(combuf,2);//send nack	
}

void sfr_read_cmd(uint32_t* combuf32){
	uint8_t i;
	uint8_t *combuf = combuf32;
	// Save the SFRPAGE
	uint8_t SFRPAGE_save = SFRPAGE;
	uint8_t len = combuf[3];
	if(sfr_cmd_invalid_header(combuf,2)){
		send_nack(combuf,1);
		return;
	}
	
	for(i=0;i<len;i+=2){
		uint8_t sfrpage = combuf[4+i];
		if((sfrpage & 0x0F) || (sfrpage>0x20)){
			send_nack(combuf,i);
			return;
		}
		SFRPAGE = sfrpage;
		combuf[1+(i>>1)] = SFR_READ(combuf[4+i+1]);
	}
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;	
	usb_send(combuf,1+(len>>1));//send ack and data
}

void sfr_write_cmd(uint32_t* combuf32){
	uint8_t i;
	uint8_t *combuf = combuf32;
	// Save the SFRPAGE
	uint8_t SFRPAGE_save = SFRPAGE;
	uint8_t len = combuf[3];
	if(sfr_cmd_invalid_header(combuf,3)){
		send_nack(combuf,1);
		return;
	}
	
	for(i=0;i<len;i+=3){
		uint8_t sfrpage = combuf[4+i];
		if((sfrpage & 0x0F) || (sfrpage>0x20)){
			send_nack(combuf,i);
			return;
		}
		SFRPAGE = sfrpage;
		//CRC0IN = combuf[4+i+2];
		SFR_WRITE(combuf[4+i+1], combuf[4+i+2]);
	}
	// Restore the SFRPAGE
	SFRPAGE = SFRPAGE_save;	
	usb_send(combuf,1);//send ack
}

#endif //__SFR)READ_CMD_H__ 