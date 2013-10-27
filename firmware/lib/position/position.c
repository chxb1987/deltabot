//serial.c
#include "position.h"
#include "../../dev/stepper/stepper.h"
#include <math.h>

#define D 130
#define r 30


volatile int x = 0;
volatile int y = 0;
volatile int z = 0;

volatile int a1 = 0;
volatile int a2 = 0;
volatile int a3 = 0;


volatile int a1_step = 0;
volatile int a2_step = 0;
volatile int a3_step = 0; 

volatile int a1_cycle = 0;
volatile int a2_cycle = 0;
volatile int a3_cycle = 0;


volatile int distance = 0;

void calc_position(){
	int a1_temp,a2_temp,a3_temp,a1_dist,a2_dist,a3_dist;
	
/*
	a1_temp = sqrt(pow((sqrt(3)*D)/3 - y + r ,2) + pow(x,2));
	a2_temp = sqrt(pow(D/2 + x - r/2,2) + pow( sqrt(3)*D/6 + y - sqrt(3)*r/2,2));
	a3_temp = sqrt(pow(D/2 - x + r/2,2) + pow( sqrt(3)*D/6 + y - sqrt(3)*r/2,2));
*/
/*
	printf("x:%d\n",x);
	printf("y:%d\n",y);
	printf("z:%d\n",z);

	printf("a1:%d\n",a1);
	printf("a2:%d\n",a2);
	printf("a3:%d\n",a3);
*/
	a1_dist = a1 - x;
	a2_dist = a2 - y;
	a3_dist = a3 - z;
/*
	printf("a1_dist:%d\n",a1_dist);
	printf("a2_dist:%d\n",a2_dist);
	printf("a3_dist:%d\n",a3_dist);
*/
	a1 = x;
	a2 = y;
	a3 = z;


	if(a1_dist > 0)
		pin_low(steppers[0]->direction);
	else
		pin_high(steppers[0]->direction);

	if(a2_dist > 0)
		pin_low(steppers[1]->direction);
	else
		pin_high(steppers[1]->direction);

	if(a3_dist > 0)
		pin_low(steppers[2]->direction);
	else
		pin_high(steppers[2]->direction);


	a1_cycle = 0;
	a2_cycle = 0;
	a3_cycle = 0;

	if(abs(a1_dist) > abs(a2_dist) && abs(a1_dist) > abs(a3_dist)){
		a1_step = 1;
		a2_step = abs(a2_dist/a1_dist);
		a3_step = abs(a3_dist/a1_dist);
//	printf("1:%i\n2_:%i\n3:%i\n",a1_step,a2_step,a3_step);
		distance = abs(a1_dist*2);
	}else
	if(abs(a2_dist) > abs(a1_dist) && abs(a2_dist) > abs(a3_dist)){
		a2_step = 1;
		a1_step = abs(a1_dist/a2_dist);
		a3_step = abs(a3_dist/a2_dist);
//	printf("1:%i\n2_:%i\n3:%i\n",a1_step,a2_step,a3_step);
		distance = abs(a2_dist*2);
	}else
	if(abs(a3_dist) > abs(a1_dist) && abs(a3_dist) > abs(a2_dist)){
		a3_step = 1;
		a1_step = abs(a1_dist/a3_dist);
		a2_step = abs(a2_dist/a3_dist);
//	printf("1:%i\n2:%i\n3:%i\n",a1_step,a2_step,a3_step);
		distance = abs(a3_dist*2);
	}

	printf("a1_step:%d\n",a1_step);
	printf("a2_step:%d\n",a2_step);
	printf("a3_step:%d\n",a3_step);
	printf("distance:%d\n",distance);



}