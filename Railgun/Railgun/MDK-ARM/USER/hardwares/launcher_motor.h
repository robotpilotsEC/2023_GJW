#ifndef __LAUNCHER_MOTOR_H
#define __LAUNCHER_MOTOR_H

#include "stm32f4xx_hal.h"
#include "motor.h"

/*CAN1*/
#define main_launcher_motor_id   0x204
#define feeder_motor_id          0x205

typedef enum
{
	MAIN_LAUNCHER = 0,
	
	FEEDER,
	
	LAUNCHER_LIST
}launcher_motor_list_e;

extern motor_t launcher_motor[LAUNCHER_LIST];


#endif




