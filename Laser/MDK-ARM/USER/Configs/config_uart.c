/**
  ******************************************************************************
  * @file           : config_uart.c\h
  * @brief          : 
  * @note           : finish 2022-2-12 12:24:28
  ******************************************************************************
  */
	
#include "config_uart.h"
#include "string.h"
//9101112
uint8_t data_receive[300];
uint16_t test;
uint8_t c1,c2,c3,c4;
float mea = 0.0f;//ÀåÃ×


uint8_t char_2_hex(uint8_t *src)
{
    uint8_t desc;

    if((*src >= '0') && (*src <= '9'))
        desc = *src - 0x30;
    else if((*src >= 'a') && (*src <= 'f'))
        desc = *src - 0x57;
    else if((*src >= 'A') && (*src <= 'F'))
        desc = *src - 0x37;

    return desc;
}


void USART1_rxDataHandler(uint8_t *rxBuf)
{
	
}

void USART2_rxDataHandler(uint8_t *rxBuf)
{

}

void USART3_rxDataHandler(uint8_t *rxBuf)
{
}

void USART4_rxDataHandler(uint8_t *rxBuf)
{
	memcpy(&data_receive, rxBuf, 20);
	
	/*È¡µÍ4Î»*/
	c1 = char_2_hex(&data_receive[9]) & (0x0F);
	c2 = char_2_hex(&data_receive[10]) & (0x0F);
	c3 = char_2_hex(&data_receive[11]) & (0x0F);
	c4 = char_2_hex(&data_receive[12]) & (0x0F);
	
	test = (c1 << 12) | (c2 << 8) | (c3 << 4) | c4;
	
	mea = (float)test / 10.0f;
}

void USART5_rxDataHandler(uint8_t *rxBuf)
{
}
