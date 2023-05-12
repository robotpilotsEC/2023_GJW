#include "config_can.h"
#include "shooting_motor.h"
#include "chassis_motor.h"
#include "gimbal_motor.h"
#include "config_status.h"
#include "string.h"
#include "judge.h"
#include "drv_haltick.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x1FF_send_buff[4];

float data_collect[500];


/*test*/
uint16_t test_last = 0;
uint16_t test_now = 0;
uint32_t time2 = 0;

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case feed_motor_id:
			feed_motor.rx(&feed_motor,rxBuf,8);
		  break;
		case chassis_motor_LF_id:
			chassis_motor[LF].rx(&chassis_motor[LF],rxBuf,8);
		  break;
		case chassis_motor_RF_id:
			chassis_motor[RF].rx(&chassis_motor[RF],rxBuf,8);
		  break;
		case chassis_motor_LB_id:
			chassis_motor[LB].rx(&chassis_motor[LB],rxBuf,8);
		  break;
		case chassis_motor_RB_id:
			chassis_motor[RB].rx(&chassis_motor[RB],rxBuf,8);
		  break;
		case gimbal_motor_yaw_id:
			Gim_Yaw.rx(&Gim_Yaw,rxBuf,8);
		  break;
		case power_heat_data:
			memcpy(&judge.base_info->chassis_power_buffer,&rxBuf[0],2);
		  memcpy(&judge.base_info->shooter_cooling_heat,&rxBuf[2],2);
		  memcpy(&judge.base_info->rfid,&rxBuf[4],1);
		  judge.info->offline_cnt = 0;
		  judge.info->status = DEV_ONLINE;
			break;
		case game_robot_status:
			memcpy(&judge.base_info->shooter_cooling_limit,&rxBuf[0],2);
		  memcpy(&judge.base_info->car_color,&rxBuf[2],1);
		  memcpy(&judge.base_info->chassis_power_limit,&rxBuf[3],2);
		  memcpy(&judge.base_info->shooter_id1_42mm_speed_limit,&rxBuf[5],2);
		  judge.info->offline_cnt = 0;
		  judge.info->status = DEV_ONLINE;
			break;
		case shoot_data:
			memcpy(&judge.base_info->shooting_speed,&rxBuf[0],4);
		  /*测试代码*/
		  if(test_now != judge.base_info->shooting_speed)
			{
				time2 = micros();
				test_now = judge.base_info->shooting_speed;
			}
		  break;
		case game_robot_pos:
			memcpy(&judge.base_info->gimbal_yaw_angle,&rxBuf[0],1);
		  judge.info->offline_cnt = 0;
		  judge.info->status = DEV_ONLINE;
			break;
		case robot_hurt:
			memcpy(&judge.base_info->hurt_type,&rxBuf[0],4);
	  	judge.info->offline_cnt = 0;
		  judge.info->status = DEV_ONLINE;
		  break;
		default:
			break;
	}
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case position_motor_id:
			position_motor.rx(&position_motor,rxBuf,8);
		  break;
		case friction_left_motor_id:
			friction_left_motor.rx(&friction_left_motor,rxBuf,8);
		  break;
		case friction_right_motor_id:
			friction_right_motor.rx(&friction_right_motor,rxBuf,8);
		  break;
		case gimbal_motor_pitch_id:
			Gim_Pitch.rx(&Gim_Pitch,rxBuf,8);
		  break;
	}
}

