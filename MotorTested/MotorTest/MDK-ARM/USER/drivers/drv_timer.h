#ifndef __DRV_TIMER_H
#define __DRV_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported macro ------------------------------------------------------------*/

#define SERVO_PWM	  TIM1->CCR2


/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void PWM_Init(void);
void SERVO_PwmOut(int16_t pwm);
void SERVO_SLEEP(void);
void SERVO_WEAK(void);
#endif




