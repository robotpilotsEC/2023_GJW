#include "my_task.h"
#include "cmsis_os.h"
#include "my_motor_control.h"
#include "drv_can.h"
#include "config_can.h"
#include "my_motor.h"
#include "drv_timer.h"

int16_t pwm_test = 0;

void Control_Task(void const * argument)
{
	
	SERVO_WEAK();
	
  for(;;)
  {
		SERVO_PwmOut(pwm_test);
		
		Motor_Heartbeat();
		
		My_Motor_Control();
		
//		can1_0x1FF_send_buff[motor_3508.id.buff_p] = 1500;
		
		CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
	  CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	  CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
		
    osDelay(1);
  }
}

