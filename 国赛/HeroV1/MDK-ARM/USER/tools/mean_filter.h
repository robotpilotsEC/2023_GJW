#ifndef __MEAN_FILTER_H
#define __MEAN_FILTER_H

#include "stm32f4xx_hal.h"

#define FILTER_LEN  5

typedef struct Mean_Filter_Struct{
	char len;          
	char index;         
	float cnt;          
	float SPEED[FILTER_LEN];   
}Mean_Filter_t;


float Mean_Filter_Ctrl(Mean_Filter_t * filter,float mea);
void Mean_Filter_Init(Mean_Filter_t * filter);

#endif

