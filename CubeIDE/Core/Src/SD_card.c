#include "SD_card.h"

uint8_t read_response(uint8_t answer1, uint8_t answer2);

// use first hex of the command line to get CRC7
uint8_t get_CRC7(uint16_t data){
	data = data << 7;
	data = data / 0x89;
	data = data << 1;
	data = data + 1;
	return data;
}

void SD_write(uint8_t *data_sent){
	uint8_t data[512];
	uint8_t command[6] = {0x58, 0x00, 0x00, 0x00, 0x01, 0x95};
	uint8_t response = 0xFF;
	uint8_t start_data = 0xFE;
	uint8_t test = 0xFF;
	uint8_t crc[2] = {0x00, 0x00};
	uint16_t i = 0;
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

	for(int i = 0; i < 10; i++){
		spi_write_data(&test, sizeof(test));
	}
	spi_write_data(&start_data, sizeof(start_data));
	for (; i < 512; i++){
		data[i] = data_sent[i];
	}

	for (; i < 512; i++){
		data[i] = 0xFF;
	}


	spi_write_data(data, sizeof(data));
	spi_write_data(crc, sizeof(crc));

	while (response != 0x0f){
			spi_read_data(&response, sizeof(response));
			response = response;
		}
	read_response(0xFF, 0xFF);


	spi_cs2_set_high();
}

// odczyt pojedyńczego bloku 512 bitów
void SD_read_single(){
	// CMD17
	uint8_t table[6] = {0x51, 0x00, 0x00, 0x00, 0x01, 0x50};
	uint8_t response = 0xFF;
	uint8_t data[512];
	int counter = 0;
	for (int i = 0; i < 512; i++){
		data[i] = 0xFF;
	}
	spi_cs2_set_low();
	spi_write_data(table, sizeof(table));
	while (response != 0xFE){
		spi_read_data(&response, sizeof(response));
		response = response;
		if (response == 0x00){
			counter++;
		}
	}

	spi_read_data(data, sizeof(data));
	read_response(0xFF, 0xFF);

	spi_cs2_set_high();
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
	while (read_loop < 10){
		spi_read_data(&response, sizeof(response));
		if(response != answer1 && response != answer2 && response != 0xFF && response != 0x00){
			read_loop= read_loop;
			//return 1;
		}
		read_loop++;
	}
	return 0;
}

void SD_card_init(){
	uint8_t wake_up[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	//Wake up SD card
	spi_write_data(wake_up, sizeof(wake_up));
	spi_cs2_set_low();

	// Init SPI mode
	uint8_t table[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95}; // special CRC needed do not change
	spi_write_data(table, sizeof(table));
	LL_mDelay(1000);
	// Check if SD card got initialized correctly
	read_response(0x01, 0xFF);
	spi_cs2_set_high();

	// Check voltage, In short check if SD card can use 3.3V, needed for something
	//CMD8
	spi_cs2_set_low();
	table[0] = 0x48; table[1] = 0x00; table[2] = 0x00; table[3] = 0x01; table[4] = 0xAA; table[5] = get_CRC7(table[0]);
	spi_write_data(wake_up, sizeof(wake_up));
	spi_write_data(table, sizeof(table));
	read_response(0x01, 0xAA);
	spi_cs2_set_high();

	//change_SPI_speed(); //needed for ACMD41 to work

	// CMD55 - needed for ACMD41 command to work (initializes special command or something, honestly idk)
	for (int i = 0; i < 100; i ++){
	spi_cs2_set_low();
	spi_write_data(wake_up, sizeof(wake_up));
	table[0] = 0x77; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);
	spi_cs2_set_high();

	// ACMD41 - used to initialize card (power mode, voltage)
	spi_cs2_set_low();
	spi_write_data(wake_up, sizeof(wake_up));
	table[0] = 0x69; table[1] = 0x40; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);
	spi_cs2_set_high();

	}

	// CMD58 - used to check card type and supported voltage (we got 0x01 so CMD16 is not needed and voltage value 0xFF and 0x80)
	table[0] = 0x7A; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0x01, 0xC0);

	change_SPI_speed();


	table[0] = 0x50; table[1] = 0x00; table[2] = 0x00; table[3] = 0x02; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0x01, 0xFF);


	// CMD13 - used to check ready status, should return 0x00, 0x00
	table[0] = 0x4D; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);

	table[0] = 0x4D; table[1] = 0x00; table[2] = 0x00; table[3] = 0x00; table[4] = 0x00; table[5] = get_CRC7(table[0]);
	spi_write_data(table, sizeof(table));
	read_response(0xFF, 0xFF);

	spi_cs2_set_high();
	uint8_t kalosz[512] = {
	    210, 241, 180, 61, 201, 99, 171, 235, 121, 225, 159, 137, 233, 156, 84, 9,
	    16, 166, 111, 181, 76, 168, 171, 219, 77, 152, 3, 255, 107, 19, 125, 99,
	    49, 67, 80, 11, 39, 167, 145, 47, 123, 175, 220, 231, 95, 155, 212, 71,
	    53, 27, 225, 167, 57, 136, 233, 234, 71, 244, 100, 218, 85, 222, 67, 56,
	    247, 136, 25, 36, 150, 10, 218, 79, 99, 243, 169, 76, 192, 42, 74, 237,
	    157, 142, 203, 221, 114, 71, 132, 63, 141, 200, 68, 29, 79, 132, 62, 117,
	    247, 125, 138, 176, 186, 180, 131, 48, 233, 146, 133, 224, 215, 110, 32, 184,
	    71, 77, 160, 200, 194, 199, 89, 139, 150, 248, 26, 128, 226, 201, 12, 140,
	    25, 172, 244, 144, 197, 178, 174, 108, 156, 71, 62, 117, 254, 193, 200, 196,
	    201, 134, 2, 111, 244, 216, 192, 160, 74, 52, 85, 203, 224, 168, 83, 74,
	    138, 100, 119, 239, 73, 110, 13, 244, 73, 194, 48, 27, 51, 12, 61, 63,
	    214, 70, 93, 123, 9, 165, 116, 45, 92, 193, 85, 222, 185, 69, 51, 68,
	    195, 69, 169, 66, 212, 106, 245, 58, 85, 108, 39, 205, 243, 154, 5, 31,
	    175, 14, 79, 64, 158, 34, 19, 144, 177, 70, 170, 131, 0, 123, 121, 155,
	    55, 169, 16, 129, 172, 237, 237, 83, 114, 219, 195, 147, 118, 151, 111, 253,
	    189, 51, 31, 197, 211, 64, 223, 123, 174, 247, 191, 151, 98, 199, 128, 214,
	    217, 37, 133, 177, 3, 57, 4, 58, 183, 157, 183, 38, 14, 84, 22, 22,
	    93, 120, 159, 135, 81, 129, 56, 99, 27, 114, 139, 24, 183, 209, 82, 160,
	    83, 122, 144, 174, 180, 112, 26, 209, 44, 160, 6, 111, 50, 111, 23, 176,
	    231, 243, 95, 164, 8, 165, 230, 224, 151, 181, 93, 67, 201, 237, 155, 246,
	    253, 106, 72, 193, 205, 127, 220, 237, 163, 230, 75, 148, 178, 25, 154, 144,
	    254, 104, 36, 149, 64, 204, 178, 79, 80, 34, 135, 51, 108, 	    183, 213, 46, 21, 159, 85, 211, 215, 80, 109, 32, 199, 52, 85, 9, 173,
149, 69, 92,
	    166, 95, 84, 5, 79, 38, 148, 141, 138, 52, 31, 187, 83, 145, 99, 2,
	    92, 238, 48, 241, 244, 128, 107, 128, 4, 216, 118, 219, 45, 86, 79, 132,
	    253, 41, 92, 82, 183, 92, 144, 64, 58, 7, 82, 201, 175, 147, 8, 33,
	    130, 78, 114, 49, 244, 29, 126, 163, 198, 74, 75, 252, 150, 210, 249, 201,
	    104, 117, 189, 218, 97, 251, 16, 204, 3, 191, 165, 72, 240, 98, 80, 220,
	    196, 167, 63, 114, 194, 43, 26, 127, 199, 24, 190, 195, 252, 111, 66, 220,
	    69, 30, 207, 47, 32, 145, 83, 59, 192, 5, 174, 220, 47, 220, 133, 134
	};


	SD_write(kalosz);
	SD_read_single();
}

