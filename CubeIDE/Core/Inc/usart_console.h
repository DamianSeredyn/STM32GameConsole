/*
 * usart_source.h
 *
 *  Created on: Nov 21, 2024
 *      Author: Damian
 */

#ifndef INC_USART_CONSOLE_H_
#define INC_USART_CONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CONSOLE_WORDS 10
#define BUFFER_SIZE_COMMAND 128

#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_spi.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_tim.h"


#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

#include "usart.h"
#include "i2c.h"
#include "spi.h"
#include "Tim.h"
#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "led_display.h"

void HandleCommand(uint8_t* data);
unsigned int hash_command( uint8_t* cmd);
void split_data_into_words(uint8_t* data, uint8_t words[MAX_CONSOLE_WORDS][BUFFER_SIZE_COMMAND]);
#endif /* INC_USART_CONSOLE_H_ */



