#include "console.h"
#include "math.h"
#include <string.h>
#include "lib/utils/array_length.h"
#include "lib/position/position.h"
#include "dev/stepper/stepper.h"
#include <util/delay.h>

static FILE mystdout = FDEV_SETUP_STREAM(uart_put_printf, NULL, _FDEV_SETUP_WRITE);


void disp_temp();
void echo();
void control_heat();
void position();
void drop();
void rise();
void get_sense();
//void test();
void aprox();
void home();
void sound();

Program programs[] = {
{"temp",disp_temp},
{"echo",echo},
{"heat",control_heat},
{"G",position},
{"drop",drop},
{"rise",rise},
{"get_sense",get_sense},
//{"test",test},
{"aprox",aprox},
{"home",home},
{"sound",sound}
};

void console_init(){
	stdout = &mystdout;

	printf("-= deltabot v0.1 =-\n");
	printf("Console Started.\n");

}



	static char args[5][16];
void console_parse(){

	//clean array
	for(int i = 0; i < ARRAY_LENGTH(args); i++)
		for(int j = 0; j < ARRAY_LENGTH(args[0]); j++)
			args[i][j] = 0;

	int i = 0;
	int arg = 0;
	char temp;

	while(1){
		temp = uart_get();
		if(temp == 0){
			break;
		}else
		if(temp == ' '){
			arg++;
			i=0;
		}else{
			args[arg][i] = temp;
			i++;
		}
	}
	
	for(int i = 0; i < ARRAY_LENGTH(programs); i++){
		if( strcmp (programs[i].name, args[0]) == 0 ){
			programs[i].function();
			return;
		}
	}
	
	printf("Command not found!\n");
}

void console_prompt(){
	while(1)
	console_interp();
}

void console_interp(){

	uart_write(">");
	while(!uart_new_line());
	console_parse();

//	uart_write("Got stuff\r\n");


	//uart_get();
}


void disp_temp(){
//	printf("temp=%0.2f\n",thermistor_get_c());
}

void echo(){
	printf("echo [%s] [%s]\n",args[0],args[1]);
}

void control_heat(){
	if(args[1][0] == 0)
	printf("heat requires one argument\n");
	else if( strcmp(args[1],"on") == 0)
	pin_high(e_heat);
	else if( strcmp(args[1],"off") == 0)
	pin_low(e_heat);
	else
	printf("error!\n");
}

void position(){
//	x = atof(args[1]);
//	y = atof(args[2]);
//	z = atof(args[3]);

//	calc_position();

}


void get_sense(){

printf("sense_rx=%i",pin_get(sense_rx));

}

void aprox(){
	long double distance = atof(args[1]);
	double a1 = atof(args[2]);
	double a2 = atof(args[3]);
	double a3 = atof(args[4]);

	while(motion_is_full()){
		printf("x");
	}

	printf("%lf %f %f %f",distance,a1,a2,a3);
	queue_motion(distance, a1, a2, a3);

}


void sound(){

	for(unsigned int i = 0; i < 60000; i++){
		pin_toggle(buzzer);
		_delay_us(1);
	}

		_delay_ms(500);
	for(unsigned int i = 0; i < 60000; i++){
		pin_toggle(buzzer);
		_delay_us(1);
	}
		_delay_ms(500);

	for(unsigned int i = 0; i < 60000; i++){
		pin_toggle(buzzer);
		_delay_us(1);
	}
		_delay_ms(500);

}





void rise(){

	motion_current->distance = 0;
	motion_current->motor[0].dir = 0;
	motion_current->motor[1].dir = 0;
	motion_current->motor[2].dir = 0;
	pin_set(steppers[0]->direction,	motion_current->motor[0].dir); 
	pin_set(steppers[1]->direction,	motion_current->motor[1].dir); 
	pin_set(steppers[2]->direction,	motion_current->motor[2].dir); 
	motion_current->motor[0].step = 1;
	motion_current->motor[1].step = 1;
	motion_current->motor[2].step = 1;

	printf("rising..\n");
	hold_on = 1;

	steppers_enable();

	while(1){

	if(pin_get(x_stop) == 0)
		motion_current->motor[0].step = 0;
	if(pin_get(y_stop) == 0)
		motion_current->motor[1].step = 0;
	if(pin_get(z_stop) == 0)
		motion_current->motor[2].step = 0;

	if( motion_current->motor[0].step==0 && motion_current->motor[1].step==0 && motion_current->motor[2].step==0 )
		break;

	}
	hold_on = 0;


	steppers_disable();
}

void drop(){
	motion_current->distance = 0;
	motion_current->motor[0].dir = 1;
	motion_current->motor[1].dir = 1;
	motion_current->motor[2].dir = 1;
	pin_set(steppers[0]->direction,	motion_current->motor[0].dir); 
	pin_set(steppers[1]->direction,	motion_current->motor[1].dir); 
	pin_set(steppers[2]->direction,	motion_current->motor[2].dir); 
	motion_current->motor[0].step = 1;
	motion_current->motor[1].step = 1;
	motion_current->motor[2].step = 1;

	printf("dropping..\n");
	hold_on = 1;
	steppers_enable();
	while(1){

	if(pin_get(sense_rx) == 1)
		break;

	}
	hold_on = 0;


	steppers_disable();
}



void home(){
	rise();
	drop();
}



