#include "stepper.h"

volatile Stepper* steppers[4];
volatile int speed = 40;
volatile unsigned int steps = 0;
volatile int s = 0;
volatile char stepper_active = 0;

Stepper *stepper_init(Pin *step, Pin *direction, Pin *enable){
	Stepper *stepper = (Stepper * ) malloc( sizeof( Stepper ) );
	stepper->step = step;
	stepper->direction = direction;
	stepper->enable = enable;

	pin_low(step);
	pin_high(direction);
	pin_low(enable);

	pin_config_out(step);
	pin_config_out(direction);
	pin_config_out(enable);

	steppers[s] = stepper;
	s++;

	return stepper;
}


void stepper_start_frame(){
	cli();
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B

	OCR1A = 1;
    // turn on CTC mode:
	TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
 //   TCCR1B |= (1 << CS00);
   // TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
   // TIMSK1 |= (1 << OCIE1A);



//	TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode 
//	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt 
//	OCR1A   = speed; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64 
//	TCCR1B |= ((1 << CS10)) | (1 << CS11); // Start timer at Fcpu/64 
//	TCCR1B |= ((1 << CS00)); // Start timer at Fcpu/64 

	sei(); //  Enable global interrupts 


}

void stepper_step(Stepper *stepper){
	pin_high(stepper->step);
	pin_low(stepper->step);

}


void stepper_dir(Stepper *stepper,int dir){
	pin_set(stepper->direction,dir);
}


void set_speed( int s){
	speed = s;
	OCR1A   = speed; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64 
}

void steppers_enable(){
	cli();
	pin_low(steppers[0]->enable);
	pin_high(r_led);
	//TCCR1B |= ((1 << CS00)); // Start timer at Fcpu/64 
	TCCR1B |= ((1 << CS10)) | (1 << CS11); // Start timer at Fcpu/64 
	TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt 
	OCR1A = speed;
	stepper_active = 1;
	sei(); //  Enable global interrupts 
	printf("enb\n");
}

void steppers_disable(){
	cli();
	pin_high(steppers[0]->enable);
	pin_low(r_led);
	//TCCR1B &= ~((1 << CS00)); // Start timer at Fcpu/64 
	TCCR1B &= ~((1 << CS10)) | (1 << CS11); // Start timer at Fcpu/64 
	TIMSK1 &= ~(1 << OCIE1A); // Enable CTC interrupt 
	OCR1A = speed;
	stepper_active = 0;
	sei(); //  Enable global interrupts 
	printf("dis\n");
}



int i = 0;

ISR(TIMER1_COMPA_vect) { 
	if( motion_current->distance > 0 || hold_on == 1 ){

		if( (int)(a1_cycle + motion_current->motor[0].step) != (int)(a1_cycle)   ){
			pin_toggle(steppers[0]->step);
		}

		if( (int)(a2_cycle + motion_current->motor[1].step) != (int)(a2_cycle)   ){
			pin_toggle(steppers[1]->step);
		}

		if( (int)(a3_cycle + motion_current->motor[2].step) != (int)(a3_cycle)   ){
			pin_toggle(steppers[2]->step);
		}

			a1_cycle += motion_current->motor[0].step;
			a2_cycle += motion_current->motor[1].step;
			a3_cycle += motion_current->motor[2].step;

			motion_current->distance--;

	}else
	shift_motion();

}
