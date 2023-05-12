#ifndef __DRV_CAN_H
#define __DRV_CAN_H

#include "stm32f4xx_hal.h"

typedef struct 
{
  CAN_RxHeaderTypeDef Header;
  uint8_t Data[8];
}can_rx_info_t;

typedef struct 
{
	CAN_HandleTypeDef *hcan;
	uint32_t rx_id;
}drv_can_t;

void CAN1_Init(void); //CAN1��ʼ��
void CAN2_Init(void); //CAN2��ʼ��
void CAN1_Send_With_int16_to_uint8(uint32_t stdId, int16_t *dat);
void CAN2_Send_With_int16_to_uint8(uint32_t stdId, int16_t *dat);
void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig); //����CAN��ʶ���˲���
uint8_t CAN_SendData(CAN_HandleTypeDef *hcan, uint32_t stdId, uint8_t *dat); //CAN���ͺ���

#endif

