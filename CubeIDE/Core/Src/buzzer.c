/*
 * buzzer.c
 *
 *  Created on: Nov 22, 2024
 *      Author: Damian
 */

#include "buzzer.h"

const uint32_t melody[] = {
		 NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
		  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
		  NOTE_B4, 4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
		  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

		  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
		  NOTE_E5, 4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
		  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
		  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, 0, 4,

		  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
		  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
		  NOTE_B4, 4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
		  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

		  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
		  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
		  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
		  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, 0, 4,


		  NOTE_E5,2,  NOTE_C5,2,
		  NOTE_D5,2,   NOTE_B4,2,
		  NOTE_C5,2,   NOTE_A4,2,
		  NOTE_GS4,2,  NOTE_B4,4,  0,8,
		  NOTE_E5,2,   NOTE_C5,2,
		  NOTE_D5,2,   NOTE_B4,2,
		  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
		  NOTE_GS5,2,
};





const uint32_t melodySizes[] ={sizeof(melody)/sizeof(uint32_t)};













void CalculateFreq(uint32_t newFreq)
{
	uint64_t tempFreq = newFreq;
		if(newFreq == 0) tempFreq = 1;

		uint64_t tempNewValue = (uint64_t) CPU_FREQ / PRESCALER / tempFreq;

		TIM2 ->ARR = (uint32_t)tempNewValue;
		TIM2 -> CCR1 = (uint32_t)tempNewValue/2;
}


void PlayMusic(uint8_t audio_id)
{
	  TIM2 -> CR1 |= TIM_CR1_CEN;
	  TIM2 ->CCER |= TIM_CCER_CC1E;


		  for(int noteIndex = 0; noteIndex < melodySizes[0]-1; noteIndex += 2)
	  	  {
			  CalculateFreq(melody[noteIndex]);
		  	  LL_mDelay(90*6);
	  	}
	   TIM2 -> CR1 &= ~TIM_CR1_CEN;
	   TIM2 ->CCER &= ~TIM_CCER_CC1E;
}















