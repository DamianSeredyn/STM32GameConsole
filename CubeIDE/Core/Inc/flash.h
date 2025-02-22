/*
 * flash.h
 *
 *  Created on: Jan 11, 2025
 *      Author: ciast
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"


#define FLASH_ADRESS 0xA0
#define CTRL_REG1_A 0X0000


void flash_test(void);


// input to twoje wejscie
// register_addres to numer rejestry w przedziale od 0 do 65,535 z którego chcesz odczytac dane
uint8_t flash_read(uint16_t register_addres);

// output to twoje wyjscie
// register_addres to numer rejestry w przedziale od 0 do 65,535 w binarnym z którego chcesz odczytac dane
void flash_write(uint8_t input, uint16_t register_addres);



#endif /* INC_FLASH_H_ */
