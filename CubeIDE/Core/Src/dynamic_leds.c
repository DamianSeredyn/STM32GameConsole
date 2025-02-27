/*
 * dynamic_leds.c
 *
 *  Created on: Feb 25, 2025
 *      Author: Damian
 */

#include "dynamic_leds.h"

uint8_t LED_Data[50+LED_NUMBER*3*8+50] = {0};


uint8_t brightness = 15;
void MX_TIM4_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
   LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

   TIM_InitStruct.Prescaler = 8-1;
   TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
   TIM_InitStruct.Autoreload = 12-1;
   TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
   LL_TIM_Init(TIM4, &TIM_InitStruct);
   LL_TIM_DisableARRPreload(TIM4);
   LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
   LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);
   TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
   TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
   TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
   TIM_OC_InitStruct.CompareValue = 0;
   TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
   LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
   LL_TIM_OC_DisableFast(TIM4, LL_TIM_CHANNEL_CH1);
   LL_TIM_SetOCRefClearInputSource(TIM4, LL_TIM_OCREF_CLR_INT_NC);
   LL_TIM_DisableExternalClock(TIM4);
   LL_TIM_ConfigETR(TIM4, LL_TIM_ETR_POLARITY_NONINVERTED, LL_TIM_ETR_PRESCALER_DIV1, LL_TIM_ETR_FILTER_FDIV1);
   LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);
   LL_TIM_DisableMasterSlaveMode(TIM4);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);



}

void DMA_TIM4_Init(void) {

	LL_TIM_EnableDMAReq_CC1(TIM4);
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM4);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_6);
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_NORMAL);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_WORD);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);

	LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);

    NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
   NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}
void DMA1_Channel1_IRQHandler(void){
	if(LL_DMA_IsActiveFlag_TC1(DMA1) == 1){
		LL_DMA_ClearFlag_TC1(DMA1);
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
		LL_DMA_DisableIT_TC(DMA1, LL_DMA_CHANNEL_6);
	}
	else if(LL_DMA_IsActiveFlag_TE1(DMA1)){

	}

}

void dynamic_Leds_Setup(void)
{
    for (int i = 50; i < 192+50; i++)
	{
	       LED_Data[i] = 3;
	}
}
void dynamic_Leds_Test(void)
{
	SetColorOnLed(0,255,0,0);
	SetColorOnLed(1,0,255,0);
	SetColorOnLed(2,0,0,255);
	SetColorOnLed(3,255,0,0);
	SetColorOnLed(4,0,255,0);
	SetColorOnLed(5,0,0,255);
	SetColorOnLed(6,255,0,0);
	SetColorOnLed(7,0,255,0);
	GenerateDataToDma(LED_Data,50+LED_NUMBER*3*8+50);
	SetColorOnLed(0,0,255,0);
	LL_mDelay(50);
	GenerateDataToDma(LED_Data,50+LED_NUMBER*3*8+50);

}

void GenerateDataToDma(uint8_t* data,uint32_t size)
{

	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_1, (uint32_t)LED_Data, (uint32_t)&TIM4->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, size);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}
void SetColorOnLed(uint8_t LEDnumber, uint8_t R, uint8_t G, uint8_t B)
	{
			float angle = 90-brightness;  // in degrees
	    	angle = angle*3.14 / 180;  // in rad

		    for (int i = 7; i >= 0; i--)
		    {


		        LED_Data[50+LEDnumber * 24 + i]      = ((uint8_t)(G/(tan(angle))) & (1 << i)) ? 9 : 3;
		        LED_Data[50+LEDnumber * 24 + 8 + i]  = ((uint8_t)(R/(tan(angle))) & (1 << i)) ? 9 : 3;
		        LED_Data[50+LEDnumber * 24 + 16 + i] = ((uint8_t)(B/(tan(angle))) & (1 << i)) ? 9 : 3;
		    }


	}
