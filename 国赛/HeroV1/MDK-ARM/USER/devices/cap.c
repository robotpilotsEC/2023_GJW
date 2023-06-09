#include "cap.h"
#include "drv_can.h"
#include "chassis.h"
#include "vision.h"
#include "car.h"
#include "config_status.h"
#include "mathematics.h"

extern CAN_HandleTypeDef hcan1;

uint8_t txBuf[8];

void cap_send(void)
{
	/*发送开超电指令*/
	txBuf[0] = 1;
	
	
	/*发送小陀螺标志位*/
	if(Chassis_Mode.chassis_spin_mode == C_S_top)
	{
		txBuf[1] = 1;
	}
	else
	{
		txBuf[1] = 0;
	}
	
	/*发送视觉启用标志位*/
	if(vision.info->status == DEV_ONLINE)
	{
		if(gim_auto_on == true)
		{
			txBuf[2] = 1;
		}
		if(vision.base_info->detect_mode == 2)//优先级高于gim_auto_on
		{
			txBuf[2] = 2;
		}
		if(gim_auto_on == false && vision.base_info->detect_mode != 2)
		{
			txBuf[2] = 0;
		}
	}
	else
	{
		txBuf[2] = 0;
	}
	
	/*发送视觉mode*/
	if(vision.base_info->main_mode == 1)
	{
		txBuf[3] = 0;
	}
	else
	{
		txBuf[3] = 1;//RY_mode
	}
	
	/*发送摩擦轮标志位*/
	if(Shoot.left_friction_shoot->state.work_state == M_OFFLINE || Shoot.right_friction_shoot->state.work_state == M_OFFLINE)
	{
		txBuf[4] = 0;
	}
	else
	{
		if(abs(Shoot.left_friction_shoot->rx_info.speed) >= 1500 && abs(Shoot.right_friction_shoot->rx_info.speed) >= 1500)
		{
			txBuf[4] = 2;
		}
		else
		{
			txBuf[4] = 1;
		}
	}
	
	/*视觉检测数字*/
	txBuf[5] = vision.base_info->detect_number;
	
	/*摩擦轮实时转速*/
	txBuf[6] = (uint8_t)((int16_t)Shoot.info->fric_speed_target >> 8);
	
	txBuf[7] = (uint8_t)((int16_t)Shoot.info->fric_speed_target);
	
	CAN_SendData(&hcan1, 0x0FF, txBuf);
}





