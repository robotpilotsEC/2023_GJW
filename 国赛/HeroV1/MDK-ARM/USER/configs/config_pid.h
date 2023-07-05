#ifndef __CONFIG_PID_H
#define __CONFIG_PID_H

#include "stm32f4xx_hal.h"
#include "pid.h"

extern pid_t feed_motor_pid_speed;
extern pid_t feed_motor_pid_angle;
extern pid_t friction_left_motor_pid_speed;
extern pid_t friction_right_motor_pid_speed;
extern pid_t position_motor_pid_speed;

#endif


