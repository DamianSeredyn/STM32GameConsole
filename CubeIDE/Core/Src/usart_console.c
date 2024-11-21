/*
 * usart_console.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Damian
 */

#include "usart_console.h"


void HandleCommand(uint8_t* data)
{
	uint8_t response[BUFFER_SIZE-1]= ""; // -1 because of structure of buffer!
	switch (hash_command(data))
	{
	        case 1:
	            strcpy((char*)response, "\r\nList of all available commands with short descriptions:\r\n"
	                             "example command -> example description\r\n");
	            break;
	        case 2:
	            strcpy((char*)response, "Hi there! Need help? Type !help or simply enter a command.\r\n");
	            break;
	        default:
	            strcpy((char*)response, "\r\nError! Unknown command.\r\n");
	            break;
	}

	USART2_SendData(response,BUFFER_SIZE-1);

}


unsigned int hash_command(const uint8_t *cmd) {
    if (strcmp((const char *)cmd, "!help") == 0) return 1;
    if (strcmp((const char *)cmd, "\r") == 0 || strcmp((const char *)cmd, "") == 0) return 2;
    return 0;
}
