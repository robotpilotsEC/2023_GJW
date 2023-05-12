#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "stdbool.h"
#include "rp_chassis.h"

#define SHIFTING_MAX_SPEED  8000
#define HIGH_SPEED_BUFF     2
#define MIDDLE_SPEED_BUFF   1
#define LOW_SPEED_BUFF      0.3

typedef enum
{
	LOCK = 0,
	UNLOCK,
}chassis_status_e;

typedef struct rc_channel_input_struct_t
{
	int16_t ch3_now;
	int16_t ch3_last;
	int16_t ch3_process;
	int16_t ch2_now;
	int16_t ch2_last;
	int16_t ch2_process;
}rc_channel_input_t;

typedef struct wasd_channel_input_struct_t
{
	int16_t w_now;
	int16_t w_last;
	int16_t w_process;
	int16_t a_now;
	int16_t a_last;
	int16_t a_process;
	int16_t s_now;
	int16_t s_last;
	int16_t s_process;
	int16_t d_now;
	int16_t d_last;
	int16_t d_process;
}wasd_channel_input_t;

typedef struct chassis_struct_info_t
{
	float shifting_buff;
	
	rc_channel_input_t *rc_input;
	
	wasd_channel_input_t *wase_input;
}my_chassis_info_t;

typedef struct chassis_struct_t
{
	uint8_t status; //µ×ÅÌ×´Ì¬
	
	my_chassis_info_t* info;
	
	motor_t* lf_motor;
	motor_t* rf_motor;
	motor_t* lb_motor;
	motor_t* rb_motor;
	
	chassis_t* rp_pack;
	
}my_chassis_t;

void chassis_init(void);
void chassis_heartbeat(void);
void rc_input_process(rc_channel_input_t *rc_input);
void WASD_input_process(wasd_channel_input_t *wasd_input);
void chassis_ctrl(my_chassis_t* my_chassis);
void chassis(void);

#endif


