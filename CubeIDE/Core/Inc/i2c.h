/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_utils.h"
#include <stdbool.h>

/* USER CODE BEGIN Includes */
#define I2C1_SCL_PIN LL_GPIO_PIN_8
#define I2C1_SDA_PIN LL_GPIO_PIN_9
#define I2C1_PORT GPIOB
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
typedef struct
{
	uint8_t* data_ptr;
	uint32_t count;
}buffer_t;

extern volatile bool i2c_transfer_complete;

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */
/*
 * Function used to read data from register
 * @param slave_addr Adress of a slave device
 * @param reg_addr Adress of registry to read from a slave device
 * @param data_ptr Variable to store read data
 * @param size Size of data_ptr
 * example:
 * static uint8_t data_r;
 * I2C1_reg_read_it(0xEC, 0xD0, &data_r, sizeof(data_r));
 */
int32_t I2C1_reg_read_it(uint16_t slave_addr, uint16_t reg_addr, uint8_t *data_ptr, uint16_t size);

/*
 * Function used to write data to register
 * @param slave_addr Adress of a slave device
 * @param reg_addr Adress of registry to read from slave a device
 * @param data_ptr Variable that stores data to write to a slave device
 * @param size Size of data_ptr
 */
int32_t I2C1_reg_write_it(uint16_t slave_addr, uint16_t reg_addr, uint8_t *data_ptr, uint16_t size);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

