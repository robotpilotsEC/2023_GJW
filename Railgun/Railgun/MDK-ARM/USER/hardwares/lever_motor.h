#ifndef __LEVER_MOTOR_H
#define __LEVER_MOTOR_H

#include "stm32f4xx_hal.h"
#include "motor.h"

/*CAN1*/
#define l_lever_motor_id 0x201
#define r_lever_motor_id 0x202
#define d_lever_motor_id 0x203

typedef enum
{
	L_LEVER = 0,
	R_LEVER ,
	D_LEVER ,
	
	LEVER_LIST
}lever_motor_list_e;

extern motor_t lever_motor[LEVER_LIST];
	
#endif


