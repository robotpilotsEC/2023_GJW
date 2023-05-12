#include "chassis_motor.h"
#include "motor.h"

motor_t chassis_motor[CHASSIS_LIST] = 
{
	[LF_WHEEL] = 
	{
		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id = lf_motor_id,
		
		.init = motor_class_init,
	},
	[RF_WHEEL] = 
	{
		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id = rf_motor_id,
		
		.init = motor_class_init,
	},
	[LB_WHEEL] = 
	{
		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id = lb_motor_id,
		
		.init = motor_class_init,
	},
	[RB_WHEEL] = 
	{
		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id = rb_motor_id,
		
		.init = motor_class_init,
	},
};

	







