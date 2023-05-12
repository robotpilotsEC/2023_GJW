#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

extern int16_t can1_0x200_send_buff[4];
extern int16_t can1_0x1FF_send_buff[4];
extern int16_t can2_0x200_send_buff[4];
extern int16_t can2_0x1FF_send_buff[4];

void CAN1_Get_Data(uint32_t id, uint8_t *rxBuf); //CAN1接收函数
void CAN2_Get_Data(uint32_t id, uint8_t *rxBuf); //CAN2接收函数

#endif






