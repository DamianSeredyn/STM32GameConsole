#ifndef BUTTONS_EXTI_H
#define BUTTONS_EXTI_H

#include "main.h"
#include <stdbool.h>

// PC13-button1 PA5-dioda led
#define Button_Pin LL_GPIO_PIN_13
#define Button_GPIO_Port GPIOC
// EXTI do PC13
#define Button_EXTI_IRQn EXTI15_10_IRQn
#define GREEN_Pin LL_GPIO_PIN_5
#define GREEN_GPIO_Port GPIOA


void BUTTON_EXTI_Init(void);
#endif