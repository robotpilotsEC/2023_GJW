#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "imu.h"
#include "car.h"
#include "ave_filter.h"

typedef enum
{
	G_Y_sleep,
	G_Y_init,
	G_Y_imu,
	G_Y_mec,
	G_Y_auto,
}gim_yaw_mode_e;

typedef enum
{
	G_P_sleep,
	G_P_init,
	G_P_imu,
	G_P_mec,
	G_P_auto,
}gim_pitch_mode_e;

typedef struct
{
	float yaw_angle_target;
	float pitch_angle_target;
	
	float yaw_angle_mec_measure;
	float pitch_angle_mec_measure;
	float yaw_angle_imu_measure;
	float pitch_angle_imu_measure;
	float yaw_speed_imu_measure;
	float pitch_speed_imu_measure;
	
	uint8_t yaw_mode;
	uint8_t pitch_mode;
	
}gimbal_info_t;

typedef struct gimbal_class_t
{
	gimbal_info_t*  info;
	
	motor_t*  yaw_gimbal;
	
	motor_t*  pitch_gimbal;
	
	
	void (*imu_update)(struct gimbal_class_t *gimbal);
	void (*yaw_mec_ctrl)(struct gimbal_class_t *gimbal);
	void (*pitch_mec_ctrl)(struct gimbal_class_t *gimbal);
	void (*yaw_imu_ctrl)(struct gimbal_class_t *gimbal);
	void (*pitch_imu_ctrl)(struct gimbal_class_t *gimbal);
	
}gimbal_t;

extern gimbal_t Gimbal;

void Gimbal_Imu_Update(gimbal_t* gimbal);
void Gimbal_Init_All(void);
void Gimbal_Heartbeat(void);
void Gimbal_Ctrl(gimbal_t* gimbal);
void Gimbal_Yaw_Mec_Ctrl(gimbal_t* gimbal);
void Gimbal_Pitch_Mec_Ctrl(gimbal_t* gimbal);
void Gimbal_Yaw_Imu_Ctrl(gimbal_t* gimbal);
void Gimbal_Pitch_Imu_Ctrl(gimbal_t* gimbal);
void Gimbal_Mode_Init(gimbal_t* gimbal);
void Gimbal_Command_Init(gimbal_t* gimbal);
void Gimbal_Pitch_Angle_Check(gimbal_t* gimbal);
void Gimbal_Yaw_Angle_Check(gimbal_t* gimbal);

#endif


