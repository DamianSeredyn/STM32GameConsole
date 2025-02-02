#include "ext_buttons.h"

void BUTTON_EXTI_Init(void)
{

	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

	  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);


	/* Set Button */
	 LL_GPIO_SetPinPull(Button_GPIO_Port, Button_Pin, LL_GPIO_PULL_NO);
     LL_GPIO_IsInputPinSet(Button_GPIO_Port, Button_Pin);
     LL_GPIO_SetPinSpeed(Button_GPIO_Port, Button_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(Button_GPIO_Port, Button_Pin, LL_GPIO_MODE_INPUT);
	
	/* Set Led */
	 LL_GPIO_SetPinOutputType(GREEN_GPIO_Port, GREEN_Pin,LL_GPIO_OUTPUT_PUSHPULL);
	 LL_GPIO_SetPinPull(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_PULL_NO);
	 LL_GPIO_SetPinSpeed(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_SPEED_FREQ_LOW);
	 LL_GPIO_SetPinMode(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_MODE_OUTPUT);
    
    /* EXTI konfiguracja */
	 LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
     LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_13);
     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);

    /* NVIC konfiguracja */
     NVIC_SetPriority(EXTI15_10_IRQn, 0);
     NVIC_EnableIRQ(EXTI15_10_IRQn);

}
