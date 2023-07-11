#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

#define chassis_motor_LF_id 0x201
#define chassis_motor_RF_id 0x202
#define chassis_motor_LB_id 0x203
#define chassis_motor_RB_id 0x204


extern int16_t can1_0x1FF_send_buff[4];
extern int16_t can2_0x1FF_send_buff[4];
extern int16_t can1_0x200_send_buff[4];
extern int16_t can2_0x200_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif


