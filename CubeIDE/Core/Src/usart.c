/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
	ringbuf_u8_s ringbuffer_UART2_RX;
	uint8_t buffer_UART2_RX[BUFFER_SIZE];

	ringbuf_u8_s ringbuffer_UART2_TX;
	uint8_t buffer_UART2_TX[BUFFER_SIZE];
/* USER CODE END 0 */

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_EnableDirectionTx(USART2);
  LL_USART_EnableDirectionRx(USART2);
  LL_USART_Enable(USART2);
  /* USER CODE BEGIN USART2_Init 2 */
  LL_USART_EnableIT_RXNE(USART2);
  //LL_USART_EnableIT_TXE(USART2);

  /* USER CODE END USART2_Init 2 */

}

/* USER CODE BEGIN 1 */

void InitRingbuffer(void){
	ringbuf_u8_init(&ringbuffer_UART2_RX, buffer_UART2_RX, BUFFER_SIZE);
	ringbuf_u8_init(&ringbuffer_UART2_TX, buffer_UART2_TX, BUFFER_SIZE);
}


void USART2_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_TXE(USART2))
	{
		uint8_t tx_data;
		if(ringbuf_u8_dequeue(&ringbuffer_UART2_TX, &tx_data) != 1){
			LL_USART_DisableIT_TXE(USART2);
		}else{
			LL_USART_TransmitData8(USART2, tx_data);
		}

	}
	if(LL_USART_IsActiveFlag_RXNE(USART2))
	{
		USART2_ReadData();
	}

}


void USART2_ReadData(void)
{
	uint8_t rx_data = LL_USART_ReceiveData8(USART2);
    if(rx_data == '\r' || rx_data == '\n') // Enter pressed
    {
    	USART2_HandleData();
    	return;
    }
    else if(rx_data == '\177') // backspace pressed
    {
    	ringbuf_u8_dequeue(&ringbuffer_UART2_RX,NULL);
    }
    else
    {
        ringbuf_u8_queue(&ringbuffer_UART2_RX,rx_data);
    }
    ringbuf_u8_queue(&ringbuffer_UART2_TX,rx_data);
    LL_USART_EnableIT_TXE(USART2);
}

void USART2_SendData(uint8_t* message,uint8_t size)
{

	for (size_t i = 0; i < size; i++) {
	    ringbuf_u8_queue(&ringbuffer_UART2_TX, message[i]);
	}
	LL_USART_EnableIT_TXE(USART2);
}

void USART2_HandleData(void)
{
	uint8_t  size = ringbuffer_UART2_RX.head-ringbuffer_UART2_RX.tail;
	uint8_t data[BUFFER_SIZE] = "";
	ringbuf_u8_dequeue_array(&ringbuffer_UART2_RX, data, size);

	HandleCommand(data);
}
/* USER CODE END 1 */
