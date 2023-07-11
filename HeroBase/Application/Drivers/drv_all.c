#include "drv_all.h"
#include "drv_can.h"
#include "drv_uart.h"
#include "remote.h"

void Driver_Init(void)
{
  CAN1_Init();
	
  CAN2_Init();
	
	USART1_Init();
	
	USART2_Init();
	
	USART3_Init();
	
	USART4_Init();
	
	USART5_Init();
}

void Device_Init(void)
{
  rc_init(&rc);
}

