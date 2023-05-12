#include "judge.h"
#include "config_status.h"
#include "gimbal.h"

judge_base_info_t judge_base_info;
judge_info_t judge_info = 
{
	.offline_cnt_max = 1000,
};
judge_t judge = 
{
	.base_info = &judge_base_info,
	.info = &judge_info,
};

uint8_t judge_tx_buff1[8];
uint8_t judge_tx_buff2[8];

void judge_init(judge_t *judge)
{
	judge->info->offline_cnt = judge->info->offline_cnt_max;
	judge->info->status = DEV_OFFLINE;
}

void judge_realtime_task(judge_t *judge)
{
	judge->info->offline_cnt++;
	if(judge->info->offline_cnt >= judge->info->offline_cnt_max)
	{
		judge_init(judge);
	}
}

void judge_transmit(judge_t *judge)
{
	memcpy(judge_tx_buff1,&Gimbal.yaw_gimbal->rx_info.angle, 2);
	memcpy(&judge_tx_buff1[2],&Gimbal.pitch_gimbal->rx_info.angle, 2);
	//TBC
}
