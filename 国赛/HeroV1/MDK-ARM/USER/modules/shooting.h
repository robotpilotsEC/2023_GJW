#ifndef __SHOOTING_H
#define __SHOOTING_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "car.h"

typedef enum
{
	S_Stop,//0
	S_Wake,//1
	S_Standby,//2
	S_Shoot,//3
	S_Done,//4
}shoot_work_status_e;

typedef enum
{
	F_reload,
	F_unload,
	F_static,
	F_f_stuck,
	F_b_stuck,
}feed_work_status_e;

typedef struct
{
	uint16_t feed_reload_angle;
	uint16_t feed_reload_times;
	uint16_t fric_speed_tar;
	uint16_t position_work_speed;
	uint16_t position_work_time;
}Shoot_config_t;

typedef struct
{
	float   angle_fix;
	uint8_t feed_work_command;
	uint8_t feed_work_status;
	uint8_t feed_work_times;
	uint8_t move_delay_cnt;
	uint8_t stuck_delay_cnt;
	uint8_t stuck_times_cnt;
	uint8_t f_stuck_delay_cnt;
	uint8_t f_stuck_times_cnt;
}Feed_work_info_t;

typedef struct
{
	Feed_work_info_t* feed_work_info;
	
	uint8_t shoot_work_command;
	uint8_t shoot_work_status;
	uint8_t standby_yes;
	uint16_t shoot_work_cnt;
}shoot_work_info_t;

typedef struct
{
	shoot_work_info_t* shoot_work_info;
	
	uint16_t feed_angle_sum_measure;
	float feed_angle_target;
	float fric_speed_target;
	float position_speed_target;
	int16_t client_speed_modify;
	int16_t clear_heat_num;
	bool clear_heat_flag;
}Shoot_info_t;

typedef struct Shoot_All_t
{
	Shoot_info_t* info;
	Shoot_config_t* config;
	
	motor_t*  feed_shoot;
	motor_t*  position_shoot;
	motor_t*  left_friction_shoot;
	motor_t*  right_friction_shoot;
	
	void (*feed_ctrl)(struct Shoot_All_t *shoot);
	void (*position_ctrl)(struct Shoot_All_t *shoot);
	void (*l_fric_ctrl)(struct Shoot_All_t *shoot);
	void (*r_fric_ctrl)(struct Shoot_All_t *shoot);
}Shoot_t;

extern Shoot_t Shoot;

void Shooting_Send(void);
void Shoot_Work_Init(Shoot_t* shoot);
void Feed_Work_Init(Shoot_t* shoot);
void Feed_Ctrl(Shoot_t* shoot);
void Position_Ctrl(Shoot_t* shoot);
void Right_Friction_Ctrl(Shoot_t* shoot);
void Left_Friction_Ctrl(Shoot_t* shoot);
void Shooting_Init_All(void);
void Shooting_Heartbeat(void);
void Shooting_Test(void);
void Feed_Stay_Static(Shoot_t* shoot);
void Shoot_speed_ctrl(Shoot_t* shoot);

#endif


