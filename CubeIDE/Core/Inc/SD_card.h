
#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#include "spi.h"
#include "main.h"
#include "stdbool.h"

// SD adapter should be powered by 5V supply

/**
 * how to get hex value of a CMD command?
 * if you have for example CMD13 convert number 13 from dec to hex
 * then add to it 0x40
 * 0x0D + 0x40 = 0x4D
 */

/**
 * inits SD card
 * returns 0 if everything is fine
 */
uint8_t SD_card_init();


/**
 * used to test if read and write works
 * will create an infinite loop if something doesn't work or SD is not connected
 * returns 0 if everything is fine
 *
 */
uint8_t read_write_test();

/**
 * used to write data to a single sector in SD
 * you MUST send 512 bytes, just use uint8_t table[512]
 * will create an infinite loop if something doesn't work or SD is not connected
 * showcased in uint8_t read_write_test();
 */
void SD_write(uint8_t *data_sent,uint8_t sector1,uint8_t sector2,uint8_t sector3,uint8_t sector4);

/**
 * used to read a single sector in SD
 * will create an infinite loop if something doesn't work or SD is not connected
 * example : uint8_t* = SD_read_single(0x00, 0x00, 0x00, 0x01);
 */
uint8_t* SD_read_single(uint8_t sector1,uint8_t sector2,uint8_t sector3,uint8_t sector4);

#endif /* INC_SD_CARD_H_ */
