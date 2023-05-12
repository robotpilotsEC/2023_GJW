#include "chassis_motor.h"
#include "motor.h"
#include "config_status.h"
#include "config_can.h"

//#define chassis_motor_LF_id 0x201
//#define chassis_motor_RF_id 0x202
//#define chassis_motor_LB_id 0x203
//#define chassis_motor_RB_id 0x204

motor_t chassis_motor[4] = 
{
	[LF] =
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = chassis_motor_LF_id,
		
		.init = motor_class_init,
	},
	[LB] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = chassis_motor_LB_id,
		
		.init = motor_class_init,
	},
	[RF] =
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = chassis_motor_RF_id,
		
		.init = motor_class_init,
	},
	[RB] = 
	{
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id = chassis_motor_RB_id,
		
		.init = motor_class_init,
	},
};








