#include "config_can.h"
#include "drv_haltick.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x1FF_send_buff[4];

void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{

	}
}

void CAN2_rxDataHandler(uint32_t canId, uint8_t *rxBuf)
{
	switch(canId)
	{

	}
}

