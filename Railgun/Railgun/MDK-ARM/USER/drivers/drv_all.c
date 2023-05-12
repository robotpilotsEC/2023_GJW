#include "drv_all.h"


void driver_init(void)
{
	CAN1_Init();
  CAN2_Init();
  USART1_Init();
  USART2_Init();
  USART3_Init();
  USART4_Init();
  USART5_Init();
	PWM_Init();
}

void device_init(void)
{
	lever_init();
	rc_init(&rc);
	chassis_init();
	launcher_init();
}

