#ifndef __CAR_H
#define __CAR_H

#include "stm32f4xx_hal.h"
#include "remote.h"
#include "stdbool.h"
#include "vision.h"

typedef enum 
{
	sleep_car,
	init_car,
	mec_car,
	imu_car,
}car_move_mode_e;

typedef enum
{
	RC_CTRL,
	KEY_CTRL,
}car_ctrl_mode_e;

typedef struct
{
	uint8_t car_move_command;
	
	uint8_t car_move_status;
	
	uint8_t ctrl_mode;
	
}car_t;

extern bool top_car_on;
extern bool top_car_off;
extern bool top_switch;

extern bool shoot_start;
extern bool auto_shoot;
extern bool shoot_stop;
extern bool shoot_standby;
extern bool shoot_wake;
extern bool feed_reload;
extern bool feed_unload;
extern bool client_increase_speed;
extern bool client_decrease_speed;

extern bool gim_left_turn_90;
extern bool gim_right_turn_90;
extern bool gim_head_turn;
extern bool gim_auto_on;
extern bool gim_auto_off;

extern bool car_mode_change;

extern car_t Car;

void Rc_S2_Status_Check(car_t* car);
void Rc_S1_Status_Check(void);
void Rc_Thumbwheel_Check(car_t* car);
void car_command_init(void);
void Rc_Status_Scan(car_t* car);
void Key_Status_Scan(car_t* car);
void car_ctrl_mode_update(car_t* car);
void car_status_update(car_t* car);
void car_init(car_t* car);
void car_ctrl(car_t* car);

#endif

