/*
 * buzzer.h
 *
 *  Created on: Nov 22, 2024
 *      Author: Damian
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void playNote(uint16_t frequency);
void MusicTest(void);
void GenerateMusicToDma(uint16_t* data,uint32_t size,uint16_t* CCR1,uint32_t sizeCCR1);
#ifdef __cplusplus
}
#endif
#endif /* INC_BUZZER_H_ */
