#ifndef BUTTONS_EXTI_H
#define BUTTONS_EXTI_H

#include "main.h"
#include <stdbool.h>

// PC13-button1 PA5-dioda led
 #define Button_GPIO_Port GPIOC
#define Button_Pin LL_GPIO_PIN_12
#define Button1_Pin LL_GPIO_PIN_13
#define Button2_Pin LL_GPIO_PIN_14
#define Button3_Pin LL_GPIO_PIN_15


// EXTI do PC13

#define GREEN_GPIO_Port GPIOA
#define GREEN_Pin LL_GPIO_PIN_4
#define GREEN1_Pin LL_GPIO_PIN_5
#define GREEN2_Pin LL_GPIO_PIN_6
#define GREEN3_Pin LL_GPIO_PIN_7



#define Button_EXTI_IRQn EXTI15_10_IRQn

void BUTTON_EXTI_Init(void);
#endif
