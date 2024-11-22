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

typedef struct
{
uint8_t* data_ptr;
uint32_t count;
} buffer_tt;

void MX_SPI2_Init(void);

void spi_init_interupts(void);
void spi_write_data_it(uint8_t *data, uint32_t size);
void spi_read_data_it(uint8_t *data, uint32_t size);
void spi_transmit_callback(void);
void spi_receive_callback(void);
void spi_cs_set_high(void);
void spi_cs_set_low(void);
void spi_cs2_set_high(void);
void spi_cs2_set_low(void);
void spi_cs3_set_high(void);
void spi_cs3_set_low(void);

void spi_write_data(uint8_t *data, uint32_t size);
void spi_read_data(uint8_t *data, uint32_t size);


#endif /* INC_SPI_H_ */
