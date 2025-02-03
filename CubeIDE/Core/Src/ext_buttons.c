#include "ext_buttons.h"

void BUTTON_EXTI_Init(void)
{

	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

	  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);


	/* Set Button */
	  //button1
	  LL_GPIO_SetPinPull(Button_GPIO_Port, Button_Pin, LL_GPIO_PULL_UP);
     LL_GPIO_IsInputPinSet(Button_GPIO_Port, Button_Pin);
     LL_GPIO_SetPinSpeed(Button_GPIO_Port, Button_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(Button_GPIO_Port, Button_Pin, LL_GPIO_MODE_INPUT);
     //button2
     LL_GPIO_SetPinPull(Button_GPIO_Port, Button1_Pin, LL_GPIO_PULL_UP);
     LL_GPIO_IsInputPinSet(Button_GPIO_Port, Button1_Pin);
     LL_GPIO_SetPinSpeed(Button_GPIO_Port, Button1_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(Button_GPIO_Port, Button1_Pin, LL_GPIO_MODE_INPUT);
     //button3
     LL_GPIO_SetPinPull(Button_GPIO_Port, Button2_Pin, LL_GPIO_PULL_UP);
     LL_GPIO_IsInputPinSet(Button_GPIO_Port, Button2_Pin);
     LL_GPIO_SetPinSpeed(Button_GPIO_Port, Button2_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(Button_GPIO_Port, Button2_Pin, LL_GPIO_MODE_INPUT);
     //button4
     LL_GPIO_SetPinPull(Button_GPIO_Port, Button3_Pin, LL_GPIO_PULL_UP);
     LL_GPIO_IsInputPinSet(Button_GPIO_Port, Button3_Pin);
     LL_GPIO_SetPinSpeed(Button_GPIO_Port, Button3_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(Button_GPIO_Port, Button3_Pin, LL_GPIO_MODE_INPUT);
	
	/* Set Led */
     // 1 dioda PA4
	 LL_GPIO_SetPinOutputType(GREEN_GPIO_Port, GREEN_Pin,LL_GPIO_OUTPUT_PUSHPULL);
	 LL_GPIO_SetPinPull(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_PULL_NO);
	 LL_GPIO_SetPinSpeed(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_SPEED_FREQ_LOW);
	 LL_GPIO_SetPinMode(GREEN_GPIO_Port, GREEN_Pin, LL_GPIO_MODE_OUTPUT);
     LL_GPIO_ResetOutputPin(GREEN_GPIO_Port, GREEN_Pin);
    // 2 dioda PA5
    LL_GPIO_SetPinOutputType(GREEN_GPIO_Port, GREEN1_Pin,LL_GPIO_OUTPUT_PUSHPULL);
   	LL_GPIO_SetPinPull(GREEN_GPIO_Port, GREEN1_Pin, LL_GPIO_PULL_NO);
   	LL_GPIO_SetPinSpeed(GREEN_GPIO_Port, GREEN1_Pin, LL_GPIO_SPEED_FREQ_LOW);
   	LL_GPIO_SetPinMode(GREEN_GPIO_Port, GREEN1_Pin, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GREEN_GPIO_Port, GREEN1_Pin);
    // 3 dioda PA6
     LL_GPIO_SetPinOutputType(GREEN_GPIO_Port, GREEN2_Pin,LL_GPIO_OUTPUT_PUSHPULL);
     LL_GPIO_SetPinPull(GREEN_GPIO_Port, GREEN2_Pin, LL_GPIO_PULL_NO);
     LL_GPIO_SetPinSpeed(GREEN_GPIO_Port, GREEN2_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(GREEN_GPIO_Port, GREEN2_Pin, LL_GPIO_MODE_OUTPUT);
     LL_GPIO_ResetOutputPin(GREEN_GPIO_Port, GREEN2_Pin);
   //4 dioda PA6
     LL_GPIO_SetPinOutputType(GREEN_GPIO_Port, GREEN3_Pin,LL_GPIO_OUTPUT_PUSHPULL);
     LL_GPIO_SetPinPull(GREEN_GPIO_Port, GREEN3_Pin, LL_GPIO_PULL_NO);
     LL_GPIO_SetPinSpeed(GREEN_GPIO_Port, GREEN3_Pin, LL_GPIO_SPEED_FREQ_LOW);
     LL_GPIO_SetPinMode(GREEN_GPIO_Port, GREEN3_Pin, LL_GPIO_MODE_OUTPUT);
     LL_GPIO_ResetOutputPin(GREEN_GPIO_Port, GREEN3_Pin);

    /* EXTI konfiguracja */
	 LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE12);
     LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_12);
     LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_12);
     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_12);

     LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE13);
     LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_13);
     LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_13);
     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);

     LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE14);
     LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_14);
     LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_14);
     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_14);

     LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTC, LL_SYSCFG_EXTI_LINE15);
     LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_15);
     LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_15);
     LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15);

    /* NVIC konfiguracja */
     NVIC_SetPriority(EXTI15_10_IRQn, 0);
     NVIC_EnableIRQ(EXTI15_10_IRQn);




}
