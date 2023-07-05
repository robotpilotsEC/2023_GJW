#ifndef __VISION_PROTOCOL_H
#define __VISION_PROTOCOL_H

#include "stm32f4xx_hal.h"
#include "stdbool.h"

typedef __packed struct
{
	uint8_t   SOF;
	uint8_t   datau8_1;
	uint8_t   CRC8;
	float     datas_1;//yaw_angle
	float     datas_2;//pitch_angle
	uint8_t   datau8_3;
	uint8_t   datau8_2;//¼º·½ÑÕÉ«
	uint8_t   datau8_4;//detect_num
	uint16_t  CRC16;
}vision_tx_info_t;

typedef __packed struct
{
	uint8_t  SOF;
	uint8_t  datau8_1;//detect_mode
	uint8_t  CRC8;
	float    dataf_1;  //pitch
	float    dataf_2;  //yaw
	uint8_t  datau8_2; //is_find_target
	uint8_t  datau8_3;  //distance
	uint8_t  datau8_4;  //gyro_found
	uint8_t  datau8_5;//car_number
	uint16_t CRC16;
}vision_rx_info_t;

extern vision_tx_info_t vision_tx_info;
extern vision_rx_info_t vision_rx_info;

bool vision_send_data(void);
bool vision_recieve_data(uint8_t *rxBuf);

#endif



