/*
 * led_display.c
 *
 *  Created on: Dec 12, 2024
 *      Author: Damian
 */
#include "led_display.h"

LCD_DIS sLCD_DIS;
// Wysyłanie komendy do wyświetlacza
static void SPI_ILI9486_WriteReg(uint8_t Reg)
{
    SPI_ILI9486_DC_low();       // Komenda (D/CX = Low)
    SPI_ILI9486_CS_low();       // CS = Low
    spi_write_data(&Reg, 1, SPI1);
    SPI_ILI9486_CS_High();      // CS = High
}

// Wysyłanie danych do wyświetlacza
static void SPI_ILI9486_WriteData(uint8_t Data)
{
    SPI_ILI9486_DC_High();
    SPI_ILI9486_CS_low();
	uint8_t data1 = Data >> 8;
	uint8_t data2 = Data & 0XFF;
	spi_write_data(&data1,1,SPI1);
	spi_write_data(&data2,1,SPI1);
    SPI_ILI9486_CS_High();
}
static void SPI_ILI9486_AllData(uint16_t Data, uint32_t DataLen)
{
    uint32_t i;
    SPI_ILI9486_DC_High();
    SPI_ILI9486_CS_low();
    for(i = 0; i < DataLen; i++) {
    	uint8_t data1 = Data >> 8;
    	uint8_t data2 = Data & 0XFF;
    	spi_write_data(&data1,1,SPI1);
    	spi_write_data(&data2,1,SPI1);
    }
    SPI_ILI9486_CS_High();
}
// Odczytywanie danych do wyświetlacza
static void SPI_ILI9486_Read_Data(uint8_t* result, uint8_t size)
{
	SPI_ILI9486_CS_low();
	SPI_ILI9486_DC_High();
	spi_read_data(result, size,SPI1);
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
LL_SPI_SetBaudRatePrescaler(spi, LL_SPI_BAUDRATEPRESCALER_DIV2);
LL_SPI_SetTransferBitOrder(spi, LL_SPI_MSB_FIRST);
LL_SPI_SetDataWidth(spi, LL_SPI_DATAWIDTH_8BIT);
LL_SPI_SetStandard(spi, LL_SPI_PROTOCOL_MOTOROLA);
LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);
LL_SPI_Enable(spi);



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


static void LCD_InitReg(void)
{
    SPI_ILI9486_WriteReg(0XF9);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x08);

    SPI_ILI9486_WriteReg(0xC0);
    SPI_ILI9486_WriteData(0x19);//VREG1OUT POSITIVE
    SPI_ILI9486_WriteData(0x1a);//VREG2OUT NEGATIVE

    SPI_ILI9486_WriteReg(0xC1);
    SPI_ILI9486_WriteData(0x45);//VGH,VGL    VGH>=14V.
    SPI_ILI9486_WriteData(0x00);

    SPI_ILI9486_WriteReg(0xC2);	//Normal mode, increase can change the display quality, while increasing power consumption
    SPI_ILI9486_WriteData(0x44);

    SPI_ILI9486_WriteReg(0XC5);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x28);//VCM_REG[7:0]. <=0X80.

    SPI_ILI9486_WriteReg(0xB1);//Sets the frame frequency of full color normal mode
    SPI_ILI9486_WriteData(0xE0);//0XB0 =70HZ, <=0XB0.0xA0=62HZ
    SPI_ILI9486_WriteData(0x1F);

    SPI_ILI9486_WriteReg(0xB4);
    SPI_ILI9486_WriteData(0x02); //2 DOT FRAME MODE,F<=70HZ.

    SPI_ILI9486_WriteReg(0xB6);//
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x42);//0 GS SS SM ISC[3:0];
    SPI_ILI9486_WriteData(0x3B);

    SPI_ILI9486_WriteReg(0xB7);
    SPI_ILI9486_WriteData(0x07);

    SPI_ILI9486_WriteReg(0xE0);
    SPI_ILI9486_WriteData(0x1F);
    SPI_ILI9486_WriteData(0x25);
    SPI_ILI9486_WriteData(0x22);
    SPI_ILI9486_WriteData(0x0B);
    SPI_ILI9486_WriteData(0x06);
    SPI_ILI9486_WriteData(0x0A);
    SPI_ILI9486_WriteData(0x4E);
    SPI_ILI9486_WriteData(0xC6);
    SPI_ILI9486_WriteData(0x39);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x00);

    SPI_ILI9486_WriteReg(0XE1);
    SPI_ILI9486_WriteData(0x1F);
    SPI_ILI9486_WriteData(0x3F);
    SPI_ILI9486_WriteData(0x3F);
    SPI_ILI9486_WriteData(0x0F);
    SPI_ILI9486_WriteData(0x1F);
    SPI_ILI9486_WriteData(0x0F);
    SPI_ILI9486_WriteData(0x46);
    SPI_ILI9486_WriteData(0x49);
    SPI_ILI9486_WriteData(0x31);
    SPI_ILI9486_WriteData(0x05);
    SPI_ILI9486_WriteData(0x09);
    SPI_ILI9486_WriteData(0x03);
    SPI_ILI9486_WriteData(0x1C);
    SPI_ILI9486_WriteData(0x1A);
    SPI_ILI9486_WriteData(0x00);

    SPI_ILI9486_WriteReg(0XF1);
    SPI_ILI9486_WriteData(0x36);
    SPI_ILI9486_WriteData(0x04);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x3C);
    SPI_ILI9486_WriteData(0x0F);
    SPI_ILI9486_WriteData(0x0F);
    SPI_ILI9486_WriteData(0xA4);
    SPI_ILI9486_WriteData(0x02);

    SPI_ILI9486_WriteReg(0XF2);
    SPI_ILI9486_WriteData(0x18);
    SPI_ILI9486_WriteData(0xA3);
    SPI_ILI9486_WriteData(0x12);
    SPI_ILI9486_WriteData(0x02);
    SPI_ILI9486_WriteData(0x32);
    SPI_ILI9486_WriteData(0x12);
    SPI_ILI9486_WriteData(0xFF);
    SPI_ILI9486_WriteData(0x32);
    SPI_ILI9486_WriteData(0x00);

    SPI_ILI9486_WriteReg(0XF4);
    SPI_ILI9486_WriteData(0x40);
    SPI_ILI9486_WriteData(0x00);
    SPI_ILI9486_WriteData(0x08);
    SPI_ILI9486_WriteData(0x91);
    SPI_ILI9486_WriteData(0x04);

    SPI_ILI9486_WriteReg(0XF8);
    SPI_ILI9486_WriteData(0x21);
    SPI_ILI9486_WriteData(0x04);

    SPI_ILI9486_WriteReg(0X3A);	//Set Interface Pixel Format
    SPI_ILI9486_WriteData(0x55);

}

/********************************************************************************
function:	Set the display scan and color transfer modes
parameter:
		Scan_dir   :   Scan direction
		Colorchose :   RGB or GBR color format
********************************************************************************/
void LCD_SetGramScanWay(LCD_SCAN_DIR Scan_dir)
{
    uint16_t MemoryAccessReg_Data = 0; //addr:0x36
    uint16_t DisFunReg_Data = 0; //addr:0xB6

    // Gets the scan direction of GRAM
    switch (Scan_dir) {
    case L2R_U2D:
        MemoryAccessReg_Data = 0x08;//0x08 | 0X8
        DisFunReg_Data = 0x22;
        break;
    case L2R_D2U:
        MemoryAccessReg_Data = 0x08;
        DisFunReg_Data = 0x62;
        break;
    case R2L_U2D: //0X4
        MemoryAccessReg_Data = 0x08;
        DisFunReg_Data = 0x02;
        break;
    case R2L_D2U: //0XC
        MemoryAccessReg_Data = 0x08;
        DisFunReg_Data = 0x42;
        break;
    case U2D_L2R: //0X2
        MemoryAccessReg_Data = 0x28;
        DisFunReg_Data = 0x22;
        break;
    case U2D_R2L: //0X6
        MemoryAccessReg_Data = 0x28;
        DisFunReg_Data = 0x02;
        break;
    case D2U_L2R: //0XA
        MemoryAccessReg_Data = 0x28;
        DisFunReg_Data = 0x62;
        break;
    case D2U_R2L: //0XE
        MemoryAccessReg_Data = 0x28;
        DisFunReg_Data = 0x42;
        break;
    }

    //Get the screen scan direction
    sLCD_DIS.LCD_Scan_Dir = Scan_dir;

    //Get GRAM and LCD width and height
    if(Scan_dir == L2R_U2D || Scan_dir == L2R_D2U || Scan_dir == R2L_U2D || Scan_dir == R2L_D2U) {
        sLCD_DIS.LCD_Dis_Column	= LCD_HEIGHT ;
        sLCD_DIS.LCD_Dis_Page = LCD_WIDTH ;
    } else {
        sLCD_DIS.LCD_Dis_Column	= LCD_WIDTH ;
        sLCD_DIS.LCD_Dis_Page = LCD_HEIGHT ;
    }

    // Set the read / write scan direction of the frame memory
    SPI_ILI9486_WriteReg(0xB6);
    SPI_ILI9486_WriteData(0X00);
    SPI_ILI9486_WriteData(DisFunReg_Data);

    SPI_ILI9486_WriteReg(0x36);
    SPI_ILI9486_WriteData(MemoryAccessReg_Data);
}

/********************************************************************************
function:
	initialization
********************************************************************************/
void ILI9486_Init(LCD_SCAN_DIR LCD_ScanDir)
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


	LL_GPIO_SetOutputPin(GPIOA, ILI9486_DC_PIN);
	LL_GPIO_SetOutputPin(GPIOA, ILI9486_RST_PIN);
	LL_GPIO_SetOutputPin(GPIOC, ILI9486_BL_PIN);
	    //Set the initialization register
    //Hardware reset
	ILI9486_Reset();

    //Set the initialization register
    LCD_InitReg();

    //Set the display scan and color transfer modes
    LCD_SetGramScanWay( LCD_ScanDir);
    LL_mDelay(200);

    //sleep out
    SPI_ILI9486_WriteReg(0x11);
    LL_mDelay(120);

    //Turn on the LCD display
    SPI_ILI9486_WriteReg(0x29);
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
	Xstart 	:   X direction Start coordinates
	Ystart  :   Y direction Start coordinates
	Xend    :   X direction end coordinates
	Yend    :   Y direction end coordinates
********************************************************************************/
void LCD_SetWindow(POINT Xstart, POINT Ystart,	POINT Xend, POINT Yend)
{
    //set the X coordinates
    SPI_ILI9486_WriteReg(0x2A);
    SPI_ILI9486_WriteData(Xstart >> 8);	 				//Set the horizontal starting point to the high octet
    SPI_ILI9486_WriteData(Xstart & 0xff);	 				//Set the horizontal starting point to the low octet
    SPI_ILI9486_WriteData((Xend - 1) >> 8);	//Set the horizontal end to the high octet
    SPI_ILI9486_WriteData((Xend - 1) & 0xff);	//Set the horizontal end to the low octet

    //set the Y coordinates
    SPI_ILI9486_WriteReg(0x2B);
    SPI_ILI9486_WriteData(Ystart >> 8);
    SPI_ILI9486_WriteData(Ystart & 0xff );
    SPI_ILI9486_WriteData((Yend - 1) >> 8);
    SPI_ILI9486_WriteData((Yend - 1) & 0xff);
    SPI_ILI9486_WriteReg(0x2C);
}

/********************************************************************************
function:	Set the display point (Xpoint, Ypoint)
parameter:
	xStart :   X direction Start coordinates
	xEnd   :   X direction end coordinates
********************************************************************************/
void LCD_SetCursor(POINT Xpoint, POINT Ypoint)
{
    LCD_SetWindow(Xpoint, Ypoint, Xpoint, Ypoint);
}

/********************************************************************************
function:	Set show color
parameter:
		Color  :   Set show color,16-bit depth
********************************************************************************/
//static void LCD_SetColor(LENGTH Dis_Width, LENGTH Dis_Height, COLOR Color ){
void LCD_SetColor(COLOR Color , POINT Xpoint, POINT Ypoint)
{
	SPI_ILI9486_AllData(Color , (uint32_t)Xpoint * (uint32_t)Ypoint);
}

/********************************************************************************
function:	Point (Xpoint, Ypoint) Fill the color
parameter:
	Xpoint :   The x coordinate of the point
	Ypoint :   The y coordinate of the point
	Color  :   Set the color
********************************************************************************/
void LCD_SetPointlColor( POINT Xpoint, POINT Ypoint, COLOR Color)
{
    if ((Xpoint <= sLCD_DIS.LCD_Dis_Column) && (Ypoint <= sLCD_DIS.LCD_Dis_Page)) {
        LCD_SetCursor (Xpoint, Ypoint);
        LCD_SetColor(Color, 1, 1);
    }
}

/********************************************************************************
function:	Fill the area with the color
parameter:
	Xstart :   Start point x coordinate
	Ystart :   Start point y coordinate
	Xend   :   End point coordinates
	Yend   :   End point coordinates
	Color  :   Set the color
********************************************************************************/
void LCD_SetArealColor(POINT Xstart, POINT Ystart, POINT Xend, POINT Yend,	COLOR Color)
{
    if((Xend > Xstart) && (Yend > Ystart)) {
        LCD_SetWindow(Xstart , Ystart , Xend , Yend  );
        LCD_SetColor ( Color , Xend - Xstart, Yend - Ystart);
    }
}

/********************************************************************************
function:
			Clear screen
********************************************************************************/
void LCD_Clear(COLOR  Color)
{
    LCD_SetArealColor(0, 0, sLCD_DIS.LCD_Dis_Column , sLCD_DIS.LCD_Dis_Page , Color);
}

