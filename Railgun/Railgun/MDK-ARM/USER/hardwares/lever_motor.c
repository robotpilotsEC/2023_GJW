#include "lever_motor.h"
#include "motor.h"

motor_t lever_motor[LEVER_LIST] = 
{
	[L_LEVER] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM2006,
		.id.rx_id = l_lever_motor_id,
		
		.init = motor_class_init,
	},
	[R_LEVER] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM2006,
		.id.rx_id = r_lever_motor_id,
		
		.init = motor_class_init,
	},
	[D_LEVER] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM2006,
		.id.rx_id = d_lever_motor_id,
		
		.init = motor_class_init,
	}
};



