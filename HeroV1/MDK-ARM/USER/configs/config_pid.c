#include "config_pid.h"

pid_info_t feed_motor_pid_speed_info = 
{
	.kp = 0.0f,
	.out_max = 3000,
};

pid_info_t feed_motor_pid_angle_info = 
{
	.kp = 0.0f,
	.ki = 0.0f,
	.integral_max = 10000,
	.out_max = 0,
};

pid_info_t friction_left_motor_pid_speed_info = 
{
	.kp = 0,
	.ki = 0,
	.integral_max = 6000,
	.out_max = 10000,
};

pid_info_t friction_right_motor_pid_speed_info = 
{
	.kp = 0,
	.ki = 0,
	.integral_max = 6000,
	.out_max = 10000,
};

pid_info_t position_motor_pid_speed_info = 
{
	.kp = 0,
	.ki = 0,
	.integral_max = 10000,
	.out_max = 9000,
};





pid_t feed_motor_pid_speed = 
{
	.info = &feed_motor_pid_speed_info,
};

pid_t feed_motor_pid_angle = 
{
	.info = &feed_motor_pid_angle_info,
};

pid_t friction_left_motor_pid_speed = 
{
	.info = &friction_left_motor_pid_speed_info,
};

pid_t friction_right_motor_pid_speed = 
{
	.info = &friction_right_motor_pid_speed_info,
};

pid_t position_motor_pid_speed = 
{
	.info = &position_motor_pid_speed_info,
};

