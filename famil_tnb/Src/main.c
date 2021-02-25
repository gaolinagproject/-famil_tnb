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


#define TEST0_TASK_PRIO		3
#define TEST0_STK_SIZE 		128 

#define TEST1_TASK_PRIO		2
#define TEST1_STK_SIZE 		128 


void Task0_Task(void *pvParameters);
void Task1_Task(void *pvParameters);
void SystemClock_Config(void);

uint8_t hello[] = "USART1 is ready...\n";
uint8_t recv_buf[20] = {0};
uint8_t recv_buf1[20] = {0};
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
    /* 判断是哪个串口触发的中断 */
    if(huart ->Instance == USART3)
    {
        //将接收到的数据发送
        HAL_UART_Transmit_IT(huart, (uint8_t*)recv_buf, 20);
        //重新使能串口接收中断
        HAL_UART_Receive_IT(huart, (uint8_t*)recv_buf, 20);
    }
    if(huart ->Instance == UART4)
    {
        //将接收到的数据发送
        HAL_UART_Transmit_IT(huart, (uint8_t*)recv_buf, 20);
        //重新使能串口接收中断
        HAL_UART_Receive_IT(huart, (uint8_t*)recv_buf, 20);
    }   
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_UART4_Init();
  
  DS3231_Init();
  Set_DS3231_Time(2021,2,25,10,10,10,4);
  
  //使能串口中断接收
  HAL_UART_Receive_IT(&huart1, (uint8_t*)recv_buf, 20);
  HAL_UART_Receive_IT(&huart2, (uint8_t*)recv_buf1, 20);
  HAL_UART_Receive_IT(&huart3, (uint8_t*)recv_buf, 20);
  HAL_UART_Receive_IT(&huart4, (uint8_t*)recv_buf, 20);
  
  xTaskCreate(Task0_Task,      "Task0_Task",        TEST0_STK_SIZE,NULL, 	TEST0_TASK_PRIO, 	NULL);    
  xTaskCreate(Task1_Task,      "Task1_Task",        TEST1_STK_SIZE,NULL, 	TEST1_TASK_PRIO, 	NULL);         
  vTaskStartScheduler();      
  
  /* USER CODE END 3 */
}



void Task0_Task(void *pvParameters)
{
  while(1)
  {
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET); 	 	
      vTaskDelay( 500 / portTICK_PERIOD_MS);	
      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);   	
      vTaskDelay( 500 / portTICK_PERIOD_MS);
      printf("smoke_value = %d\n", 1);
  }
}

void Task1_Task(void *pvParameters)
{
  uint8_t temp = 0,humi = 0;
  while(1)
  {

//      if(0 == HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin))
//      {
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET); 	
//      }
//      if(0 == HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin))
//      {
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);   
//      }
    
    Get_DS3231_Time(); 
    printf("syear %d - smon %d  - sday %d - hour %d - min %d - sec %d -s week %d \n",
           calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
    
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
