/*
 * buzzer.c
 *
 *  Created on: Nov 22, 2024
 *      Author: Damian
 */

#include "buzzer.h"

uint16_t melody[] = {
    262, // C4 (do)
    294, // D4 (re)
    330, // E4 (mi)
    349, // F4 (fa)
    392, // G4 (sol)
    440, // A4 (la)
    494, // B4 (si)
    523,
	0// C5 (do - oktawa wyżej)
};
uint16_t durations[] = {
    500, 500, 500, 500, 500, 500, 500, 500
};


uint16_t freq[9] = {0};
uint16_t duty[9] = {0};
void MusicTest(void)
{
	for (int i = 0; i < 9; i++) {
		freq[i]  = (1000000 / melody[i]);
		duty[i]  = (1000000 / melody[i])/2;
	}
	TIM2->CCR1 = 1908;
	GenerateMusicToDma(freq,9,duty,9);
}


void playNote(uint16_t frequency) {
        uint32_t ARR = (1000000 / frequency);
}


void GenerateMusicToDma(uint16_t* data,uint32_t size,uint16_t* CCR1,uint32_t sizeCCR1)
{

	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_5, (uint32_t)data, (uint32_t)&TIM2->ARR, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, size);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

}
