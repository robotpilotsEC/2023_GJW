#include "to_slave.h"
#include "drv_can.h"
#include "string.h"
#include "chassis.h"

extern CAN_HandleTypeDef hcan1;

int16_t slave_buf_1[4];//power_heat
uint8_t slave_buf_2[8];//game_robot_status
uint8_t slave_buf_3[8];//shoot_data
uint8_t slave_buf_4[8];//game_robot_pos
uint8_t slave_buf_5[8];//robot_hurt

void trans_to_slave_top_data(void)
{
	if(Chassis_Mode.chassis_spin_mode == C_S_top)
	{
		slave_buf_2[0] = 1;
	}
	else
	{
		slave_buf_2[0] = 0;
	}
//	CAN1_Send_With_int16_to_uint8(0x300, slave_buf_1);
	CAN_SendData(&hcan1, 0x300, slave_buf_2);
}

void slave_task(void)
{
	trans_to_slave_top_data();
}

