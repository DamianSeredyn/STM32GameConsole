/*
 * led_display.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Damian
 */
#include "led_display.h"


void SPI_Led_init(void)
{
/* USER CODE BEGIN SPI1_Init 0 */

/* USER CODE END SPI1_Init 0 */

LL_SPI_InitTypeDef SPI_InitStruct = {0};

LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

/* Peripheral clock enable */
LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
/**SPI1 GPIO Configuration
PA5   ------> SPI1_SCK
PA6   ------> SPI1_MISO
PA7   ------> SPI1_MOSI
*/
GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN SPI1_Init 1 */

/* USER CODE END SPI1_Init 1 */
/* SPI1 parameter configuration*/
SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
SPI_InitStruct.CRCPoly = 7;
LL_SPI_Init(SPI1, &SPI_InitStruct);
LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
LL_SPI_EnableNSSPulseMgt(SPI1);
/* USER CODE BEGIN SPI1_Init 2 */

/* USER CODE END SPI1_Init 2 */
}

uint8_t Led_ReadStatus(void)
{
	uint8_t status =0;


	return status;
}
}
