/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "SD_card.h"
#include "flash.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
   InitRingbuffer();
   MX_I2C1_Init();
   LL_mDelay(100);
   SPI_ILI9486_init();

   LCD_SCAN_DIR Lcd_ScanDir = D2U_L2R;
   ILI9486_Init(Lcd_ScanDir);

   BUTTON_EXTI_Init();

   MX_SPI2_Init();
   spi_init_interupts();

   MX_TIM2_Init();
   MX_TIM4_Init();

   spi_init_DMA();
   DMA_TIM4_Init();

   LL_mDelay(100);

   dynamic_Leds_Setup();
   dynamic_Leds_Test();
   MusicTest();

   SD_card_init();
   //read_write_test();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ManageActions(); // Manage USART actions
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	 LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
	  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_4)
	  {
	  }
	  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	  while (LL_PWR_IsActiveFlag_VOS() != 0)
	  {
	  }
	  LL_RCC_MSI_Enable();

	   /* Wait till MSI is ready */
	  while(LL_RCC_MSI_IsReady() != 1)
	  {

	  }
	  LL_RCC_MSI_EnableRangeSelection();
	  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);
	  LL_RCC_MSI_SetCalibTrimming(0);
	  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
	  LL_RCC_PLL_EnableDomain_SYS();
	  LL_RCC_PLL_Enable();

	   /* Wait till PLL is ready */
	  while(LL_RCC_PLL_IsReady() != 1)
	  {

	  }
	  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	   /* Wait till System clock is ready */
	  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	  {

	  }
	  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	  LL_Init1msTick(80000000);

	  LL_SetSystemCoreClock(80000000);
}

/* USER CODE BEGIN 4 */
void ManageActions()
{
	if(ACTION == 0)
		return;
	switch(ACTION)
		  {
		  	  case 1:
		  		GUI_ShowTest();
			  break;
		  	  case 2:
		  		DisplayMenu();
			  break;
		  	  case 3:
		  		GUI_ShowProducent();
		  	  break;
		  	  case 4:
		  		LL_GPIO_ResetOutputPin(GPIOC, ILI9486_BL_PIN);
		  	  break;
		  	  case 5:
		  		LL_GPIO_SetOutputPin(GPIOC, ILI9486_BL_PIN);
		  	  break;
		  	  case 6:
		  		GUI_Clear(0xFFFF);
		  	  break;
		  }
	ACTION = 0;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
