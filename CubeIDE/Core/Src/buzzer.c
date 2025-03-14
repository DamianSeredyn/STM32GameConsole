/*
 * buzzer.c
 *
 *  Created on: Nov 22, 2024
 *      Author: Damian
 */

#include "buzzer.h"

#define TIM_FREQ 80000000

#define TEMPO 144

uint16_t testMelody[] = {
		131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165, 131, 165, 196, 247, 262, 247, 196, 165
};

int16_t tetrisMelody[] = {

  // Based on the arrangement at https://www.flutetunes.com/tunes.php?id=192

  659, 4,  494, 8,  523, 8,  587, 4,  523, 8,  494, 8,
  440, 4,  440, 8,  523, 8,  659, 4,  587, 8,  523, 8,
  494, -4,  523, 8,  587, 4,  659, 4,
  523, 4,  440, 4,  440, 8,  440, 4,  494, 8,  523, 8,

  587, -4,  698, 8,  880, 4,  784, 8,  698, 8,
  659, -4,  523, 8,  659, 4,  587, 8,  523, 8,
  494, 4,  494, 8,  523, 8,  587, 4,  659, 4,
  523, 4,  440, 4,  440, 4,  0, 4,

  659, 4,  494, 8,  523, 8,  587, 4,  523, 8,  494, 8,
  440, 4,  440, 8,  523, 8,  659, 4,  587, 8,  523, 8,
  494, -4,  523, 8,  587, 4,  659, 4,
  523, 4,  440, 4,  440, 8,  440, 4,  494, 8,  523, 8,

  587, -4,  698, 8,  880, 4,  784, 8,  698, 8,
  659, -4,  523, 8,  659, 4,  587, 8,  523, 8,
  494, 4,  494, 8,  523, 8,  587, 4,  659, 4,
  523, 4,  440, 4,  440, 4,  0, 4,

  659, 2,  523, 2,
  587, 2,  494, 2,
  523, 2,  440, 2,
  415, 2,  494, 4,  0, 8,
  659, 2,  523, 2,
  587, 2,  494, 2,
  523, 4,  659, 4,  880, 2,
  830, 2,
};

 MusicBuffer Mbuffer;

void MX_TIM2_Init(void)
  {

    /* USER CODE BEGIN TIM2_Init 0 */

    /* USER CODE END TIM2_Init 0 */

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    LL_APB1_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_GPIO_SetPinPull(TIM2_CH1_OC_Port, TIM2_CH1_OC_Pin, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(TIM2_CH1_OC_Port, TIM2_CH1_OC_Pin, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetAFPin_0_7(TIM2_CH1_OC_Port, TIM2_CH1_OC_Pin, LL_GPIO_AF_1);
  LL_GPIO_SetPinMode(TIM2_CH1_OC_Port, TIM2_CH1_OC_Pin, LL_GPIO_MODE_ALTERNATE);

    LL_TIM_SetClockSource(TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
    LL_TIM_SetPrescaler(TIM2, 0);
    LL_TIM_SetAutoReload(TIM2, 1000);
    LL_TIM_GenerateEvent_UPDATE(TIM2);
    LL_TIM_ClearFlag_UPDATE(TIM2);

    LL_TIM_OC_SetCompareCH1(TIM2, 0);

    LL_TIM_OC_SetMode(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);


    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
  	LL_TIM_EnableCounter(TIM2);


    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
    LL_TIM_EnableIT_UPDATE(TIM2);
  }

void TIM2_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
	{
		LL_TIM_ClearFlag_UPDATE(TIM2);
		if(Mbuffer.isPlaying == 0 || Mbuffer.freq[ Mbuffer.currentNote] == 0) // nic ma nie grać
		{
			TIM2->PSC = 0;
			TIM2->CCR1 = 0;
		}
		else // gra muzyke
		{
			TIM2->PSC = Mbuffer.freq[ Mbuffer.currentNote];
			TIM2->CCR1 = 500;
		}

	}
}
void MusicTest(void)
{
	Mbuffer.musicDuration = 198; // długość utworu

	Mbuffer.isPlaying = 1; // dajemy znać timerowi aby generował muzyke

	uint16_t wholenote = (60000 * 4) / TEMPO;
	for (int i = 0; i < Mbuffer.musicDuration; i=i+2) {
		Mbuffer.freq[i]  = ((TIM_FREQ/(1000*tetrisMelody[i]))-1); // obsługujemy nową nutkę
		 Mbuffer.currentNote = i;

		int8_t divider = tetrisMelody[i + 1];
		int16_t noteDuration = 0;

		if (divider > 0) {
		      noteDuration = (wholenote) / divider;
		    } else if (divider < 0) {
		      noteDuration = (wholenote) / abs(divider);
		      noteDuration *= 1.5;
		    }
		LL_mDelay(noteDuration); // dlugosc nutki
		Mbuffer.isPlaying = 0;
		LL_mDelay(30); // przerwa miedzy nutami
		Mbuffer.isPlaying = 1;

	}
	Mbuffer.isPlaying = 0; // dajemy znać timerowi aby przestał grać muzyke
}
