#ifndef __CONFIG_SHOOTING_H
#define __CONFIG_SHOOTING_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "imu.h"
#include "car.h"
#include "ave_filter.h"

#define feed_reload_work_angle 33480
#define position_work_speed_init 5800//5800
#define friction_work_speed_init 5000//3000
#define friction_10ms_work_speed 3600
#define friction_16ms_work_speed 5380//5450//5250//tested speed 5480
#define shoot_perfect_speed      15

#define feed_work_reload_time 10
#define position_work_time_tick 240

#define resonable_err 500

#endif


