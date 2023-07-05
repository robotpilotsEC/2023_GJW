#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

/*
0x200 对应id为0x201~0x204
0x1FF 对应id为0x205~0x208
*/

#define motor_3508_id  0x201
#define motor_6020_id  0x205
#define motor_2006_id  0x208

extern int16_t can1_0x1FF_send_buff[4];
extern int16_t can2_0x1FF_send_buff[4];
extern int16_t can1_0x200_send_buff[4];
extern int16_t can2_0x200_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif


