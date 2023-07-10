/**
  ******************************************************************************
  * @file           : drv_uart.c\h
  * @brief          : 
  * @note           : finish 2022-2-12 12:24:28
  ******************************************************************************
  */

#include "drv_uart.h"
#include "config_uart.h"
#include "string.h"

extern UART_HandleTypeDef huart4;

uint8_t usart4_dma_rxbuf[USART4_RX_BUF_LEN];

void USART4_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart4);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	
	//使能DMA接收
	SET_BIT(huart4.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart4.hdmarx, \
			  (uint32_t)&huart4.Instance->DR, \
			  (uint32_t)usart4_dma_rxbuf, \
			  USART4_RX_BUF_LEN);
}

void DRV_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    // 判断是否为空闲中断
	if( __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&		//IDLE中断标志
		__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))		//IDLE中断使能
	{
		uart_rx_idle_callback(huart);
	}
}

//static HAL_StatusTypeDef DMAEx_MultiBufferStart_NoIT(DMA_HandleTypeDef *hdma, \
//                                                    uint32_t SrcAddress, \
//                                                    uint32_t DstAddress, \
//                                                    uint32_t SecondMemAddress, \
//                                                    uint32_t DataLength)
//{
//    HAL_StatusTypeDef status = HAL_OK;

//    /* Memory-to-memory transfer not supported in double buffering mode */
//    if (hdma->Init.Direction == DMA_MEMORY_TO_MEMORY)
//    {
//		hdma->ErrorCode = HAL_DMA_ERROR_NOT_SUPPORTED;
//		return HAL_ERROR;
//    }   

//	/* Set the UART DMA transfer complete callback */
//	/* Current memory buffer used is Memory 1 callback */
//	hdma->XferCpltCallback   = dma_m0_rxcplt_callback;
//	/* Current memory buffer used is Memory 0 callback */
//	hdma->XferM1CpltCallback = dma_m1_rxcplt_callback;	

//	/* Check callback functions */
//	if ((NULL == hdma->XferCpltCallback) || (NULL == hdma->XferM1CpltCallback))
//	{
//	hdma->ErrorCode = HAL_DMA_ERROR_PARAM;
//	return HAL_ERROR;
//	}
//	
//	/* Process locked */
//	__HAL_LOCK(hdma);
//	
//	if(HAL_DMA_STATE_READY == hdma->State)
//	{	
//		/* Change DMA peripheral state */
//		hdma->State = HAL_DMA_STATE_BUSY;

//		/* Initialize the error code */
//		hdma->ErrorCode = HAL_DMA_ERROR_NONE;

//		/* Enable the Double buffer mode */
//		hdma->Instance->CR |= (uint32_t)DMA_SxCR_DBM;

//		/* Configure DMA Stream destination address */
//		hdma->Instance->M1AR = SecondMemAddress;		

//		/* Configure DMA Stream data length */
//		hdma->Instance->NDTR = DataLength;		
//		
//		/* Peripheral to Memory */
//		if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
//		{   
//			/* Configure DMA Stream destination address */
//			hdma->Instance->PAR = DstAddress;

//			/* Configure DMA Stream source address */
//			hdma->Instance->M0AR = SrcAddress;
//		}
//		/* Memory to Peripheral */
//		else
//		{
//			/* Configure DMA Stream source address */
//			hdma->Instance->PAR = SrcAddress;

//			/* Configure DMA Stream destination address */
//			hdma->Instance->M0AR = DstAddress;
//		}		
//		
//		/* Clear TC flags */
//		__HAL_DMA_CLEAR_FLAG (hdma, __HAL_DMA_GET_TC_FLAG_INDEX(hdma));
//		/* Enable TC interrupts*/
////		hdma->Instance->CR  |= DMA_IT_TC;
//		
//		/* Enable the peripheral */
//		__HAL_DMA_ENABLE(hdma); 
//	}
//	else
//	{
//		/* Process unlocked */
//		__HAL_UNLOCK(hdma);	  

//		/* Return error status */
//		status = HAL_BUSY;		
//	}
//	/* Process unlocked */
//	__HAL_UNLOCK(hdma);

//	return status; 	
//}

static HAL_StatusTypeDef DMA_Start(DMA_HandleTypeDef *hdma, \
                            uint32_t SrcAddress, \
                            uint32_t DstAddress, \
                            uint32_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* Process locked */
	__HAL_LOCK(hdma);
	if(HAL_DMA_STATE_READY == hdma->State)
	{
		/* Change DMA peripheral state */
		hdma->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		hdma->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		/* Clear DBM bit */
		hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

		/* Configure DMA Stream data length */
		hdma->Instance->NDTR = DataLength;

		/* Memory to Peripheral */
		if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
		{
			/* Configure DMA Stream destination address */
			hdma->Instance->PAR = DstAddress;//外设地址

			/* Configure DMA Stream source address */
			hdma->Instance->M0AR = SrcAddress;//储存器
		}
		/* Peripheral to Memory */
		else
		{
			/* Configure DMA Stream source address */
			hdma->Instance->PAR = SrcAddress;

			/* Configure DMA Stream destination address */
			hdma->Instance->M0AR = DstAddress;
		}

		/* Enable the Peripheral */
		__HAL_DMA_ENABLE(hdma);
	}
	else
	{
		/* Process unlocked */
		__HAL_UNLOCK(hdma);

		/* Return error status */
		status = HAL_BUSY;
	} 
	return status; 	
}

static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
	/* clear idle it flag avoid idle interrupt all the time */
	__HAL_UART_CLEAR_IDLEFLAG(huart);	
	/* handle received data in idle interrupt */
	if(huart == &huart4)
	{
		/* clear DMA transfer complete flag */
		__HAL_DMA_DISABLE(huart->hdmarx);
		/* handle dbus data dbus_buf from DMA */
		USART4_rxDataHandler(usart4_dma_rxbuf);
		memset(usart4_dma_rxbuf, 0, USART4_RX_BUF_LEN);
		/* restart dma transmission */	  
		__HAL_DMA_ENABLE(huart->hdmarx);		
	}
}
