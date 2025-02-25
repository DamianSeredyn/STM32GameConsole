/*
 * dynamic_leds.c
 *
 *  Created on: Feb 25, 2025
 *      Author: Damian
 */

#include "dynamic_leds.h"

void MX_TIM4_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetCounterMode(TIM4, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetPrescaler(TIM4, 8-1);
  LL_TIM_SetAutoReload(TIM4, 12-1);


  LL_TIM_OC_SetCompareCH1(TIM4, 6);

  LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
  LL_TIM_OC_SetPolarity(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);

  LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableDMAReq_CC1(TIM4);
  LL_TIM_EnableCounter(TIM4);


  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);



}

void DMA_TIM4_Init(void) {

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_6);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_WORD);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_WORD);

    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1,
        (uint32_t)LED_Data,
        (uint32_t)&TIM4->CCR1,
		LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, sizeof(LED_Data));
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);


}

void dynamic_Leds_Test(void)
{
	SetColorOnLed(0,0,0,255);
	SetColorOnLed(6,255,0,0);
}


void SetColorOnLed(uint8_t LEDnumber, uint8_t R, uint8_t G, uint8_t B)
{
	for(int i = 7; i>-1;i--)
	{
		if((G & 1<<i) != 0)
			LED_Data[LEDnumber*3+i] = 7;
		else
			LED_Data[LEDnumber*3+i] = 3;
		if((R & 1<<i) != 0)
			LED_Data[LEDnumber*3+8+i] = 7;
		else
			LED_Data[LEDnumber*3+8+i] = 3;
		if((B & 1<<i) != 0)
			LED_Data[LEDnumber*3+16+i] = 7;
		else
			LED_Data[LEDnumber*3+16+i] = 3;

	}

}
