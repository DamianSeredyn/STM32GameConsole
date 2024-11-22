/*
 * Delay.c
 *
 *  Created on: Nov 22, 2024
 *      Author: lab1
 */

#include "Delay.h"

void Delay(uint32_t delay_tick)
{
	uint32_t Tick = 0;
    while((4*delay_tick)>Tick){
		Tick++; // Increase Tick and after delay_tick*1us Delay stops
    }
}



