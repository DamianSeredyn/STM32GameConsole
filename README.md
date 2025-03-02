## Specification
- microcontoler STM32L476
- LCD resistive touch screen TFT 4 Waveshare 13587
- 2x  LED RGB 8 x WS2812 5050
- buzzer with BJT
## Pinout
 ### Microcontoler   
  ##### SPI/touch screen
 - ALL female goldpins RESERVED
 ##### SPI/SC_CARD
- MISO --> PB_14
- MOSI --> PB_15
- SCK  --> PB_13
- CS --> PB_11
##### RGB LEDs
- TIM4-CH1  --> PB_6
##### Buzzer 
- TIM4-CH1  --> PA_0
##### I2C
- I2C1_SCL --> PB_8
- I2C1_SDA --> PB_9
## UART console command list
- !help --> show all of avalible commands.
- !display --> The !display function is used to control the display screen.

## Power
- SD_card - 5V
