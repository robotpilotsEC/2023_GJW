#include "launcher_motor.h"

motor_t launcher_motor[LAUNCHER_LIST] = 
{
	[MAIN_LAUNCHER] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = main_launcher_motor_id,
		
		.init = motor_class_init,
	},
	[FEEDER] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = feeder_motor_id,
		
		.init = motor_class_init,
	}
};
