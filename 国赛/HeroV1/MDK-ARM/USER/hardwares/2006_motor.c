//#include "2006_motor.h"
//#include "string.h"
//#include "config_status.h"
//#include "pid.h"
//#include "mathematics.h"

//extern uint8_t can2_tx_buf[16];
//extern CAN_HandleTypeDef hcan2;

//void motor_2006_info_init(motor_2006_info_t *info)
//{
//	info->offline_cnt = 5;
//	info->offline_cnt_max = 5;
//	info->target_angle_sum = 0;
//	info->status = DEV_OFFLINE;
//}

//void motor_2006_init(motor_2006_t *motor)
//{
//	memset(motor->base_info,0,sizeof(motor_2006_base_info_t));
//	
//	motor_2006_info_init(motor->info);
//	
//	if(motor->pid_angle != NULL) pid_init(motor->pid_angle);
//	if(motor->pid_speed != NULL) pid_init(motor->pid_speed);
//}

//void motor_2006_update(motor_2006_t *motor,uint8_t *rxBuf)
//{
//	motor_2006_base_info_t *base_info = motor->base_info;
//	int16_t last_angle = base_info->angle;
//	
//	base_info->angle = rxBuf[0];
//	base_info->angle <<= 8;
//	base_info->angle |= rxBuf[1];
//	base_info->speed = rxBuf[2];
//	base_info->speed <<= 8;
//	base_info->speed |= rxBuf[3];
//	base_info->torque = rxBuf[4];
//	base_info->torque <<= 8;
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
//	
//	motor->info->offline_cnt = 0;
//	motor->info->status = DEV_ONLINE;
//}



