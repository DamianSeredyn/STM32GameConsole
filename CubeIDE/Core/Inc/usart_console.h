/*
 * usart_source.h
 *
 *  Created on: Nov 21, 2024
 *      Author: Damian
 */

#ifndef INC_USART_CONSOLE_H_
#define INC_USART_CONSOLE_H_

#define MAX_CONSOLE_WORDS 10
#define BUFFER_SIZE_COMMAND 128

#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>


void HandleCommand(uint8_t* data);
unsigned int hash_command( uint8_t* cmd);
void split_data_into_words(uint8_t* data, uint8_t words[MAX_CONSOLE_WORDS][BUFFER_SIZE_COMMAND]);
#endif /* INC_USART_CONSOLE_H_ */



