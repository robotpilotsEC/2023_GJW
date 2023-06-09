#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stm32f4xx_hal.h"
#include "rp_chassis.h"

#define CHAS_FAST_BUFF    5
#define CHAS_MID_BUFF     3
#define CHAS_SLOW_BUFF    2

typedef enum
{
	C_M_sleep,
	C_M_stop,
	C_M_normal,
	C_M_special,
}chassis_move_mode_e;

typedef enum
{
	C_S_sleep,
	C_S_stop,
	C_S_normal,
	C_S_follow,
	C_S_top,
}chassis_spin_mode_e;

typedef struct
{
	uint8_t chassis_move_mode;
	
	uint8_t chassis_spin_mode;
}Chassis_Mode_t;

typedef struct
{
	uint16_t top_speed_level_0;
	
	uint16_t top_speed_level_1;
	
	uint16_t top_speed_level_2;
	
	int16_t speed_max;
	
	uint8_t top_mode;
	
	uint16_t top_period;
}Chassis_Config_Info_t;

typedef struct 
{
	int16_t ch3_now;
  int16_t ch2_now;
  int16_t ch3_last;
  int16_t ch2_last;
	int16_t ch3_process;
	int16_t ch2_process;
}Chassis_Channel_Input_t;

typedef struct
{
	uint16_t spin_cnt;
}Chassis_Info_t;

extern Chassis_Mode_t Chassis_Mode;
extern chassis_t Chassis;

void Chassis_Init_All(void);
void Chassis_Heartbeat(void);
void Chassis_Ctrl(Chassis_Mode_t* chassis_mode);
void Chassis_Mode_Init(Chassis_Mode_t* chassis_mode);
void Chassis_Command_Init(void);
void Chassis_Process(Chassis_Mode_t* chassis_mode);
float chassis_yaw_cal(void);
void rc_input_process(Chassis_Channel_Input_t* channal);

#endif

