#ifndef SERIAL_H
#define SERIAL_H

#include "sys/port.h"
#include "lib/delay/delay.h"

typedef struct Serial{
	Pin *clk;
	Pin *data;
}Serial;

//Shift_register shift_register_new(byte data, Pin *serial, Pin *clk, Pin *latch);
//void shift_register_latch_output(Shift_register *shift_register);

void serial_out(Serial *serial,byte data);
//byte serial_in(Serial *serial);

#endif
