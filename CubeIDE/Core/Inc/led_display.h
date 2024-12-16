/*
 * led_display.h
 *
 *  Created on: Dec 12, 2024
 *      Author: Damian
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_


#include "main.h"
#include "fonts.h"
void SPI_ILI9486_init(void);

void SPI_ILI9486_CS_low(void);
void SPI_ILI9486_CS_High(void);
void SPI_ILI9486_DC_low(void);
void SPI_ILI9486_DC_High(void);
void ILI9486_Init(void);
void ILI9486_Reset(void);
uint8_t ILI9486_ReadID(void);
uint8_t ILI9486_ReadStatus(void);




#define ILI9486_BL_PIN LL_GPIO_PIN_7
#define ILI9486_RST_PIN LL_GPIO_PIN_9
#define ILI9486_DC_PIN LL_GPIO_PIN_8

#define ILI9486_BL_PORT GPIOC
#define ILI9486_RST_PORT GPIOA
#define ILI9486_DC_PORT GPIOA

// Komendy podstawowe
#define ILI9486_NOP            0x00  // No Operation
#define ILI9486_SOFT_RESET     0x01  // Soft Reset
#define ILI9486_READ_ID        0x04  // Read Display ID
#define ILI9486_SLEEP_IN       0x10  // Enter Sleep Mode
#define ILI9486_SLEEP_OUT      0x11  // Exit Sleep Mode
#define ILI9486_DISPLAY_OFF    0x28  // Display OFF
#define ILI9486_DISPLAY_ON     0x29  // Display ON

// Konfiguracja pamięci
#define ILI9486_COLUMN_ADDR    0x2A  // Set Column Address
#define ILI9486_PAGE_ADDR      0x2B  // Set Page Address
#define ILI9486_MEMORY_WRITE   0x2C  // Memory Write
#define ILI9486_MEMORY_READ    0x2E  // Memory Read

// Kontrola wyświetlania
#define ILI9486_DISPLAY_INVERSION_OFF  0x20
#define ILI9486_DISPLAY_INVERSION_ON   0x21
#define ILI9486_VERTICAL_SCROLL_DEF    0x33
#define ILI9486_PIXEL_FORMAT           0x3A  // Set Pixel Format

// Ustawienia sterowania
#define ILI9486_POWER_CONTROL_1 0xC0
#define ILI9486_POWER_CONTROL_2 0xC1
#define ILI9486_VCOM_CONTROL    0xC5

// Gamma
#define ILI9486_POSITIVE_GAMMA  0xE0
#define ILI9486_NEGATIVE_GAMMA  0xE1

// Odczytywanie statusu
#define ILI9486_READ_STATUS       0x09
#define ILI9486_READ_POWER_MODE   0x0A
#define ILI9486_READ_MADCTL       0x0B
#define ILI9486_READ_PIXEL_FORMAT 0x0C
#define ILI9486_READ_ID1          0xDA
#define ILI9486_READ_ID2          0xDB
#define ILI9486_READ_ID3          0xDC


#endif /* INC_LED_DISPLAY_H_ */
