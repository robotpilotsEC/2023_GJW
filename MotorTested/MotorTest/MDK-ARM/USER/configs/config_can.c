#include "config_can.h"
#include "drv_haltick.h"
#include "my_motor.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x1FF_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case motor_3508_id:
			motor_3508.rx(&motor_3508, rxBuf, 8);
			break;
		case motor_6020_id:
			motor_6020.rx(&motor_6020, rxBuf, 8);
			break;
		case motor_2006_id:
			motor_2006.rx(&motor_2006, rxBuf, 8);
			break;
		default:
			break;
	}
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{
		case motor_3508_id:
			motor_3508.rx(&motor_3508, rxBuf, 8);
			break;
		case motor_6020_id:
			motor_6020.rx(&motor_6020, rxBuf, 8);
			break;
		case motor_2006_id:
			motor_2006.rx(&motor_2006, rxBuf, 8);
			break;
		default:
			break;
	}
}

