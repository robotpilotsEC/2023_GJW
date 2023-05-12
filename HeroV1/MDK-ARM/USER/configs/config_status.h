#ifndef __CONFIG_STATUS_H
#define __CONFIG_STATUS_H

#include "stm32f4xx_hal.h"
#include "gimbal.h"
#include "shooting.h"
#include "chassis.h"
#include "remote.h"
#include "rp_chassis.h"
#include "string.h"
#include "config_can.h"

typedef enum DEVICE_WORK_STATE
{
	DEV_ONLINE = 1,
	DEV_OFFLINE = 0,
}Dev_Work_State;

typedef enum
{
	LF,
	RF,
	LB,
	RB,
}motor_num;

void Car_Sleep(void);
void Cal_Pulse(void);
void rc_init_check(void);
void Car_forced_to_stop(void);

#endif


