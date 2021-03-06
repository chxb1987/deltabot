//serial.c
#include "serial.h"

/*
Shift_register shift_register_new(byte data, Pin *serial, Pin *clk, Pin *latch){
	Shift_register shift_register = {data,serial,clk,latch};
	pin_config_out(serial);
	pin_config_out(clk);
	pin_config_out(latch);
	return shift_register;
}
*/


void serial_out(Serial *serial,byte data){
	for(int i = 7; i >= 0; i--){
		pin_set(serial->data,data>>i&0x01);
		pin_pulse(serial->clk);	
	}
}
