#ifndef __RAILGUN_H
#define __RAILGUN_H

#include "stm32f4xx_hal.h"
#include "motor.h"
#include "stdbool.h"
#include "launcher.h"
#include "lever.h"

typedef enum 
{
	RC_CTRL = 0,
	KEY_CTRL,
}railgun_ctrl_e;

typedef enum
{
	init_mode = 0,//初始化态
	camer_mode,//控制相机态
	shoot_mode,//控制发射机构态
	shift_mode,//控制底盘状态
}railgun_mode_e;

/*chassis command begin*/
typedef enum 
{
	middle_speed = 0,
	high_speed,
	low_speed,
	key_slow_move_on,
	key_slow_move_off,
	key_chassis_accelerate_ok,
	key_chassis_accelerate_n_ok,
}chassis_command_e;

typedef struct chassis_command_struct_t
{
	uint8_t shifting_speed_command;//移动速度命令
	uint8_t key_unlock_slow_speed_command;//键盘解锁慢速底盘命令
	uint8_t key_accelerate_command;
}chassis_command_t;
/*chassis command end*/

/*shoot command begin*/
typedef enum
{
	key_lever_accelerate_ok = 0,
	key_lever_accelerate_n_ok,
	fire,
	reload,
	launch_standby,
	manual_ctrl,
	auto_ctrl,
}shoot_command_e;

typedef struct shoot_command_struct_t
{
	uint8_t key_accelerate_command;
	uint8_t launcher_command;
	uint8_t ctrl_command;
}shoot_command_t;
/*shoot command end*/

typedef struct railgun_command_struct_t
{
	uint8_t camer_command;
	shoot_command_t *shoot_command;
	chassis_command_t *chassis_command;
}railgun_command_t;

typedef struct railgun_struct_t
{
	uint8_t ctrl;
	uint8_t status;
	railgun_command_t *command;
}railgun_t;

extern railgun_t RAILGUN;

void rc_value_clear(void);
void Rc_S1_Status_Check(railgun_t* rail);
void Rc_S2_Status_Check(railgun_t* rail);
void Rc_Thumbwheel_Check(railgun_t* rail);
void Rc_Status_Scan(railgun_t* rail);
void Key_Status_Scan(railgun_t* rail);
void rail_status_update(railgun_t* rail);

#endif




