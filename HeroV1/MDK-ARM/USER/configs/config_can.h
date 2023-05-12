#ifndef __CONFIG_CAN_H
#define __CONFIG_CAN_H

#include "stm32f4xx_hal.h"

#define chassis_motor_LF_id 0x201
#define chassis_motor_RF_id 0x202//0x202
#define chassis_motor_LB_id 0x203
#define chassis_motor_RB_id 0x204
#define feed_motor_id             0x207//0x207

#define position_motor_id         0x202
#define friction_left_motor_id    0x206
#define friction_right_motor_id   0x207

#define gimbal_motor_pitch_id 0x205//0x205
#define gimbal_motor_yaw_id   0x208

#define power_heat_data     0x100
#define game_robot_status   0x101
#define shoot_data          0x102
#define game_robot_pos      0x103
#define robot_hurt          0x104

extern int16_t can1_0x1FF_send_buff[4];
extern int16_t can2_0x1FF_send_buff[4];
extern int16_t can1_0x200_send_buff[4];
extern int16_t can2_0x200_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);
void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

#endif


