/*
 * spi.c
 *
 *  Created on: Nov 10, 2024
 *      Author: szymo
 */
#include "spi.h"

#define DUMMY_BYTE 0xFF




static buffer_f rx_buffer = {0};
static buffer_f tx_buffer = {0};

void MX_SPI2_Init(void)
{
	SPI_TypeDef * spi = SPI2;
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

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

void spi_init_interupts(void)
{
	NVIC_SetPriority(SPI2_IRQn, 0);
	NVIC_EnableIRQ(SPI2_IRQn);
}

void spi_init_DMA(void)
{
		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_1);
	  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_HIGH);
	  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);
	  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);
	  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);
	  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);
	  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);
	  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
	  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&SPI1->DR);

}
void SPI2_IRQHandler(void)
{
	if(LL_SPI_IsActiveFlag_TXE(SPI2) && LL_SPI_IsEnabledIT_TXE(SPI2))
	{
		//spi_it_transmit_callback(SPI2);
	}

	if(LL_SPI_IsActiveFlag_RXNE(SPI2) && LL_SPI_IsEnabledIT_RXNE(SPI2))
	{
		//spi_it_receive_callback(SPI2);
	}
}
void spi_write_data(uint8_t *data, uint32_t size, SPI_TypeDef * spi)
{
	uint32_t count = 0;


	while(count < size)
	{
		while (!LL_SPI_IsActiveFlag_TXE(spi))
			;

		LL_SPI_TransmitData8(spi, *(data+count));

		count++;
	}

	while (LL_SPI_GetTxFIFOLevel(spi) != LL_SPI_TX_FIFO_EMPTY)
		;

	while (LL_SPI_IsActiveFlag_BSY(spi) != 0)
		;


	while (LL_SPI_GetRxFIFOLevel(spi) != LL_SPI_RX_FIFO_EMPTY)
	{
		LL_SPI_ReceiveData8(spi);
	}

	LL_SPI_ClearFlag_OVR(spi);
}

void spi_read_data(uint8_t *data, uint32_t size, SPI_TypeDef * spi)
{
	uint32_t count = 0;


	while(count < size)
	{
		while (!LL_SPI_IsActiveFlag_TXE(spi))
			;

		LL_SPI_TransmitData8(spi, DUMMY_BYTE);

		while (!LL_SPI_IsActiveFlag_RXNE(spi))
			;

		*(data+count) = LL_SPI_ReceiveData8(spi);

		count++;
	}

	while (LL_SPI_GetTxFIFOLevel(spi) != LL_SPI_TX_FIFO_EMPTY)
		;

	while (LL_SPI_IsActiveFlag_BSY(spi) != 0)
		;


	while (LL_SPI_GetRxFIFOLevel(spi) != LL_SPI_RX_FIFO_EMPTY)
	{
		LL_SPI_ReceiveData8(spi);
	}

	LL_SPI_ClearFlag_OVR(spi);
}

void spi_write_data_dma(uint8_t *data, uint32_t size)
{
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)data);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, size);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);

    LL_SPI_EnableDMAReq_TX(SPI1);

    while (!LL_DMA_IsActiveFlag_TC3(DMA1));
    LL_DMA_ClearFlag_TC3(DMA1);

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
}

