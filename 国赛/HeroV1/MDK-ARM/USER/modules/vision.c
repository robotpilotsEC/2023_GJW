#include "vision.h"
#include "string.h"
#include "config_status.h"
#include "gimbal.h"
#include "judge.h"
#include "mathematics.h"

vision_base_info_t vision_base_info;
vision_info_t vision_info;
vision_process_info_t vision_process_info;
vision_config_t vision_config = 
{
	.offline_cnt_max = 50,
};

vision_t vision = 
{
	.base_info = &vision_base_info,
	.info = &vision_info,
	.config = &vision_config,
	.pro_info = &vision_process_info,
	.tx_info = &vision_tx_info,
	.rx_info = &vision_rx_info,
};

void vision_tick_task(vision_t *vision)
{
	vision_info_t *info = vision->info;
	
	info->offline_cnt++;
	if(info->offline_cnt >= vision->config->offline_cnt_max)
	{
		info->offline_cnt = vision->config->offline_cnt_max + 1;
		info->status = DEV_OFFLINE;
	}
	else
	{
		info->status = DEV_ONLINE;
	}
}

bool vision_send(uint8_t datau8_1,float dataf_1,float dataf_2,uint8_t datau8_2,uint8_t datau8_3)
{
	memcpy(&vision_tx_info.datau8_1, &datau8_1, 1);
	memcpy((void*)&vision_tx_info.datas_1, &dataf_1, 4);
	memcpy((void*)&vision_tx_info.datas_2, &dataf_2, 4);
	memcpy(&vision_tx_info.datau8_2, &datau8_2, 1);
	memcpy(&vision_tx_info.datau8_3, &datau8_3, 1);
	if(vision_send_data() == true)
	{
			return true;
	}
	return false;
}

bool vision_receive(uint8_t *rxBuf)
{
	vision_base_info_t *base_info = vision.base_info;
	vision_info_t *info = vision.info;
	vision_process_info_t *pro_info = vision.pro_info;
	
	info->offline_cnt = 0;
	
	if(vision_recieve_data(rxBuf) == true)
	{
		memcpy(&base_info->pitch_receive,(void*)&vision_rx_info.dataf_1,4);
		memcpy(&base_info->yaw_receive,(void*)&vision_rx_info.dataf_2,4);
		memcpy(&base_info->detect_mode,&vision_rx_info.datau8_1,1);
		memcpy(&base_info->capture_Y_O_N,&vision_rx_info.datau8_2,1);
		memcpy(&base_info->distance_receive,&vision_rx_info.datau8_3,1);
		memcpy(&base_info->gyro_found,&vision_rx_info.datau8_4,1);
		memcpy(&base_info->detect_number,&vision_rx_info.datau8_5,1);
		
		if(base_info->capture_Y_O_N != 0)
		{
			if(base_info->yaw_receive <= 4096)
			{
				pro_info->yaw_move = 4096 - base_info->yaw_receive;
			}
			else
			{
				pro_info->yaw_move = -(base_info->yaw_receive - 4096);
			}
			if(base_info->pitch_receive <= 4096)
			{
				pro_info->pitch_move = (4096 - base_info->pitch_receive);
			}
			else
			{
				pro_info->pitch_move = -(base_info->pitch_receive - 4096);
			}
		}
		else
		{
			pro_info->pitch_move = Gimbal.info->pitch_angle_imu_measure;
			pro_info->yaw_move = Gimbal.info->yaw_angle_imu_measure;
		}
		return true;
	}
	return false;
}

/*视觉角度处理*/
void vision_transmit_data_process(vision_t *vision)
{
	if(Gimbal.info->yaw_angle_imu_measure >= 0)
	{
		vision->base_info->yaw_vision_send = 4096 - Gimbal.info->yaw_angle_imu_measure;
	}
	else
	{
		vision->base_info->yaw_vision_send = 4096 - Gimbal.info->yaw_angle_imu_measure;
	}
	if(Gimbal.info->pitch_angle_imu_measure >= 0)
	{
		vision->base_info->pitch_vision_send = 4096 - Gimbal.info->pitch_angle_imu_measure;
	}
	else
	{
		vision->base_info->pitch_vision_send = 4096 - Gimbal.info->pitch_angle_imu_measure;
	}
}
/*视觉传输任务*/
void vision_transmit_task(vision_t *vision)
{
	vision_transmit_data_process(vision);
	
	vision_send(vision->base_info->main_mode,vision->base_info->yaw_vision_send,vision->base_info->pitch_vision_send,\
	            judge.base_info->car_color,0);
}

void vision_init(vision_t *vision)
{
	memset(vision->base_info, 0, sizeof(vision_base_info_t));
	memset(vision->info, 0, sizeof(vision_info_t));
	memset(vision->rx_info, 0, sizeof(vision_rx_info_t));
	memset(vision->pro_info, 0, sizeof(vision_process_info_t));
	vision->base_info->main_mode = 1;
}
