#include "my_motor.h"
#include "motor.h"
#include "config_can.h"


motor_t motor_3508 = 
{
	.id.drive_type = M_CAN1,
	.id.motor_type = RM3508,
	.id.rx_id = motor_3508_id,
		
	.init = motor_class_init,
};

motor_t  motor_6020= 
{
	.id.drive_type = M_CAN1,
	.id.motor_type = GM6020,
	.id.rx_id = motor_6020_id,
		
	.init = motor_class_init,
};

motor_t motor_2006 = 
{
	.id.drive_type = M_CAN2,
	.id.motor_type = RM2006,
	.id.rx_id = motor_2006_id ,
		
	.init = motor_class_init,
};


