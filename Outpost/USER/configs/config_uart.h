#ifndef __CONFIG_UART_H
#define __CONFIG_UART_H

#include "stm32f4xx_hal.h"

#define USART1_RX_BUF_LEN     350
#define USART2_RX_DATA_FRAME_LEN	(18u)	// ����2����֡����
#define USART2_RX_BUF_LEN			(USART2_RX_DATA_FRAME_LEN + 6u)	// ����2���ջ���������
#define USART3_RX_BUF_LEN			200
#define USART4_RX_BUF_LEN			200
#define USART5_RX_BUF_LEN			200

void USART2_rxDataHandler(uint8_t *rxBuf);


#endif

