#include "SD_card.h"

// use first hex of the command line to get CRC7
uint8_t get_CRC7(uint16_t data){
	data = data << 7;
	data = data / 0x89;
	data = data << 1;
	data = data + 1;
	return data;
}

void SD_write(){
}

void change_SPI_speed(){
	spi_cs2_set_high();
	LL_SPI_Disable(SPI2);
	LL_SPI_SetBaudRatePrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV2);
	LL_SPI_Enable(SPI2);
	spi_cs2_set_low();
}

uint8_t read_response(uint8_t answer1, uint8_t answer2){
	uint8_t response;
	uint8_t read_loop = 0;
	while (read_loop < 10){
		spi_read_data(&response, sizeof(response));
		if(response != answer1 && response != answer2 && response != 0xFF && response != 0x00){
			return 1;
		}
		read_loop++;
	}
	return 0;
}

void SD_card_init(){
	uint8_t wake_up[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	uint8_t response;
	//Wake up SD card
	spi_write_data(wake_up, sizeof(wake_up));
	spi_cs2_set_low();

	// Init SPI mode
	uint8_t table[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95}; // special CRC needed do not change
	spi_write_data(table, sizeof(table));
	// Check if SD card got initialized correctly
	read_response(0x01, 0xFF);

	// Check voltage, In short check if SD card can use 3.3V, needed for something
	//CMD8
	table[0] = 0x48; table[1] = 0x00; table[2] = 0x00; table[3] = 0x01; table[4] = 0xAA; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0x01, 0xAA);

	// CMD58 - used to check card type and supported voltage (we got 0x01 so CMD16 is not needed and voltage value 0xFF and 0x80)
	table[0] = 0x7A; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0x01, 0x80);

	change_SPI_speed(); //needed for ACMD41 to work

	// CMD55 - needed for ACMD41 command to work (initializes special command or something, honestly idk)
	table[0] = 0x77; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);

	// ACMD41 - used to initialize card (power mode, voltage)
	table[0] = 0x69; table[1] = 0x50; table[2] = 0xFF; table[3] = 0x80; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);

	// CMD13 - used to check ready status, should return 0x00, 0x00
	table[0] = 0x4D; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);

	spi_cs2_set_high();
}

