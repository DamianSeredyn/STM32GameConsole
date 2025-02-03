/*
 * usart_console.c
 *
 *  Created on: Nov 21, 2024
 *      Author: Damian
 */

#include "usart_console.h"


uint16_t ACTION = 0;

void HandleCommand(uint8_t* data)
{
	uint8_t response[OUTPUT_BUFFER_SIZE-1]= {0}; // -1 because of structure of buffer!
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
	        		else if(strcmp((const char *)decomposedData[1], "display")==0)
	        		{
	        			strcpy((char*)response, "\r\nHelp for !display function:\r\n");
	        			strcpy((char*)response + strlen((char*)response), "The !display function is used to control the display screen. Below are the available commands:\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display TestScreen - Switch to the test screen on the display, showing diagnostic or test patterns.\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display Menu - Switch to the main menu screen, where the user can interact with different options.\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display ProducerTestScreen - Switch to a special test screen for production purposes, typically used for testing hardware.\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display Disable - Disable the display (turn it off), stopping it from showing any content.\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display Enable - Enable the display (turn it on), allowing the display to show content again.\r\n");
	        			strcpy((char*)response + strlen((char*)response), "!display Clear - Clear the current content on the display, resetting it to a blank state.\r\n");

	        		}
	        		else
	        		{
	        			if (strlen((char*)decomposedData[1]) < BUFFER_SIZE)
	        			{
	        				snprintf((char*)response,OUTPUT_BUFFER_SIZE, "\r\nUnable to find help for '%s' -> Are you sure that you typed a correct command?\r\n", (char*)decomposedData[1]);
	        			}

	        		}

	            break;
	        case 2:
	        		strcpy((char*)response, "Hi there! Need help? Type !help or simply enter a command.\r\n");
	            break;
	        case 3:
	        		if(strcmp((const char *)decomposedData[1], "TestScreen")==0)
	        		{
	        			strcpy((char*)response, "\r\n Displaying Test Screen on Display...\r\n");
	        			ACTION = 1;
	        		}
	        		else if(strcmp((const char *)decomposedData[1], "Menu")==0)
	        		{
	        			strcpy((char*)response, "\r\n Displaying Menu on Display...\r\n");
	        			ACTION =2;
	        		}
	        		else if(strcmp((const char *)decomposedData[1], "ProducerTestScreen")==0)
	        		{
	        			strcpy((char*)response, "\r\n Displaying  Producer Test Screen...\r\n");
	        			ACTION =3;
	        		}
	        		else if(strcmp((const char *)decomposedData[1], "Disable")==0)
	        		{
	        			strcpy((char*)response, "\r\n Disabling display...\r\n");
	        			ACTION =4;
	        		}
	        		else if(strcmp((const char *)decomposedData[1], "Enable")==0)
	        		{
	        			strcpy((char*)response, "\r\n Enabling display...\r\n");
	        			ACTION =5;
	        		}
	        		else if(strcmp((const char *)decomposedData[1], "Clear")==0)
	        		{
	        			strcpy((char*)response, "\r\n Clearing display...\r\n");
	        			ACTION =6;
	        		}
	        		else
	        		{
	        			strcpy((char*)response, "\r\n ERROR! Invalid arguments, view !help display.\r\n");
	        		}
	            break;

	        default:
	            snprintf((char*)response,OUTPUT_BUFFER_SIZE, "\r\nError! '%s' -> Unknown command.\r\n"  , (char*)decomposedData[0]);
	            break;
	}


	USART2_SendData(response,OUTPUT_BUFFER_SIZE-1);

}


unsigned int hash_command( uint8_t *cmd) {
    if (strcmp((const char *)cmd, "!help") == 0) return 1;
    if (strcmp((const char *)cmd, "\r") == 0 || strcmp((const char *)cmd, "") == 0) return 2;
    if (strcmp((const char *)cmd, "!display") == 0) return 3;
    if (strcmp((const char *)cmd, "!cat") == 0) return 4;
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
