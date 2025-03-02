/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "stdlib.h"
#include "math.h"

#include "GUI.h"
#include "DIALOG.h"
  
#include "spi.h"  
#include "i2c.h"  
#include "crc.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"
#include "ILI93xx.h"
  
  
#include "delay.h"
#include "touch.h"   
#include "mylib.h" 
#include "w25qxx.h" 
#include "dht11.h" 
#include "ds3231.h"
#include "gui_common.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern SemaphoreHandle_t ButtonArmSemaphore;
extern SemaphoreHandle_t ClockArmSemaphore;
extern QueueHandle_t ParaSensorQue;  
extern QueueHandle_t ParaClockQue;  

void Task_Lcd(void *pvParameters);
void Task_Core(void *pvParameters);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
void draw_bitmap(void);
void zoom_bitmap(int Xmag,int Ymag);
/* USER CODE END EC */
void draw_bitmap(void);
void zoom_bitmap(int Xmag,int Ymag);
/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
