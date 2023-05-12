#ifndef __LEVER_H
#define __LEVER_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "lever_motor.h"
#include "stdbool.h"

#define DIFFER_PID_ON         0  //我原本想从宿舍到教室开战斗机
#define LE_STUCK_MAX_CNT      20
#define POS_INIT_SPEED        -3000
#define TWIN_MOION_RANGE      15541016//12820952//限多了
#define D_LEVER_MOTION_RANGE  6904438//需测试

typedef enum 
{
	L_LOCK = 0,
	L_UNLOCK,
	L_INIT,
}lever_status_e;

typedef struct
{
	bool lever_position_init_flag;
	bool twin_init_flag;
	bool d_lever_init_flag;
	bool twin_stuck_flag;
	bool d_lever_stuck_flag;
	
	float twin_superior_limit;
	float twin_lower_limit;
	float d_lever_superior_limit;
	float d_lever_lower_limit;
	
	float twin_lever_target;
	float l_lever_target;
	float d_lever_target;
	
	uint16_t twin_init_stuck_cnt;
	uint16_t d_lever_init_stuck_cnt;
	uint16_t twin_stuck_cnt;
	uint16_t d_lever_stuck_cnt;
	
}lever_info_t;

typedef struct lever_struct_t
{
	uint8_t status;
	
	lever_info_t* info;
	
	motor_t* l_lever;
  motor_t* r_lever;
	motor_t* d_lever;
}lever_t;

extern lever_t Lever;

void lever_init(void);
void lever_heartbeat(void);
void lever_position_init(void);
void lever(void);
void twin_ctrl(lever_t* lever);
void d_lever_ctrl(lever_t* lever);
void d_lever_stuck_test(lever_t* lever);
void twin_stuck_test(lever_t* lever);
float lever_position_lock(float tar,uint8_t type);

#endif
