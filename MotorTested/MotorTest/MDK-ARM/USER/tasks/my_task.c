#include "my_task.h"
#include "cmsis_os.h"
#include "my_motor_control.h"
#include "drv_can.h"
#include "config_can.h"

void Control_Task(void const * argument)
{
  for(;;)
  {
		Shooting_Heartbeat();
		
		My_Motor_Control();
		
		CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
	  CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	  CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
		
    osDelay(1);
  }
}

