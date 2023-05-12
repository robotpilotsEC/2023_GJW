#include "gimbal_motor.h"
#include "motor.h"
#include "config_can.h"


motor_t Gim_Yaw = 
{
	.id.drive_type = M_CAN1,
	.id.motor_type = GM6020,
	.id.rx_id = gimbal_motor_yaw_id,
		
	.init = motor_class_init,
};

motor_t Gim_Pitch = 
{
	.id.drive_type = M_CAN2,
	.id.motor_type = GM6020,
	.id.rx_id = gimbal_motor_pitch_id,
		
	.init = motor_class_init,
};
