#include "mean_filter.h"

//Mean_Filter_t Mean_Filter;
//.filter->maen(3)


void Mean_Filter_Init(Mean_Filter_t * filter)
{
	filter->len = FILTER_LEN;
	filter->cnt = 0.0f;
	filter->index = 0;
	for(int i = 0;i < filter->len;i++)
	{
		filter->SPEED[i] = 0.0f;
	}
}

void Mean_Filter_Input(Mean_Filter_t * filter,float mea)
{
  filter->index %= FILTER_LEN;
	
	filter->SPEED[filter->index] = mea;
	
	filter->index++;
	
	filter->cnt ++;
}

float Mean_Filter_Output(Mean_Filter_t * filter,float mea)
{
	float sum = 0,speed = 0;
	
	if(filter->cnt < FILTER_LEN)
	{
		for(int i = 0;i < filter->cnt;i++)
		{
			sum += filter->SPEED[i];
		}
		if(filter->cnt != 0)
		{
			speed = sum / filter->cnt;
		}
		else
		{
			speed = mea;
		}
	}
	else
	{
		for(int j = 0;j < FILTER_LEN;j++)
		{
			sum += filter->SPEED[j];
			
			speed = sum / FILTER_LEN;
		}
	}
	return speed;                  
}

float Mean_Filter_Ctrl(Mean_Filter_t * filter,float mea)
{
	float out = 0.0f;
	
	Mean_Filter_Input(filter,mea);
	
  out = Mean_Filter_Output(filter,mea);
	
	return out;
}




