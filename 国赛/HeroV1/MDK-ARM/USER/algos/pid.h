#ifndef __PID_H
#define __PID_H

#include "stm32f4xx_hal.h"

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
	/* ≈‰÷√ */
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
void single_pid_ctrl(pid_info_t *pid);


#endif




