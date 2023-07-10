#include "drv_all.h"
#include "drv_can.h"
#include "my_motor_control.h"
#include "drv_timer.h"


void Driver_Init(void)
{
  CAN1_Init();
	
  CAN2_Init();
	
	PWM_Init();
}

void Device_Init(void)
{
	My_Motor_Init();
}

