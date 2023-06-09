#ifndef __3508_MOTOR_H
#define __3508_MOTOR_H

#include "stm32f4xx_hal.h"
#include "drv_can.h"
#include "pid.h"

typedef struct
{
	int16_t   angle;            //电机角度(0~8191)
	int16_t   speed;            //电机转速(RPM)(max = 7600)
	int16_t   current;          //转矩电流-16384~16384(-20~20A)
	int16_t   temperature;      //摄氏度
}motor_3508_base_info_t;

typedef struct
{
	uint8_t          offline_cnt;
	uint8_t          offline_cnt_max;
	uint8_t          status;
	int16_t          angle_add;        //-4096~4096
	int32_t          angle_sum;        //-2147683647~2147683647
	int32_t          target_angle_sum;
}motor_3508_info_t;

typedef struct motor_3508_t
{
	motor_3508_base_info_t *base_info;
	motor_3508_info_t      *info;
	pid_t                  *pid_speed;
	pid_t                  *pid_angle;
	drv_can_t              *can;
	void                  (*init)(struct motor_3508_t *motor);
	void                  (*update)(struct motor_3508_t *motor,uint8_t *rxBuf);
}motor_3508_t;

void motor_3508_init(motor_3508_t *motor);
void motor_3508_update(motor_3508_t *motor,uint8_t *rxBuf);


#endif



