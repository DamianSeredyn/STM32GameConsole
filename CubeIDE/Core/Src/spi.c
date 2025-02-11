/*
 * spi.c
 *
 *  Created on: Nov 10, 2024
 *      Author: szymo
 */
#include "spi.h"

#define DUMMY_BYTE 0xFF

SPI_TypeDef * spi = SPI2;


static buffer_f rx_buffer = {0};
static buffer_f tx_buffer = {0};

void MX_SPI2_Init(void)
{

  /**SPI2 GPIO Configuration
   *
  PB11   ------> SPI2_NSS2
  PB12   ------> SPI2_NSS
  PB13   ------> SPI2_SCK
  PB14   ------> SPI2_MISO
  PB15   ------> SPI2_MOSI
  */
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

	spi_cs_set_high();
	spi_cs2_set_high();

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

		LL_SPI_Disable(spi);
		LL_SPI_SetMode(spi, LL_SPI_MODE_MASTER);
		LL_SPI_SetTransferDirection(spi, LL_SPI_FULL_DUPLEX);
		LL_SPI_SetClockPolarity(spi, LL_SPI_POLARITY_LOW);
		LL_SPI_SetClockPhase(spi, LL_SPI_PHASE_1EDGE);
		LL_SPI_SetNSSMode(spi, LL_SPI_NSS_SOFT);

		 // changed to low value for SD card, it is later changed to DIV2 in SD init
		LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV256);
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

void spi_write_data_it(uint8_t *data, uint32_t size)
{
	tx_buffer.data_ptr = data;
	tx_buffer.count = size;

	//for echo
	rx_buffer.count = size;

	LL_SPI_EnableIT_TXE(SPI2);
	LL_SPI_Enable(SPI2);
}

void spi_read_data_it(uint8_t *data, uint32_t size)
{
	tx_buffer.count = size;

	rx_buffer.data_ptr = data;
	rx_buffer.count = size;

	LL_SPI_EnableIT_TXE(SPI2);
	LL_SPI_EnableIT_RXNE(SPI2);
	LL_SPI_Enable(SPI2);
}

void SPI2_IRQHandler(void)
{
	if(LL_SPI_IsActiveFlag_TXE(SPI2) && LL_SPI_IsEnabledIT_TXE(SPI2))
	{
		spi_transmit_callback();
	}
	if(LL_SPI_IsActiveFlag_RXNE(SPI2) && LL_SPI_IsEnabledIT_RXNE(SPI2))
	{
		spi_receive_callback();
	}
}

void spi_transmit_callback(void)
{
	if(tx_buffer.count > 0)
	{
		if(!LL_SPI_IsEnabledIT_RXNE(SPI2))
		{
			LL_SPI_TransmitData8(SPI2, *tx_buffer.data_ptr);

			//echo
			rx_buffer.data_ptr = tx_buffer.data_ptr;
			rx_buffer.data_ptr++;
			rx_buffer.count--;
			// end echo

			tx_buffer.data_ptr++;
			tx_buffer.count--;
		}
		else
		{
			LL_SPI_TransmitData8(SPI2, DUMMY_BYTE);
			tx_buffer.count--;
		}
	}
	if(tx_buffer.count <= 0 && !LL_SPI_IsEnabledIT_RXNE(SPI2))
	{
		LL_SPI_DisableIT_TXE(SPI2);

		while (LL_SPI_GetTxFIFOLevel(SPI2) != LL_SPI_TX_FIFO_EMPTY)
			;

		while (LL_SPI_IsActiveFlag_BSY(SPI2) != 0)
			;

		LL_SPI_Disable(SPI2);

		while (LL_SPI_GetRxFIFOLevel(SPI2) != LL_SPI_RX_FIFO_EMPTY)
		{
			LL_SPI_ReceiveData8(SPI2);
		}

		LL_SPI_ClearFlag_OVR(SPI2);
	}
}

void spi_receive_callback(void)
{
	if(rx_buffer.count > 0)
	{
		*rx_buffer.data_ptr = LL_SPI_ReceiveData8(SPI2);

		rx_buffer.data_ptr++;
		rx_buffer.count--;
	}

	if(rx_buffer.count <= 0)
	{
		LL_SPI_DisableIT_RXNE(SPI2);
		LL_SPI_DisableIT_TXE(SPI2);

		while (LL_SPI_GetTxFIFOLevel(SPI2) != LL_SPI_TX_FIFO_EMPTY);

		while (LL_SPI_IsActiveFlag_BSY(SPI2) != 0);

		LL_SPI_Disable(SPI2);

		while (LL_SPI_GetRxFIFOLevel(SPI2) != LL_SPI_RX_FIFO_EMPTY)
		{
			LL_SPI_ReceiveData8(SPI2);
		}

		LL_SPI_ClearFlag_OVR(SPI2);

	}
}

void spi_cs_set_high(void)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_12);
}

void spi_cs_set_low(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_12);
}

void spi_cs2_set_high(void)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_11);
}

void spi_cs2_set_low(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_11);
}

void spi_cs3_set_high(void)
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
}

void spi_cs3_set_low(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
}

void spi_write_data(uint8_t *data, uint32_t size)
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

void spi_read_data(uint8_t *data, uint32_t size)
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

void spi_it_transmit_callback(void)
{
	if(tx_buffer.count > 0)
	{
		if(!LL_SPI_IsEnabledIT_RXNE(spi))
		{
			LL_SPI_TransmitData8(spi, *tx_buffer.data_ptr);

			tx_buffer.data_ptr++;
			tx_buffer.count--;
		}
		else
		{
			LL_SPI_TransmitData8(spi, DUMMY_BYTE);
			tx_buffer.count--;
		}
	}

	if(tx_buffer.count <= 0 && !LL_SPI_IsEnabledIT_RXNE(spi))
	{
		LL_SPI_DisableIT_TXE(spi);

		while (LL_SPI_GetTxFIFOLevel(spi) != LL_SPI_TX_FIFO_EMPTY)
			;

		while (LL_SPI_IsActiveFlag_BSY(spi) != 0)
			;

		LL_SPI_Disable(spi);

		while (LL_SPI_GetRxFIFOLevel(spi) != LL_SPI_RX_FIFO_EMPTY)
		{
			LL_SPI_ReceiveData8(spi);
		}

		LL_SPI_ClearFlag_OVR(spi);

		spi_cs_set_high();
	}
}

void spi_it_receive_callback(void)
{
	if(rx_buffer.count > 0)
	{
		*rx_buffer.data_ptr = LL_SPI_ReceiveData8(spi);

		rx_buffer.data_ptr++;
		rx_buffer.count--;
	}

	if(rx_buffer.count <= 0)
	{
		LL_SPI_DisableIT_RXNE(spi);
		LL_SPI_DisableIT_TXE(spi);

		while (LL_SPI_GetTxFIFOLevel(spi) != LL_SPI_TX_FIFO_EMPTY)
			;

		while (LL_SPI_IsActiveFlag_BSY(spi) != 0)
			;

		LL_SPI_Disable(spi);

		while (LL_SPI_GetRxFIFOLevel(spi) != LL_SPI_RX_FIFO_EMPTY)
		{
			LL_SPI_ReceiveData8(spi);
		}

		LL_SPI_ClearFlag_OVR(spi);

		spi_cs_set_high();
	}
}
