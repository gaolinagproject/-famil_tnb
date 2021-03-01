/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"


/* USER CODE BEGIN PV */
#define TEST0_TASK_PRIO		3
#define TEST0_STK_SIZE 		128 

#define TEST1_TASK_PRIO		4
#define TEST1_STK_SIZE 		256 
/* USER CODE END PV */
void Task0_Task(void *pvParameters);
void Task1_Task(void *pvParameters);

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */



  xTaskCreate(Task0_Task,      "Task0_Task",        TEST0_STK_SIZE,NULL, 	TEST0_TASK_PRIO, 	NULL);    
  xTaskCreate(Task1_Task,      "Task1_Task",        TEST1_STK_SIZE,NULL, 	TEST1_TASK_PRIO, 	NULL);         
  vTaskStartScheduler(); 

  while (1)
  {

  }

}
void Task0_Task(void *pvParameters)
{
  while(1)
  {
      printf("0000 \n");	
      vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}

void Task1_Task(void *pvParameters)
{
  printf("4.3' TFT-LCD Test By Mculover666\r\n");
  lcd_init();


  //���߲���
  lcd_clear(BLACK);
  lcd_draw_line(0, lcd_params.lcd_height/2, lcd_params.lcd_width, lcd_params.lcd_height/2, BLUE);
  lcd_draw_line(lcd_params.lcd_width/2, 0, lcd_params.lcd_width/2, lcd_params.lcd_height, YELLOW);
  lcd_draw_line(0, 0, lcd_params.lcd_width, lcd_params.lcd_height, GREEN);
  lcd_draw_line(lcd_params.lcd_width, 0, 0, lcd_params.lcd_height, RED);

  //�����β���
  lcd_draw_rect(lcd_params.lcd_width/4, lcd_params.lcd_height/4, lcd_params.lcd_width/4*3, lcd_params.lcd_height/4*3, CYAN);
  lcd_draw_rect(lcd_params.lcd_width/6, lcd_params.lcd_height/6, lcd_params.lcd_width/8*7, lcd_params.lcd_height/8*7,BRED);
  //��Բ����
  lcd_draw_circle(lcd_params.lcd_width/2, lcd_params.lcd_height/2, 50, BRED);
  while(1)
  {
      printf("1111 \n");	
      vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
