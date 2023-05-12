#ifndef __LAUNCHER_H
#define __LAUNCHER_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "stdbool.h"

#define LA_STUCK_MAX_CNT                30
#define LAUNCHER_MOION_RANGE            710000//698764
#define LAU_MAIN_LAUNCH_POS_INIT_SPEED  -500
#define LAU_FEEDER_POS_INIT_SPEED       2500
#define SERVO_LOCK_PWM                  200
#define SERVO_UNLOCK_PWM                75
#define FEEDER_HALF_BALL_POS            15565//need to be tested
#define FEEDER_ONE_BALL_POS             31129
#define SERVO_DELAY_TIME                700

typedef enum 
{
	LA_INIT = 0,
	LA_LOCK,
	LA_UNLOCK,
}launcher_status_e;

typedef enum
{
	speed_state = 0,
	position_state,
	foreward_state,
	backward_state,
}feeder_init_status_e;

typedef struct launcher_struct_info_t
{
	bool  main_launch_init_flag;
	bool  main_launch_stuck_flag;
	bool  feeder_init_flag;
	bool  feeder_stuck_flag;
	bool  launcher_init_flag;
	bool  feeder_reload_ok;
	bool  main_launcher_reload_ok;
	bool  main_launcher_reload_sup_ok;
	bool  fire_ready;
	bool  reload_processing;
	bool  launch_available;
	
	float main_launch_superior_limit;
	float main_launch_lower_limit;
	
	uint16_t main_launch_stuck_cnt;
	uint16_t main_launch_init_stuck_cnt;
	uint16_t feeder_stuck_cnt;
	uint16_t feeder_init_stuck_cnt;
	uint32_t servo_delay_cnt;
	
	uint8_t feeder_foreback_time;
	uint32_t feeder_state_in_cnt;
	uint8_t feeder_init_status;
	uint8_t feeder_moving_state;
	uint32_t main_launch_up_state_in_cnt;
	uint32_t main_launch_down_state_in_cnt;
	
	float main_launch_target;
	float feeder_target;
	float feeder_target_save;
}launcher_info_t;

typedef struct launcher_struct_t
{
	uint8_t status;
	
	launcher_info_t* info;
	
	motor_t* main_launch;
	
	motor_t* feeder;
	
	void (*main_launch_ctrl)(struct launcher_struct_t *launcher,float tar);
	
	void (*feeder_ctrl)(struct launcher_struct_t *launcher,float tar);
}launcher_t;

extern launcher_t Launcher;

void launcher_init(void);
void launcher_position_init(void);
void launcher_heartbeat(void);
void main_launcher_stuck_test(launcher_t* launch);
float main_launcher_position_lock(float tar);
void main_launcher_ctrl(launcher_t* launch,float tar);
void main_launch_ctrl(launcher_t* launch);
void launcher(void);
void feeder_ctrl(launcher_t* launch, float tar);
void servo_unlock(void);
void servo_lock(void);
void servo_sleep(void);
void feeder_foreback(launcher_t* launch);
bool feeder_stuck_test(launcher_t* launch);
void launcher_command_refresh(launcher_t* launcher);
void feed_ctrl(launcher_t* launch);

#endif







