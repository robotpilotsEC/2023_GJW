//#include "3508_motor.h"
//#include "string.h"
//#include "config_status.h"
//#include "pid.h"
//#include "mathematics.h"
//#include "motor.h"
//#include "motor_def.h"

//extern uint8_t can1_tx_buf[16];
//extern uint8_t can2_tx_buf[16];
//extern CAN_HandleTypeDef hcan1;
//extern CAN_HandleTypeDef hcan2;


//void motor_3508_info_init(motor_3508_info_t *info)
//{
//	info->offline_cnt = 5;
//	info->offline_cnt_max = 5;
//	info->target_angle_sum = 0;
//	info->status = DEV_OFFLINE;
//}

//void motor_3508_init(motor_3508_t *motor)
//{
//	memset(motor->base_info,0,sizeof(motor_3508_base_info_t));
//	
//	motor_3508_info_init(motor->info);
//	
//	if(motor->pid_angle != NULL) pid_init(motor->pid_angle);
//	if(motor->pid_speed != NULL) pid_init(motor->pid_speed);
//}

//void motor_3508_update(motor_3508_t *motor,uint8_t *rxBuf)
//{
//	motor_3508_base_info_t *base_info = motor->base_info;
//	int16_t last_angle = base_info->angle;
//	
//	base_info->angle = rxBuf[0];
//	base_info->angle <<= 8;
//	base_info->angle |= rxBuf[1];
//	base_info->speed = rxBuf[2];
//	base_info->speed <<= 8;
//	base_info->speed |= rxBuf[3];
//	base_info->current = rxBuf[4];
//	base_info->current <<= 8;
//	base_info->current |= rxBuf[5];
//	base_info->temperature = rxBuf[6];
//	
//	motor->info->angle_add = base_info->angle - last_angle;
//	
//	if(abs(motor->info->angle_add) > 4096)
//	{
//		if(motor->info->angle_add < 0)motor->info->angle_add += 8191;
//		else motor->info->angle_add -= 8191;
//	}
//	
//	motor->info->angle_sum += motor->info->angle_add;
//	//角度和需要处理
//	
//	motor->info->offline_cnt = 0;
//	motor->info->status = DEV_ONLINE;
//}
