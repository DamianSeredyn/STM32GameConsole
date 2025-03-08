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

typedef struct {
    uint16_t freq[128];
    uint16_t musicDuration;
    uint16_t currentNote;
    uint8_t isPlaying;
    uint8_t noteDuration;
    uint8_t delayBetweenNotes;
} MusicBuffer;



#define TIM2_CH1_OC_Pin LL_GPIO_PIN_0
#define TIM2_CH1_OC_Port GPIOA



void MX_TIM2_Init(void);
void MusicTest(void);
#ifdef __cplusplus
}
#endif
#endif /* INC_BUZZER_H_ */
