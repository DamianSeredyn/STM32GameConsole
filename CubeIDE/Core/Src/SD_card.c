#include "SD_card.h"

// use first hex of the command line to get CRC7
uint8_t get_CRC7(uint16_t data){
	data = data << 7;
	data = data / 0x89;
	data = data << 1;
	data = data + 1;
	return data;
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
	/////////////// Check if SD card got initialized correctly
	uint8_t read_loop = 0;
	while (response != 0x01){
		spi_read_data(&response, sizeof(response));
		read_loop++;
		if (read_loop > 10){
			break;
		}
	}
	/////////////////////////////////////////////////////////

	// Check voltage, In short check if SD card can use 3.3V
	table[0] = 0x48; table[1] = 0x00; table[2] = 0x00; table[3] = 0x01; table[4] = 0xAA; table[5] = get_CRC7(table[0]);

	spi_write_data(table, sizeof(table));
	read_loop = 0;
	response = 0x00;
	//check if command worked, should also return check pattern (0xAA) but who cares about checking that
	while (response != 0x01){
		spi_read_data(&response, sizeof(response));
		read_loop++;
		if (read_loop > 10){
			break;
		}
	}
	/////////////////////////////////////////////////////////

	// Supposed to put SD into working mode, doesn't work idk why. Needs fixing: response is 0x05
	table[0] = 0x69; table[1] = 0x50; table[2] = 0xFF; table[3] = 0x80; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
		read_loop = 0;
		response = 0x00;
		LL_mDelay(100);
		while (response == 0x01){
			spi_read_data(&response, sizeof(response));
			read_loop++;
			if (read_loop > 10){
				break;
			}
		}
		LL_mDelay(100);
		spi_write_data(table, sizeof(table));
			read_loop = 0;
			response = 0x00;
			LL_mDelay(100);
			while (response == 0x01){
				spi_read_data(&response, sizeof(response));
				read_loop++;
				if (read_loop > 10){
					break;
				}
			}

	spi_cs2_set_high();
}

