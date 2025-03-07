/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "ringbuf_ptr.h"
#include "ringbuf_u8.h"
#include "usart_console.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define BUFFER_SIZE 256

/* USER CODE END Private defines */
void InitRingbuffer(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART2_ReadData(void);
void USART2_SendData(uint8_t* message,uint16_t size);
void USART2_HandleData(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

