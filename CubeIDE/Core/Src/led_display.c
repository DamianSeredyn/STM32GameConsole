/*
 * led_display.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Damian
 */
#include "led_display.h"

static uint8_t SPI_ILI9486_Read_Data(uint8_t reg)
{
	uint8_t result;


	SPI_ILI9486_CS_low();
	SPI_ILI9486_DC_low();
	spi_write_data(&reg, 1,SPI1);
	SPI_ILI9486_DC_High();
	spi_read_data(&result, 1,SPI1);
	SPI_ILI9486_CS_High();
	return result;
}
static void SPI_ILI9486_Write_Data(uint8_t reg, uint8_t val)
{
	SPI_ILI9486_CS_low();
	uint8_t tmp[2];
	tmp[0] = reg;
	tmp[1] = val;
	spi_write_data(tmp, 2,SPI1);
	SPI_ILI9486_CS_High();
}

void SPI_ILI9486_init(void)
{



LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
LL_GPIO_InitTypeDef GPIO_InitStructB = {0};
/* Peripheral clock enable */
LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
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

GPIO_InitStructB.Pin = LL_GPIO_PIN_6;
GPIO_InitStructB.Mode = LL_GPIO_MODE_OUTPUT;
GPIO_InitStructB.Speed = LL_GPIO_SPEED_FREQ_LOW;
GPIO_InitStructB.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
GPIO_InitStructB.Pull = LL_GPIO_PULL_NO;
LL_GPIO_Init(GPIOB, &GPIO_InitStructB);

SPI_ILI9486_CS_High();


SPI_TypeDef * spi = SPI1;

LL_SPI_Disable(spi);
LL_SPI_SetMode(spi, LL_SPI_MODE_MASTER);
LL_SPI_SetTransferDirection(spi, LL_SPI_FULL_DUPLEX);
LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
LL_SPI_SetNSSMode(spi, LL_SPI_NSS_SOFT);
LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV8);
LL_SPI_SetTransferBitOrder(spi, LL_SPI_MSB_FIRST);
LL_SPI_SetDataWidth(spi, LL_SPI_DATAWIDTH_8BIT);
LL_SPI_SetStandard(spi, LL_SPI_PROTOCOL_MOTOROLA);
LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);
LL_SPI_Enable(spi);



}

void ILI9486_Init(void)
{

	LL_GPIO_InitTypeDef GPIO_InitStructA = {0};
	LL_GPIO_InitTypeDef GPIO_InitStructC = {0};

	GPIO_InitStructA.Pin = ILI9486_DC_PIN|ILI9486_RST_PIN;
	GPIO_InitStructA.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStructA.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructA.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStructA.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStructA);

	GPIO_InitStructC.Pin = ILI9486_BL_PIN;
	GPIO_InitStructC.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStructC.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructC.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStructC.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStructC);

    ILI9486_Reset();

	LL_GPIO_SetOutputPin(GPIOA, ILI9486_DC_PIN);
	LL_GPIO_SetOutputPin(GPIOA, ILI9486_RST_PIN);
	LL_GPIO_SetOutputPin(GPIOC, ILI9486_BL_PIN);
}

void SPI_ILI9486_CS_low(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
}
void SPI_ILI9486_CS_High(void)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void SPI_ILI9486_DC_low(void)
{
	LL_GPIO_ResetOutputPin(ILI9486_DC_PORT, ILI9486_DC_PIN);
}
void SPI_ILI9486_DC_High(void)
{
	LL_GPIO_SetOutputPin(ILI9486_DC_PORT, ILI9486_DC_PIN);
}

void ILI9486_Reset(void)
{
    LL_GPIO_ResetOutputPin(GPIOA, ILI9486_RST_PIN);
    LL_mDelay(10);
    LL_GPIO_SetOutputPin(GPIOA, ILI9486_RST_PIN);
    LL_mDelay(120);
}

uint8_t ILI9486_ReadStatus(void)
{
	uint8_t status =0;
	status = SPI_ILI9486_Read_Data(ILI9486_READ_STATUS);
	return status;
}

uint8_t ILI9486_ReadID(void)
{
	uint8_t id =0;
	id = SPI_ILI9486_Read_Data(ILI9486_READ_ID);
	return id;
}




