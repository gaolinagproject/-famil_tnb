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



/* USER CODE BEGIN PV */
#define TEST0_TASK_PRIO		3
#define TEST0_STK_SIZE 		128 

#define TEST1_TASK_PRIO		2
#define TEST1_STK_SIZE 		500 

#define TEST2_TASK_PRIO		3
#define TEST2_STK_SIZE 		128 
/* USER CODE END PV */
void Task0_Task(void *pvParameters);
void Task1_Task(void *pvParameters);
void Task2_Task(void *pvParameters);
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t hello[] = "USART1 is ready...\n";
uint8_t recv_buf[20] = {0};
uint8_t recv_buf1[20] = {0};
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
  MX_CRC_Init();
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  
  
  
  //使能串口中断接收
  HAL_UART_Receive_IT(&huart1, (uint8_t*)recv_buf, 20);
  HAL_UART_Receive_IT(&huart2, (uint8_t*)recv_buf1, 20);
  
  /* USER CODE BEGIN 2 */
  xTaskCreate(Task0_Task,      "Task0_Task",        TEST0_STK_SIZE,NULL, 	TEST0_TASK_PRIO, 	NULL);    
  xTaskCreate(Task1_Task,      "Task1_Task",        TEST1_STK_SIZE,NULL, 	TEST1_TASK_PRIO, 	NULL);      
  xTaskCreate(Task2_Task,      "Task2_Task",        TEST2_STK_SIZE,NULL, 	TEST2_TASK_PRIO, 	NULL);  
  vTaskStartScheduler(); 

  while (1)
  {

  }

}


/* USER CODE BEGIN 4 */
/* 中断回调函数 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* 判断是哪个串口触发的中断 */
    if(huart ->Instance == USART1)
    {
        //将接收到的数据发送
        HAL_UART_Transmit_IT(huart, (uint8_t*)recv_buf, 20);
        //重新使能串口接收中断
        HAL_UART_Receive_IT(huart, (uint8_t*)recv_buf, 20);
    }
    if(huart ->Instance == USART2)
    {
        //将接收到的数据发送
        HAL_UART_Transmit_IT(huart, (uint8_t*)recv_buf1, 20);
        //重新使能串口接收中断
        HAL_UART_Receive_IT(huart, (uint8_t*)recv_buf1, 20);
    }  

}



void Task2_Task(void *pvParameters)
{
  uint8_t temp = 0,humi = 0;
  
  DS3231_Init();
  Set_DS3231_Time(2021,2,25,10,10,10,4);
  
  if (-1 == sgp30_init()) {
          printf("sgp30 init fail\r\n");
  }
  printf("sgp30 init success\r\n");
  
  DHT11_Init();
  while(1)
  {
//    mp3_test();
    Get_DS3231_Time(); 
    printf("syear %d - smon %d  - sday %d - hour %d - min %d - sec %d -s week %d \n",
           calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
    
    if( -1 == spg30_read()) {
      printf("sgp30 read fail\r\n");
    }
    else {
      printf("sgp30 read success, co2:%4d ppm, tvoc:%4d ppd\r\n", sgp30_data.co2, sgp30_data.tvoc);
    }

    
    
    if(DHT11_Read_Data(&temp,&humi) == 0)
    {
      printf("temp = %d\n humi = %d\n",temp,humi);
    }
    else
    {
      printf("jjjj \n");
    }
      vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}


void Task0_Task(void *pvParameters)
{
  while(1)
  {
    GUI_TOUCH_Exec();
//  printf("0000 \n");	
      vTaskDelay( 10 / portTICK_PERIOD_MS);
  }
}
#include "DIALOG.h"
extern WM_HWIN CreateFramewin(void);
void Task1_Task(void *pvParameters)
{
  ILI93XX_LCD_Init();
  LCD_Clear(BLACK);
  TP_Init();

  GUI_Init();
  GUI_CURSOR_Show();
  CreateFramewin();
  WM_MULTIBUF_Enable(1);

  while(1)
  {
    GUI_Delay(100);
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
