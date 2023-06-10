#include "drv_all.h"
#include "drv_can.h"
#include "my_motor_control.h"


void Driver_Init(void)
{
  CAN1_Init();
	
  CAN2_Init();
	
}

void Device_Init(void)
{
	My_Motor_Init();
}

