/*
 * dynamic_leds.h
 *
 *  Created on: Feb 25, 2025
 *      Author: Damian
 */

#ifndef INC_DYNAMIC_LEDS_H_
#define INC_DYNAMIC_LEDS_H_

#include "main.h"

#define LED_NUMBER 8

static uint8_t LED_Data[LED_NUMBER*3*8+50] = {0};

void MX_TIM4_Init(void);
void dynamic_Leds_Test(void);
void SetColorOnLed(uint8_t LEDnumber, uint8_t R, uint8_t G, uint8_t B);
void DMA_TIM4_Init(void);
#endif /* INC_DYNAMIC_LEDS_H_ */
