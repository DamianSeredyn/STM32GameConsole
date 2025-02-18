#include "SD_card.h"

uint8_t read_response(uint8_t answer1, uint8_t answer2);

// use first hex of the command line(CMD) to get CRC7
uint8_t get_CRC7(uint16_t data){
	data = data << 7;
	data = data / 0x89;
	data = data << 1;
	data = data + 1;
	return data;
}

void SD_write(uint8_t *data_sent,uint8_t sector1,uint8_t sector2,uint8_t sector3,uint8_t sector4){
	uint8_t data[512];

	//cmd24
	uint8_t command[6] = {0x58, sector1, sector2, sector3, sector4, 0x95};

	uint8_t response = 0xFF;
	uint8_t start_data = 0xFE;
	uint8_t test = 0xFF;
	uint8_t crc[2] = {0x00, 0x00};
	spi_cs2_set_low();
	spi_write_data(command, sizeof(command));
	while (response != 0x00){
			spi_read_data(&response, sizeof(response));
			response = response;
		}
	while (response != 0xFF){
			spi_read_data(&response, sizeof(response));
			response = response;
		}

	for(uint8_t i = 0; i < 10; i++){
		spi_write_data(&test, sizeof(test));
	}

	spi_write_data(&start_data, sizeof(start_data));

	for (uint16_t i = 0; i < 512; i++){
		data[i] = data_sent[i];
	}


	spi_write_data(data, sizeof(data));
	spi_write_data(crc, sizeof(crc));

	// returns error idk why but data is written anyways
	while (response != 0x0f){
			spi_read_data(&response, sizeof(response));
		}
	read_response(0xFF, 0xFF);


	spi_cs2_set_high();
}

uint8_t* SD_read_single(uint8_t sector1,uint8_t sector2,uint8_t sector3,uint8_t sector4){
	// CMD17
	uint8_t table[6] = {0x51, sector1, sector2, sector3, sector4, 0x50};
	uint8_t response = 0xFF;
	static uint8_t data[512];
	for (int i = 0; i < 512; i++){
		data[i] = 0xFF;
	}
	spi_cs2_set_low();
	spi_write_data(table, sizeof(table));
	while (response != 0x00){
		spi_read_data(&response, sizeof(response));
	}
	while (response != 0xFE){
		spi_read_data(&response, sizeof(response));
	}

	spi_read_data(data, sizeof(data));
	read_response(0xFF, 0xFF);

	spi_cs2_set_high();
	return data;
}

void change_SPI_speed(){
	spi_cs2_set_high();
	LL_SPI_Disable(SPI2);
	LL_SPI_SetBaudRatePrescaler(SPI2, LL_SPI_BAUDRATEPRESCALER_DIV4);
	LL_SPI_Enable(SPI2);
	spi_cs2_set_low();
}

uint8_t read_response(uint8_t answer1, uint8_t answer2){
	uint8_t response = 0xFF;
	uint8_t read_loop = 0;
	bool error = 0;
	while (read_loop < 10){
		spi_read_data(&response, sizeof(response));
		if(response != answer1 && response != answer2 && response != 0xFF && response != 0x00){
			error = 1;
		}
		read_loop++;
	}
	return error;
}

uint8_t SD_card_init(){
	uint8_t wake_up[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	bool error = 0;
	//Wake up SD card
	spi_write_data(wake_up, sizeof(wake_up));
	spi_cs2_set_low();

	// Init SPI mode
	uint8_t table[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95}; // special CRC needed do not change
	spi_write_data(table, sizeof(table));
	LL_mDelay(1000);
	// Check if SD card got initialized correctly
	error = read_response(0x01, 0xFF);
	spi_cs2_set_high();

	// CMD8 - Check voltage, In short check if SD card can use 3.3V, needed for something
	spi_cs2_set_low();
	table[0] = 0x48; table[1] = 0x00; table[2] = 0x00; table[3] = 0x01; table[4] = 0xAA; table[5] = get_CRC7(table[0]);
	spi_write_data(wake_up, sizeof(wake_up));
	spi_write_data(table, sizeof(table));
	error = read_response(0x01, 0xAA);
	spi_cs2_set_high();

	// CMD55 - needed for all ACMD commands to work (initializes special command)
	for (uint8_t i = 0; i < 100; i ++){
	spi_cs2_set_low();
	spi_write_data(wake_up, sizeof(wake_up));
	table[0] = 0x77; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);
	spi_cs2_set_high();

	// ACMD41 - used to initialize the card
	spi_cs2_set_low();
	spi_write_data(wake_up, sizeof(wake_up));
	table[0] = 0x69; table[1] = 0x40; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);
	spi_cs2_set_high();

	}

	// CMD58 - used to check card type and supported voltage
	table[0] = 0x7A; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	error = read_response(0x01, 0xC0);

	change_SPI_speed();

	// CMD16 to set block length to 512 bytes, might by not needed
	table[0] = 0x50; table[1] = 0x00; table[2] = 0x00; table[3] = 0x02; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	error = read_response(0xFF, 0xFF);


	// CMD13 - used to check status, should return 0x00, 0x00
	table[0] = 0x4D; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	error = read_response(0xFF, 0xFF);

	spi_cs2_set_high();
	return error;
}

uint8_t read_write_test(){
	uint8_t kalosz[512] = {
	    103, 47, 215, 162, 98, 201, 75, 243, 12, 64, 189, 133, 47, 56, 229, 91,
	    177, 88, 144, 255, 202, 35, 176, 71, 14, 194, 209, 3, 168, 121, 60, 27,
	    210, 236, 55, 191, 84, 20, 98, 250, 239, 194, 172, 65, 31, 249, 150, 92,
	    188, 23, 69, 195, 43, 120, 210, 11, 241, 167, 97, 181, 57, 200, 147, 152,
	    30, 101, 223, 74, 13, 184, 253, 40, 112, 139, 242, 94, 215, 37, 185, 67,
	    135, 26, 87, 90, 125, 230, 39, 199, 78, 154, 51, 109, 209, 183, 4, 198,
	    147, 88, 206, 176, 163, 55, 68, 22, 130, 97, 52, 240, 20, 225, 143, 128,
	    39, 91, 233, 187, 108, 41, 253, 97, 174, 145, 247, 222, 94, 79, 21, 254,
	    213, 167, 9, 186, 247, 66, 56, 147, 115, 123, 97, 250, 245, 190, 137, 88,
	    153, 205, 78, 14, 103, 239, 132, 177, 21, 118, 194, 95, 222, 4, 27, 243,
	    174, 108, 145, 29, 248, 219, 92, 143, 132, 201, 183, 220, 35, 5, 170, 44,
	    87, 111, 178, 124, 206, 33, 231, 251, 179, 97, 145, 15, 88, 52, 225, 199,
	    160, 14, 76, 181, 109, 209, 37, 132, 170, 93, 63, 241, 250, 18, 81, 127,
	    101, 201, 43, 204, 98, 157, 39, 224, 18, 62, 76, 244, 135, 8, 119, 111,
	    104, 232, 87, 253, 43, 113, 209, 179, 198, 94, 132, 57, 180, 234, 30, 201,
	    121, 78, 173, 12, 58, 214, 253, 176, 87, 42, 114, 23, 247, 192, 250, 151,
	    140, 109, 97, 64, 209, 202, 48, 134, 125, 35, 184, 172, 238, 19, 62, 214,
	    174, 246, 233, 94, 163, 14, 220, 97, 255, 79, 172, 158, 143, 52, 208, 190,
	    78, 102, 239, 121, 153, 212, 3, 221, 69, 91, 47, 108, 175, 230, 192, 57,
	    118, 35, 128, 56, 134, 219, 143, 251, 76, 142, 67, 28, 99, 215, 204, 37,
	    85, 96, 143, 252, 74, 28, 203, 181, 46, 118, 193, 199, 176, 34, 79, 208,
	    93, 107, 171, 164, 137, 49, 255, 96, 20, 122, 47, 84, 97, 141, 230, 93,
	    217, 87, 182, 38, 61, 141, 114, 183, 54, 109, 251, 201, 175, 72, 220, 133,
	    92, 77, 245, 140, 163, 248, 207, 191, 100, 178, 49, 67, 92, 187, 141, 224,
	    179, 212, 30, 127, 54, 253, 206, 248, 178, 233, 64, 120, 218, 103, 144, 99,
	    187, 156, 91, 242, 167, 131, 245, 221, 18, 76, 50, 192, 149, 85, 113, 40,
	    57, 229, 234, 190, 24, 206, 130, 204, 47, 97, 93, 239, 88, 130, 169, 143,
	    44, 220, 167, 250, 154, 14, 55, 243, 192, 129, 103, 156, 112, 210, 92, 41,
	    138, 118, 200, 62, 137, 174, 109, 87, 243, 169, 158, 133, 99, 182, 57, 155,
	    245, 233, 200, 102, 199, 141, 31, 65, 38, 120, 47, 202, 225, 140, 99, 195,
	    176, 109, 223, 128, 235, 45, 197, 113, 82, 207, 155, 30, 236, 245, 97, 121,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
	};



		SD_write(kalosz,0x00, 0x00, 0x00, 0x01);
		uint8_t* kozak = SD_read_single(0x00, 0x00, 0x00, 0x01);
		for(uint16_t i = 0 ; i < 512; i++){
			if (kozak[i] != kalosz[i]){
				return 1;
			}
		}
		return 0;
}

