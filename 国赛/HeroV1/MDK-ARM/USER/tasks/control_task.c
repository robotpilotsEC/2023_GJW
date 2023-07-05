#include "control_task.h"



//底盘左前电机又隐患，会掉线2022/11/26
//8000转底盘打滑严重
bool shoot_stop_ok = false;
extern IWDG_HandleTypeDef hiwdg;

uint32_t cnt_test = 0;
void control_task(void const * argument)
{
	static int16_t cnt = 0;
	static int16_t shoot_stop_cnt = 0;
	
	car_init(&Car);
	
  for(;;)
  {
		rc_ctrl(&rc);
		
		imu_update(&imu);
		
		Gimbal.imu_update(&Gimbal);
		
		car_ctrl(&Car);
		
		Cal_Pulse();
		
		#ifndef VISION_OFF
		if(cnt > 5)
		{
			vision_transmit_task(&vision);
			if(vision.base_info->gyro_found == 2 && vision.base_info->detect_mode == 2)
		  {
			  cnt_test++;
		  }
			cnt = 0;
		}
		else
		{
			cnt++;
		}
		#endif
		
		if(rc.info->status == DEV_ONLINE)
		{
			if(rc_init_flag == false)
			{
				rc_init_check();
			}
			
			if(rc_init_flag == true)
			{
				shoot_stop_ok = false;
				
				Shooting_Test();
			
        Gimbal_Ctrl(&Gimbal);
			
			  Chassis_Ctrl(&Chassis_Mode);
			
			  CAN1_Send_With_int16_to_uint8(0x1FF,can1_0x1FF_send_buff);
		    CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
			  CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	      CAN2_Send_With_int16_to_uint8(0x200,can2_0x200_send_buff);
			}
			else
			{
				Car_Sleep();
			}
		}
		else
		{
			if(shoot_stop_cnt >= 120 || shoot_stop_ok == true)
				{
					Car_Sleep();
					shoot_stop_ok = true;
					shoot_stop_cnt = 0;
				}
				else
				{
					shoot_stop_cnt++;
					Car_forced_to_stop();
					
				}
		}
		
		HAL_IWDG_Refresh(&hiwdg);
		
    osDelay(1);
  }
}


void realtime_task(void const * argument)
{
  for(;;)
  {
		vision_tick_task(&vision);
		
		judge_realtime_task(&judge);
		
		cap_send();
		
    osDelay(1);
  }
}

