#include "config_status.h"

uint8_t stop_buff[16];
bool rc_init_flag = false;

uint16_t rc_init_cnt = 0;

void Car_Sleep(void)
{
	memset(stop_buff,0,16);
	
	CAN_SendData(&hcan1,0x200,stop_buff);
	CAN_SendData(&hcan1,0x1FF,&stop_buff[8]);
	CAN_SendData(&hcan2,0x200,stop_buff);
	CAN_SendData(&hcan2,0x1FF,&stop_buff[8]);
	
	memset(stop_buff,0,16);
	
	rc_init_flag = false;
}

void Car_forced_to_stop(void)
{
	Shoot.info->fric_speed_target = 0;
	Shoot.l_fric_ctrl(&Shoot);
	Shoot.r_fric_ctrl(&Shoot);
//	Chassis.base_info.target.cycle_speed = 0;
//	Chassis.base_info.target.front_speed = 0;
//	Chassis.base_info.target.right_speed = 0;
//	Chassis.work(&Chassis);
	
//	CAN1_Send_With_int16_to_uint8(0x200,can1_0x200_send_buff);
	CAN2_Send_With_int16_to_uint8(0x1FF,can2_0x1FF_send_buff);
}

void Cal_Pulse(void)
{
	rc_tick_task(&rc);
	Chassis_Heartbeat();
	Gimbal_Heartbeat();
	Shooting_Heartbeat();
}



void rc_init_check(void)
{
	if(rc.base_info->ch0 == 0 && rc.base_info->ch1 == 0 \
		 && rc.base_info->ch2 == 0 && rc.base_info->ch3 == 0)
	{
		rc_init_cnt++;
		if(rc_init_cnt >= 200)
		{
			rc_init_flag = true;
		}
		else
		{
			rc_init_flag = false;
		}
	}
	else
	{
		rc_init_flag = false;
		rc_init_cnt = 0;
	}
}

