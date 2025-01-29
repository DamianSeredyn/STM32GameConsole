/*
 * spi.h
 *
 *  Created on: Nov 10, 2024
 *      Author: szymo
 */
#include "main.h"

#ifndef INC_SPI_H_
#define INC_SPI_H_

typedef struct
{
	uint8_t* data_ptr;
	uint32_t count;

} buffer_f;
typedef enum
{
	TRANSMIT = 0,
	RECEIVE = 1
}transfer_type_t;
typedef struct
{
uint8_t* data_ptr;
uint32_t count;
} buffer_tt;

void MX_SPI2_Init(void);

void spi_init_interupts(void);
void spi_write_data_it(uint8_t *data, uint32_t size, SPI_TypeDef * spi);
void spi_read_data_it(uint8_t *data, uint32_t size, SPI_TypeDef * spi);
void spi_transmit_callback(SPI_TypeDef * spi);
void spi_receive_callback(SPI_TypeDef * spi);
void spi_it_transmit_callback(SPI_TypeDef * spi);
void spi_it_receive_callback( SPI_TypeDef * spi);
void spi_write_data(uint8_t *data, uint32_t size, SPI_TypeDef * spi);
void spi_read_data(uint8_t *data, uint32_t size, SPI_TypeDef * spi);
void spi_init_DMA(void);
void spi_write_data_dma(uint8_t *data, uint32_t size);
void spi_dma_transmit_callback(void);
void spi_dma_receive_callback(void);

#endif /* INC_SPI_H_ */
