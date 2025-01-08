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
#define	COLOR				uint16_t		//The variable type of the color (unsigned short)
#define	POINT				uint16_t		//The type of coordinate (unsigned short)
#define	LENGTH				uint16_t		//The type of coordinate (unsigned short)

/********************************************************************************
function:
		Define the full screen height length of the display
********************************************************************************/
#define LCD_X_MAXPIXEL  480  //LCD width maximum memory
#define LCD_Y_MAXPIXEL  320 //LCD height maximum memory
#define LCD_X	 0
#define LCD_Y	 0

#define LCD_WIDTH  (LCD_X_MAXPIXEL - 2 * LCD_X)  //LCD width
#define LCD_HEIGHT  LCD_Y_MAXPIXEL //LCD height

#define SCAN_DIR_DFT  D2U_L2R  //Default scan direction = L2R_U2D

typedef enum {
    L2R_U2D  = 0,	//The display interface is displayed , left to right, up to down
    L2R_D2U  ,
    R2L_U2D  ,
    R2L_D2U  ,

    U2D_L2R  ,
    U2D_R2L  ,
    D2U_L2R  ,
    D2U_R2L  ,
} LCD_SCAN_DIR;

typedef struct {
    LENGTH LCD_Dis_Column;	//COLUMN
    LENGTH LCD_Dis_Page;	//PAGE
    LCD_SCAN_DIR LCD_Scan_Dir;
    POINT LCD_X_Adjust;		//LCD x actual display position calibration
    POINT LCD_Y_Adjust;		//LCD y actual display position calibration
} LCD_DIS;

void SPI_ILI9486_init(void);
void SPI_ILI9486_CS_low(void);
void SPI_ILI9486_CS_High(void);
void SPI_ILI9486_DC_low(void);
void SPI_ILI9486_DC_High(void);
void ILI9486_Init(LCD_SCAN_DIR LCD_ScanDir);
void ILI9486_Reset(void);

uint8_t ILI9486_ReadID(void);
uint8_t ILI9486_ReadStatus(void);

void LCD_SetWindow(POINT Xstart, POINT Ystart, POINT Xend, POINT Yend);
void LCD_SetCursor(POINT Xpoint, POINT Ypoint);
void LCD_SetColor(COLOR Color ,POINT Xpoint, POINT Ypoint);
void LCD_SetPointlColor(POINT Xpoint, POINT Ypoint, COLOR Color);
void LCD_SetArealColor(POINT Xstart, POINT Ystart, POINT Xend, POINT Yend,COLOR  Color);
void LCD_Clear(COLOR  Color);


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

#define ILI9486_FRAME_RATE_CONTROL 0xB1
#define ILI9486_DISPLAY_FUNCTION_CONTROL 0xB6


#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F




#endif /* INC_LED_DISPLAY_H_ */
