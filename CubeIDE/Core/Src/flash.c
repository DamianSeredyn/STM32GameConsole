#include "flash.h"
#include "i2c.h"

void flash_test(void){
	uint8_t output;
	output = 0x00;
	//uint16_t i = 10000;
	//output = output;

	//flash_i2c_write(0xA0, 0x0000, &output, 1);
	//while(i2c_transfer_complete != true)
	//		;
	output = 0x01;
	output = flash_read(0x0000);

	//i = 10000;

	output = output;
	//flash_i2c_read(0xA1, 0x0000, &output, 1);
	//while(i2c_transfer_complete != true)
	//	;
	output = 0x00;
	flash_write(output, 0x0000);

	//	i = 10000;

	output = output;
}

// output to twoje wyjscie
// register_addres to numer rejestry w przedziale od 0 do 65,535 w binarnym z którego chcesz odczytac dane
uint8_t flash_read(uint16_t register_addres){
	uint8_t output;
	uint16_t i = 100000;
	flash_i2c_read(0xA1, register_addres, &output, 1);
	while(i2c_transfer_complete != true)
		;
	while (i > 0)
	{
		i--;
	}
	return output;
}

// input to twoje wejscie
// register_addres to numer rejestry w przedziale od 0 do 65,535 z którego chcesz odczytac dane
void flash_write(uint8_t input, uint16_t register_addres){
	uint16_t i = 100000;
	flash_i2c_write(0xA0, register_addres, &input, 1);
	while(i2c_transfer_complete != true)
		;
	while (i > 0)
	{
		i--;
	}
}





