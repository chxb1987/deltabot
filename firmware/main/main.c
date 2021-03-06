#include "etc/byte.h"
#include "etc/config.h"
#include "sys/port.h"
#include "dev/stepper/stepper.h"
#include <util/delay.h>
#include "lib/position/position.h"


int main(void){
pin_config_out(r_led);

Stepper *stepper_x = stepper_init(x_step,x_dir,motor_enb);
Stepper *stepper_y = stepper_init(y_step,y_dir,motor_enb);
Stepper *stepper_z = stepper_init(z_step,z_dir,motor_enb);
//Stepper *stepper_e = stepper_init(e_step,e_dir,motor_enb);

stepper_dir(stepper_x,1);
stepper_dir(stepper_y,1);
stepper_dir(stepper_z,1);


a1_step = 180;
a2_step = 180;
a3_step = 180;
//distance = 18000*20;
stepper_start_frame();

	while(1);
}
