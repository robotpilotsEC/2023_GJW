#include "control_task.h"
#include "cmsis_os.h"
#include "config_can.h"
#include "drv_can.h"
#include "remote.h"
#include "launcher.h"
#include "string.h"
#include "config_all.h"
#include "chassis.h"
#include "drv_timer.h"
#include "railgun.h"

int16_t test_pwm = 0;

void Control_Task(void const * argument)
{
	/*Servo_init*/
  SERVO_WEAK();
	
  for(;;)
  {
		/*整车状态更新*/
		rail_status_update(&RAILGUN);
		
		if(rc.info->status == DEV_ON)
		{
			rc_ctrl(&rc);
			
		  launcher();
		  lever();
			chassis();
		}
    else
		{
			rail_sleep();
		}
		
		CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
		CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
		CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
    osDelay(1);
  }
}

void Realtime_Task(void const * argument)
{
  /* USER CODE BEGIN Realtime_Task */
  /* Infinite loop */
  for(;;)
  {
		rc_tick_task(&rc);
		lever_heartbeat();
    launcher_heartbeat();		
		chassis_heartbeat();
    osDelay(1);
  }
  /* USER CODE END Realtime_Task */
}

int16_t rail_sleep_buff[4] = {0,0,0,0};

void rail_sleep(void)
{
	memcpy(can1_0x200_send_buff,rail_sleep_buff,sizeof(can1_0x200_send_buff));
	memcpy(can1_0x1FF_send_buff,rail_sleep_buff,sizeof(can1_0x1FF_send_buff));
	memcpy(can2_0x200_send_buff,rail_sleep_buff,sizeof(can2_0x200_send_buff));
	memcpy(can2_0x1FF_send_buff,rail_sleep_buff,sizeof(can2_0x1FF_send_buff));
	
	/*Servo_Sleep*/
  servo_sleep();
}

