#include "shooting_motor.h"
#include "config_pid.h"
#include "3508_motor.h"
#include "2006_motor.h"
#include "motor.h"
#include "config_can.h"

extern drv_can_t feed_motor_can_driver;
extern drv_can_t position_motor_can_driver;
extern drv_can_t friction_left_motor_can_driver;
extern drv_can_t friction_right_motor_can_driver;

motor_t feed_motor = 
{
	.id.drive_type = M_CAN1,
	.id.motor_type = RM3508,
	.id.rx_id = feed_motor_id,
		
	.init = motor_class_init,
};

motor_t position_motor = 
{
	.id.drive_type = M_CAN2,
	.id.motor_type = RM2006,
	.id.rx_id = position_motor_id ,
		
	.init = motor_class_init,
};

motor_t friction_left_motor = 
{
	.id.drive_type = M_CAN2,
	.id.motor_type = RM3508,
	.id.rx_id = friction_left_motor_id,
		
	.init = motor_class_init,
};

motor_t friction_right_motor = 
{
	.id.drive_type = M_CAN2,
	.id.motor_type = RM3508,
	.id.rx_id = friction_right_motor_id,
		
	.init = motor_class_init,
};

