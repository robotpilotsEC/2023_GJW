#ifndef __MY_MOTOR_CONTROL_H
#define __MY_MOTOR_CONTROL_H

#include "stm32f4xx_hal.h"
#include "motor.h"

/*MODE_0:电机速度环  MODE_1:电机位置环*/
#define MODE_0
void My_Motor_Init(void);
void My_Motor_Control(void);
void Motor_Heartbeat(void);

#endif


