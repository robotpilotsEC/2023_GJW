#include "config_uart.h"
#include "remote.h"
#include "remote_potocol.h"
//#include "vision.h"
//#include "usmart.h"

void USART1_rxDataHandler(uint8_t *rxBuf)
{

}

void USART2_rxDataHandler(uint8_t *rxBuf)
{
	/* ң������Ϣ���� */
	rc_base_info_update(rc.base_info,rxBuf);
	rc_base_info_check(rc.base_info);
	
	/* ң�������� */
	rc_interrupt_update(&rc);
}

void USART3_rxDataHandler(uint8_t *rxBuf)
{

}

void USART4_rxDataHandler(uint8_t *rxBuf)
{
}

void USART5_rxDataHandler(uint8_t *rxBuf)
{
}
