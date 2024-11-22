/*
 * usart_console.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Damian
 */

#include "usart_console.h"


void HandleCommand(uint8_t* data)
{
	uint8_t response[BUFFER_SIZE-1]= {0}; // -1 because of structure of buffer!
	uint8_t decomposedData[MAX_CONSOLE_WORDS][BUFFER_SIZE_COMMAND]= {0};
	split_data_into_words(data,decomposedData);
	switch (hash_command(decomposedData[0]))
	{
	        case 1:
	        		if(strcmp((const char *)decomposedData[1], "")==0)
	        		{
	        			strcpy((char*)response, "\r\nList of all available commands with short descriptions:\r\n"
                        "example command -> example description\r\n");
	        		}
	        		else
	        		{
	        			if (strlen((char*)decomposedData[1]) < BUFFER_SIZE)
	        			{
	        				snprintf((char*)response,BUFFER_SIZE, "\r\nUnable to find help for '%s' -> Are you sure that you typed a correct command?\r\n", (char*)decomposedData[1]);
	        			}

	        		}

	            break;
	        case 2:
	        		strcpy((char*)response, "Hi there! Need help? Type !help or simply enter a command.\r\n");
	            break;
	        default:
	            snprintf((char*)response,BUFFER_SIZE, "\r\nError! '%s' -> Unknown command.\r\n"  , (char*)decomposedData[0]);
	            break;
	}

	USART2_SendData(response,BUFFER_SIZE-1);

}


unsigned int hash_command( uint8_t *cmd) {
    if (strcmp((const char *)cmd, "!help") == 0) return 1;
    if (strcmp((const char *)cmd, "\r") == 0 || strcmp((const char *)cmd, "") == 0) return 2;
    return 0;
}

void split_data_into_words(uint8_t* data, uint8_t words[MAX_CONSOLE_WORDS][BUFFER_SIZE_COMMAND]){
	uint8_t wordCount = 0;
	uint8_t word[BUFFER_SIZE_COMMAND] = {0};

	uint8_t charCounter = 0;
    for (size_t i = 0; i< BUFFER_SIZE ; i++) {
       if(data[i] == ' ')
       {
    	   strcpy((char*)words[wordCount], (char*)word);
    	   wordCount++;
    	   memset(word, 0, sizeof(word));
    	   charCounter = 0;
       }
       else if(charCounter<BUFFER_SIZE_COMMAND)
       {
    	   word[charCounter] = data[i];
    	   charCounter++;
       }


       if(wordCount == MAX_CONSOLE_WORDS-1)
    	   	return;
    }

    strcpy((char*)words[wordCount], (char*)word);

}
