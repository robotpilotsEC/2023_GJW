#ifndef __VISION_H
#define __VISION_H

#include "stm32f4xx_hal.h"
#include "vision_protocol.h"

typedef struct
{
	uint16_t offline_cnt_max;
}vision_config_t;

typedef struct
{
	uint16_t offline_cnt;
	uint8_t status;
}vision_info_t;

typedef struct
{
	float yaw_vision_send;
	float pitch_vision_send;
	
	float yaw_receive;
	float pitch_receive;
	float distance_receive;
	uint8_t capture_Y_O_N;
	uint8_t detect_mode;//视觉状态：1 自瞄 2 识别小陀螺
	uint8_t gyro_found;
	uint8_t detect_number;
	uint8_t main_mode;
	
}vision_base_info_t;

typedef struct 
{
	float yaw_move;
	float pitch_move;
}vision_process_info_t;

typedef struct 
{
	vision_base_info_t *base_info;
	vision_config_t *config;
	vision_info_t *info;
	vision_process_info_t* pro_info;
	
	vision_tx_info_t *tx_info;
	vision_rx_info_t *rx_info;
}vision_t;

extern vision_t vision;

void vision_tick_task(vision_t *vision);
bool vision_send(uint8_t datau8_1,float dataf_1,float dataf_2,uint8_t datau8_2,uint8_t datau8_3);
bool vision_receive(uint8_t *rxBuf);
void vision_transmit_task(vision_t *vision);
void vision_init(vision_t *vision);

#endif
