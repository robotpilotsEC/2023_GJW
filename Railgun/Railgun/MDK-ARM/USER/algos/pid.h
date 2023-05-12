/**
  ******************************************************************************
  * @file           : pid.c\h
  * @brief          : 
  * @note           : finish 2022-2-13 19:19:47
  ******************************************************************************
  */

#ifndef __PID_H
#define __PID_H

#include "stm32f4xx_hal.h"

#define abs(x) ((x)>0? (x):(-(x)))
#define sgn(x) (((x)>0)?1:((x)<0?-1:0))

float lowpass(float X_last, float X_new, float K);//低通滤波
float ave(int16_t sum, float ave_last, float new_value);  //求均值

int16_t str_to_num(uint8_t *str, uint16_t len);  //字符串转数值
uint8_t num_to_str(int16_t num, uint8_t *str, uint16_t *len);  //字符串转数值
void num_to_str_2(uint16_t num, uint8_t *str, uint16_t len);
void num_to_str_3(int16_t num, uint8_t *str, uint16_t len);

typedef struct 
{
	float	  target;
	float	  measure;
	float 	err;
	float 	last_err;
	float	  integral;
	float 	pout;
	float 	iout;
	float 	dout;
	float 	out;
	/* 配置 */
	float   blind_err;
	float   integral_bias;
	float	  kp;
	float 	ki;
	float 	kd;
	float 	integral_max;
	float 	out_max;
}pid_info_t;

typedef struct
{
	pid_info_t *info;
}pid_t;


void pid_init(pid_t *pid);
void single_pid_cal(pid_info_t *pid);
void nonlinear_pid_cal(pid_info_t *pid);
float fal(float err, float a, float d);

#endif
