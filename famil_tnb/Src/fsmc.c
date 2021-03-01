/**
  ******************************************************************************
  * File Name          : FSMC.c
  * Description        : This file provides code for the configuration
  *                      of the FSMC peripheral.
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

/* Includes ------------------------------------------------------------------*/
#include "fsmc.h"

/* USER CODE BEGIN 0 */
lcd_params_t lcd_params;
/* USER CODE END 0 */

SRAM_HandleTypeDef hsram1;

/* FSMC initialization function */
void MX_FSMC_Init(void)
{
  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
//  Timing.AddressSetupTime = 2;
//  Timing.AddressHoldTime = 15;
//  Timing.DataSetupTime = 4;
//  Timing.BusTurnAroundDuration = 15;
//  Timing.CLKDivision = 16;
//  Timing.DataLatency = 17;
//  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 2;
  Timing.DataSetupTime = 6;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

}

static uint32_t FSMC_Initialized = 0;

static void HAL_FSMC_MspInit(void){
  /* USER CODE BEGIN FSMC_MspInit 0 */

  /* USER CODE END FSMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FSMC_Initialized) {
    return;
  }
  FSMC_Initialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD13   ------> FSMC_A18
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */
  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* GPIO_InitStruct */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN FSMC_MspInit 1 */

  /* USER CODE END FSMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspInit 0 */

  /* USER CODE END SRAM_MspInit 0 */
  HAL_FSMC_MspInit();
  /* USER CODE BEGIN SRAM_MspInit 1 */

  /* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspDeInit(void){
  /* USER CODE BEGIN FSMC_MspDeInit 0 */

  /* USER CODE END FSMC_MspDeInit 0 */
  if (FSMC_DeInitialized) {
    return;
  }
  FSMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FSMC_CLK_DISABLE();
  
  /** FSMC GPIO Configuration  
  PE7   ------> FSMC_D4
  PE8   ------> FSMC_D5
  PE9   ------> FSMC_D6
  PE10   ------> FSMC_D7
  PE11   ------> FSMC_D8
  PE12   ------> FSMC_D9
  PE13   ------> FSMC_D10
  PE14   ------> FSMC_D11
  PE15   ------> FSMC_D12
  PD8   ------> FSMC_D13
  PD9   ------> FSMC_D14
  PD10   ------> FSMC_D15
  PD13   ------> FSMC_A18
  PD14   ------> FSMC_D0
  PD15   ------> FSMC_D1
  PD0   ------> FSMC_D2
  PD1   ------> FSMC_D3
  PD4   ------> FSMC_NOE
  PD5   ------> FSMC_NWE
  PD7   ------> FSMC_NE1
  */

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_7);

  /* USER CODE BEGIN FSMC_MspDeInit 1 */

  /* USER CODE END FSMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef* sramHandle){
  /* USER CODE BEGIN SRAM_MspDeInit 0 */

  /* USER CODE END SRAM_MspDeInit 0 */
  HAL_FSMC_MspDeInit();
  /* USER CODE BEGIN SRAM_MspDeInit 1 */

  /* USER CODE END SRAM_MspDeInit 1 */
}


/**
 * @brief    �������
 * @param    state ����״̬
 * @retval   none
*/
void lcd_backlight_ctrl(lcd_backlight_state_t state)
{
    if (state == LCD_BACKLIGHT_ON) {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);

    } else {
      HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);

    }
    
}
static void lcd_write_cmd(__IO uint16_t cmd)
{
    cmd = cmd;
    LCD->LCD_REG = cmd;
}

static void lcd_write_data(__IO uint16_t data)
{
    data = data;
    LCD->LCD_RAM=data;;
}
static uint16_t lcd_read_data(void)
{
    __IO uint16_t data;

    data = LCD->LCD_RAM;

    return data;
}

/**
 * @brief    дLCD�еļĴ���
 * @param    reg  �Ĵ������
 * @param    data Ҫд��Ĵ�����ֵ
 * @retval   none
*/
static void lcd_write_reg(__IO uint16_t reg, __IO uint16_t data)
{
    lcd_write_cmd(reg);
    lcd_write_data(data);
}

/**
 * @brief    ��ȡLCD����IC��ID������LCD�����ṹ��
 * @param    none
 * @retval   �ɹ� 0
 * @retval   ʧ�� -1
*/
static int lcd_read_id(void)
{
    /* ����ִ��ILI9341������ID�Ķ�ȡ���� */
    lcd_write_cmd(0xD3);				   
	lcd_params.lcd_id = lcd_read_data();
	lcd_params.lcd_id = lcd_read_data();
	lcd_params.lcd_id = lcd_read_data();				   
	lcd_params.lcd_id <<= 8;
	lcd_params.lcd_id |= lcd_read_data();
    /* ��������������򷵻سɹ� */
    if (lcd_params.lcd_id == 0x9341) {
        return 0;
    }
    
    /* ����ִ��NT35310������ID�Ķ�ȡ���� */
    lcd_write_cmd(0xD4);				   
    lcd_params.lcd_id = lcd_read_data();
    lcd_params.lcd_id = lcd_read_data();
    lcd_params.lcd_id = lcd_read_data();
    lcd_params.lcd_id <<= 8;	 
    lcd_params.lcd_id |= lcd_read_data();
    /* ��������������򷵻سɹ� */
    if (lcd_params.lcd_id == 0x5310) {
        return 0;
    }
    
    /* ����ִ��NT35510������ID�Ķ�ȡ���� */
    lcd_write_cmd(0xDA00);
    lcd_params.lcd_id = lcd_read_data();
    lcd_write_cmd(0xDB00);
    lcd_params.lcd_id = lcd_read_data();
    lcd_params.lcd_id <<= 8;	 
    lcd_write_cmd(0xDC00);
    lcd_params.lcd_id |= lcd_read_data();
    /* ��������������򷵻سɹ� */
    if (lcd_params.lcd_id == 0x8000) {
        lcd_params.lcd_id = 0x5510;
        return 0;
    }
    
    /* �����Ҫ��Ӹ�������֧�֣����ڴ˴���Ӷ�ȡID���� */
   
    /* ����IC��֧�� */
    return -1;
}

/**
 * @brief    LCD����ʾ
 * @param    none
 * @retval   none
*/
void lcd_display_on(void)
{
    /* �����Ҫ��Ӹ�������֧�֣����ڴ˴���ӿ���ʾ���� */    
    
	if (lcd_params.lcd_id == 0x9341 || lcd_params.lcd_id == 0x5310) {
        lcd_write_cmd(0x29);
    } else if (lcd_params.lcd_id == 0x5510) {
        lcd_write_cmd(0x2900);
    }
}	 

/**
 * @brief    LCD����ʾ
 * @param    none
 * @retval   none
*/
void lcd_display_off(void)
{
    /* �����Ҫ��Ӹ�������֧�֣����ڴ˴���ӿ���ʾ���� */    
    
 	if (lcd_params.lcd_id == 0x9341 || lcd_params.lcd_id == 0x5310) {
        lcd_write_cmd(0x28);
    } else if (lcd_params.lcd_id == 0x5510) {
        lcd_write_cmd(0x2800);
    }
}

/**
 * @brief    LCD�����Զ�ɨ�跽��
 * @param    dir ɨ�跽��
 * @retval   none
*/
static void lcd_set_scan_direction(lcd_scan_dir_t dir)
{
    uint16_t regval = 0;
	uint16_t dirreg = 0;
    uint16_t temp = 0;
    
    /* ����ʱ��ֻ��1963���������ı�ɨ�跽�����඼Ҫ�� */
    if (lcd_params.lcd_direction == 1) {
        switch (dir) {
            case L2R_U2D:   dir = D2U_L2R;    break;
			case L2R_D2U:   dir = D2U_R2L;    break;
			case R2L_U2D:   dir = U2D_L2R;    break;
			case R2L_D2U:   dir = U2D_R2L;    break;	 
			case U2D_L2R:   dir = L2R_D2U;    break;
			case U2D_R2L:   dir = L2R_U2D;    break;
			case D2U_L2R:   dir = R2L_D2U;    break;
			case D2U_R2L:   dir = R2L_U2D;    break;	 
        }
    }
    
	if (lcd_params.lcd_id == 0x9341 || lcd_params.lcd_id == 0x5310 || lcd_params.lcd_id == 0x5510) {
		switch (dir) {
			case L2R_U2D:
				regval |= (0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:
				regval |= (1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:
				regval |= (0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:
				regval |= (1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:
				regval |= (0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L:
				regval |= (0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R:
				regval |= (1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L:
				regval |= (1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
        
		if (lcd_params.lcd_id == 0x5510) {
            dirreg = 0x3600;
        } else {
            dirreg = 0x36;
        }
        
 		if ((lcd_params.lcd_id != 0x5310) && (lcd_params.lcd_id != 0X5510)) {
            regval |= 0X08;
        }
        
        lcd_write_cmd(dirreg);
        lcd_write_data(regval);
 		
        if(regval & 0x20) {
            if(lcd_params.lcd_width < lcd_params.lcd_height) {
                temp = lcd_params.lcd_width;
                lcd_params.lcd_width = lcd_params.lcd_height;
                lcd_params.lcd_height = temp;
            }
        } else {
            if (lcd_params.lcd_width > lcd_params.lcd_height) {
                temp = lcd_params.lcd_width;
                lcd_params.lcd_width = lcd_params.lcd_height;
                lcd_params.lcd_height = temp;
            }
        }
	
		if (lcd_params.lcd_id == 0x5510) {
			lcd_write_cmd(lcd_params.set_x_cmd);
            lcd_write_data(0); 
			lcd_write_cmd(lcd_params.set_x_cmd + 1);
            lcd_write_data(0); 
			lcd_write_cmd(lcd_params.set_x_cmd + 2);
            lcd_write_data((lcd_params.lcd_width - 1) >> 8); 
			lcd_write_cmd(lcd_params.set_x_cmd + 3);
            lcd_write_data((lcd_params.lcd_width - 1) & 0xFF); 
			lcd_write_cmd(lcd_params.set_y_cmd);
            lcd_write_data(0); 
			lcd_write_cmd(lcd_params.set_y_cmd + 1);
            lcd_write_data(0); 
			lcd_write_cmd(lcd_params.set_y_cmd + 2);
            lcd_write_data((lcd_params.lcd_height - 1) >> 8); 
			lcd_write_cmd(lcd_params.set_y_cmd + 3);
            lcd_write_data((lcd_params.lcd_height - 1) & 0xFF);
		} else {
			lcd_write_cmd(lcd_params.set_x_cmd); 
			lcd_write_data(0);
            lcd_write_data(0);
			lcd_write_data((lcd_params.lcd_width - 1) >> 8);
            lcd_write_data((lcd_params.lcd_width - 1) & 0xFF);
			lcd_write_cmd(lcd_params.set_y_cmd); 
			lcd_write_data(0);
            lcd_write_data(0);
			lcd_write_data((lcd_params.lcd_height - 1) >> 8);
            lcd_write_data((lcd_params.lcd_height - 1) & 0xFF);  
		}   
  	}
}

/**
 * @brief    LCD������ʾ����
 * @param    dir ��ʾ����
 * @retval   none
*/
static void lcd_set_display_drection(lcd_display_dir_t dir)
{
    if (dir == VERTICAL_DISP) {
        /* �������� */
        lcd_params.lcd_direction = 0;
        lcd_params.lcd_width     = LCD_WIDTH;
        lcd_params.lcd_height    = LCD_HEIGHT;
        
        if(lcd_params.lcd_id==0X9341||lcd_params.lcd_id==0X6804||lcd_params.lcd_id==0X5310)
		{
			lcd_params.wram_cmd=0X2C;
	 		lcd_params.set_x_cmd=0X2A;
			lcd_params.set_y_cmd=0X2B;  	 
			if(lcd_params.lcd_id==0X6804||lcd_params.lcd_id==0X5310)
			{
				lcd_params.lcd_width=LCD_WIDTH;
				lcd_params.lcd_height=LCD_HEIGHT;
			}
		}else if(lcd_params.lcd_id==0x5510)
		{
			lcd_params.wram_cmd=0X2C00;
	 		lcd_params.set_x_cmd=0X2A00;
			lcd_params.set_y_cmd=0X2B00; 
			lcd_params.lcd_width=240;
			lcd_params.lcd_height=320;
        }
    } else {
        /* ���ú��� */
        lcd_params.lcd_direction = 1;
        lcd_params.lcd_width     = LCD_HEIGHT;
        lcd_params.lcd_height    = LCD_WIDTH;
        
         if(lcd_params.lcd_id==0X9341||lcd_params.lcd_id==0X6804||lcd_params.lcd_id==0X5310)
		{
			lcd_params.wram_cmd=0X2C;
	 		lcd_params.set_x_cmd=0X2A;
			lcd_params.set_y_cmd=0X2B;  	 
			if(lcd_params.lcd_id==0X6804||lcd_params.lcd_id==0X5310)
			{
				lcd_params.lcd_width=LCD_WIDTH;
				lcd_params.lcd_height=LCD_HEIGHT;
			}
		}else if(lcd_params.lcd_id==0x5510)
		{
			lcd_params.wram_cmd=0X2C00;
	 		lcd_params.set_x_cmd=0X2A00;
			lcd_params.set_y_cmd=0X2B00; 
			lcd_params.lcd_width=320;
			lcd_params.lcd_height=240;
        }
    }
     
    /* ����ɨ�跽�� */
    lcd_set_scan_direction(L2R_U2D);
}

/**
 * @brief    LCD���ù��λ��
 * @param    x_pos x��������
 * @param    y_pos yu��������
 * @retval   none
*/
static void lcd_set_cursor(uint16_t x_pos, uint16_t y_pos)
{
    /* �����Ҫ��Ӹ�������֧�֣����ڴ˴�������ù��λ�ô��� */ 
    
 	if (lcd_params.lcd_id == 0x9341 || lcd_params.lcd_id == 0x5310) {	    
		lcd_write_cmd(lcd_params.set_x_cmd); 
		lcd_write_data(x_pos >> 8);
        lcd_write_data(x_pos & 0xFF); 			 
		lcd_write_cmd(lcd_params.set_y_cmd); 
		lcd_write_data(y_pos >> 8);
        lcd_write_data(y_pos & 0xFF); 		
	} else if (lcd_params.lcd_id == 0x5510) {
		lcd_write_cmd(lcd_params.set_x_cmd);
        lcd_write_data(x_pos >> 8); 		
		lcd_write_cmd(lcd_params.set_x_cmd + 1);
        lcd_write_data(x_pos & 0xFF);			 
		lcd_write_cmd(lcd_params.set_y_cmd);
        lcd_write_data(y_pos >> 8);  		
		lcd_write_cmd(lcd_params.set_y_cmd + 1);
        lcd_write_data(y_pos & 0xFF);			
	}
}

/**
 * @brief    LCD��ʼдGRAM
 * @param    none
 * @retval   none
*/
static void lcd_write_ram_start(void)
{
    lcd_write_cmd(lcd_params.wram_cmd);
}

/**
 * @brief    LCDдGRAM
 * @param    rgb_color ��ɫֵ
 * @retval   none
*/
static void lcd_write_ram(uint16_t rgb_color)
{
    lcd_write_data(rgb_color);
}

/**
 * @brief    LCD��������
 * @param    color ������ɫ
 * @retval   none
*/
void lcd_clear(uint16_t color)
{
	uint32_t index = 0;      
	uint32_t totalpoint = lcd_params.lcd_width;
    
    /* ����õ��ܵ��� */
	totalpoint *= lcd_params.lcd_height;
    
    /* ���ù��λ�� */
	lcd_set_cursor(0x00,0x0000);
    
    /* ��ʼд��GRAM */
	lcd_write_ram_start();
    
    /* д�����ݵ�GRAM */
	for (index = 0; index < totalpoint; index++) {
		lcd_write_ram(color);
	}
}

/**
 * @brief    LCD��ʼ��
 * @param    none
 * @retval   none
*/
void lcd_init(void)
{
    /* ��ʼ��FMC�ӿ� */
    //MX_FMC_Init();
	
 	HAL_Delay(50); 
	
    /* ��ȡLCD������IC */
    if (lcd_read_id() == -1) {
        LCD_LOG("Read ID Fail, Not Support LCD IC:%#x!\r\n", lcd_params.lcd_id);
        return;
    } else {
        LCD_LOG("LCD IC ID is:%#x\r\n", lcd_params.lcd_id);
    }
    
    /*
      ���ݲ�ͬ������ID����LCD��ʼ��
      Ŀǰ֧�֣�
        - 0x9341
        - 0x5310
        - 0x5510
    */
    if (lcd_params.lcd_id == 0x9341) {
        lcd_write_cmd(0xCF);  
        lcd_write_data(0x00); 
        lcd_write_data(0xC1); 
        lcd_write_data(0X30); 
        lcd_write_cmd(0xED);  
        lcd_write_data(0x64); 
        lcd_write_data(0x03); 
        lcd_write_data(0X12); 
        lcd_write_data(0X81); 
        lcd_write_cmd(0xE8);  
        lcd_write_data(0x85); 
        lcd_write_data(0x10); 
        lcd_write_data(0x7A); 
        lcd_write_cmd(0xCB);  
        lcd_write_data(0x39); 
        lcd_write_data(0x2C); 
        lcd_write_data(0x00); 
        lcd_write_data(0x34); 
        lcd_write_data(0x02); 
        lcd_write_cmd(0xF7);  
        lcd_write_data(0x20); 
        lcd_write_cmd(0xEA);  
        lcd_write_data(0x00); 
        lcd_write_data(0x00); 
        lcd_write_cmd(0xC0);
        lcd_write_data(0x1B);
        lcd_write_cmd(0xC1);
        lcd_write_data(0x01);
        lcd_write_cmd(0xC5);
        lcd_write_data(0x30);
        lcd_write_data(0x30);
        lcd_write_cmd(0xC7);
        lcd_write_data(0XB7); 
        lcd_write_cmd(0x36);
        lcd_write_data(0x48); 
        lcd_write_cmd(0x3A);   
        lcd_write_data(0x55); 
        lcd_write_cmd(0xB1);   
        lcd_write_data(0x00);   
        lcd_write_data(0x1A); 
        lcd_write_cmd(0xB6);
        lcd_write_data(0x0A); 
        lcd_write_data(0xA2); 
        lcd_write_cmd(0xF2);
        lcd_write_data(0x00); 
        lcd_write_cmd(0x26);
        lcd_write_data(0x01); 
        lcd_write_cmd(0xE0);
        lcd_write_data(0x0F); 
        lcd_write_data(0x2A); 
        lcd_write_data(0x28); 
        lcd_write_data(0x08); 
        lcd_write_data(0x0E); 
        lcd_write_data(0x08); 
        lcd_write_data(0x54); 
        lcd_write_data(0XA9); 
        lcd_write_data(0x43); 
        lcd_write_data(0x0A); 
        lcd_write_data(0x0F); 
        lcd_write_data(0x00); 
        lcd_write_data(0x00); 
        lcd_write_data(0x00); 
        lcd_write_data(0x00); 		 
        lcd_write_cmd(0XE1);
        lcd_write_data(0x00); 
        lcd_write_data(0x15); 
        lcd_write_data(0x17); 
        lcd_write_data(0x07); 
        lcd_write_data(0x11); 
        lcd_write_data(0x06); 
        lcd_write_data(0x2B); 
        lcd_write_data(0x56); 
        lcd_write_data(0x3C); 
        lcd_write_data(0x05); 
        lcd_write_data(0x10); 
        lcd_write_data(0x0F); 
        lcd_write_data(0x3F); 
        lcd_write_data(0x3F); 
        lcd_write_data(0x0F); 
        lcd_write_cmd(0x2B); 
        lcd_write_data(0x00);
        lcd_write_data(0x00);
        lcd_write_data(0x01);
        lcd_write_data(0x3f);
        lcd_write_cmd(0x2A); 
        lcd_write_data(0x00);
        lcd_write_data(0x00);
        lcd_write_data(0x00);
        lcd_write_data(0xef);	 
        lcd_write_cmd(0x11);
        HAL_Delay(120);
        lcd_write_cmd(0x29);
    } else {
        LCD_LOG("Not find LCD Init Code, LCD ID is:%#x\r\n", lcd_params.lcd_id);
	}
    
    LCD_LOG("LCD Init Success\r\n");
    
    /* ����LCD��ʾ����������0��������1*/
	lcd_set_display_drection(LCD_DEFAULT_DISPLAY_DIR);
    
    /* ����ʾ */
    lcd_display_on();
    
    /* LCD���� */
	lcd_clear(LCD_DEFAULT_CLEAR_COLOR);

    /* �򿪱��� */
    lcd_backlight_ctrl(LCD_BACKLIGHT_ON);
    
    return;
}

/**
 * @brief    LCD��㺯��
 * @param    x_pos x��������
 * @param    y_pos y��������
 * @retval   none
*/
void lcd_draw_point(uint16_t x_pos, uint16_t y_pos, uint16_t color)
{
//    if (x_pos > lcd_params.lcd_width || y_pos > lcd_params.lcd_height) {
//        return;
//    }
    
    lcd_set_cursor(x_pos, y_pos);
    lcd_write_ram_start();
    lcd_write_ram(color);
}

/**
 * @brief    LCD���ô���
 * @param    x_pos_start x������ʼ����
 * @param    y_pos_start y������ʼ����
 * @param    width       ���ڿ��
 * @param    height      ���ڸ߶�
 * @retval   none
 * @note     �˺���ִ���꣬�����ڴ������Ͻ�
*/
void lcd_set_window(uint16_t x_pos_start, uint16_t y_pos_start, uint16_t width, uint16_t height)
{
    uint16_t x_pos_end, y_pos_end;

    x_pos_end = x_pos_start + width - 1;
    y_pos_end = y_pos_start + height - 1;
    
//    if (x_pos_end < x_pos_start || x_pos_end > lcd_params.lcd_width) {
//        return;
//    }
    
//    if (y_pos_end < y_pos_start || y_pos_end > lcd_params.lcd_height) {
//        return;
//    }
//    
    if(lcd_params.lcd_id == 0x9341 || lcd_params.lcd_id == 0x5310) {
		lcd_write_cmd(lcd_params.set_x_cmd); 
		lcd_write_data(x_pos_start >> 8); 
		lcd_write_data(x_pos_start & 0xFF);	 
		lcd_write_data(x_pos_end >> 8); 
		lcd_write_data(x_pos_end & 0xFF);  
		lcd_write_cmd(lcd_params.set_y_cmd); 
		lcd_write_data(y_pos_start >> 8); 
		lcd_write_data(y_pos_start & 0xFF); 
		lcd_write_data(y_pos_end >> 8); 
		lcd_write_data(y_pos_end & 0xFF); 
	} else if (lcd_params.lcd_id == 0x5510) {
		lcd_write_cmd(lcd_params.set_x_cmd); 
        lcd_write_data(x_pos_start >> 8); 
		lcd_write_cmd(lcd_params.set_x_cmd + 1);
        lcd_write_data(x_pos_start & 0xFF);	  
		lcd_write_cmd(lcd_params.set_x_cmd + 2);
        lcd_write_data(x_pos_end >> 8);   
		lcd_write_cmd(lcd_params.set_x_cmd + 3);
        lcd_write_data(x_pos_end & 0xFF);   
		lcd_write_cmd(lcd_params.set_y_cmd);
        lcd_write_data(y_pos_start >> 8);   
		lcd_write_cmd(lcd_params.set_y_cmd + 1);
        lcd_write_data(y_pos_start&0xFF);  
		lcd_write_cmd(lcd_params.set_y_cmd + 2);
        lcd_write_data(y_pos_end >> 8);   
		lcd_write_cmd(lcd_params.set_y_cmd + 3);
        lcd_write_data(y_pos_end & 0xFF);  
	}
}

/**
 * @brief   LCD���ߺ���
 * @param   x1 x������ʼ����
 * @param   x2 x������ֹ����
 * @param   y1 y������ʼ����
 * @param   y2 y������ֹ����
 * @return  none
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t	i = 0;
	int16_t		delta_x = 0, delta_y = 0;
	int8_t		incx = 0, incy = 0;
	uint16_t	distance = 0;
	uint16_t    t = 0;
	uint16_t	x = 0, y = 0;
	uint16_t 	x_temp = 0, y_temp = 0;
    
    if(y1 == y2)
    {
     /* ���ٻ�ˮƽ�� */
        lcd_set_window(x1, y1, x2, y2);
        lcd_write_ram_start();
        for(i = 0; i < x2 - x1; i++)
        {
            lcd_write_ram(color);
        }

        return;
    }
	else
	{
		/* ��б�ߣ�Bresenham�㷨�� */
		/* ��������֮����x��y����ļ�࣬�õ�������x��y����Ĳ���ֵ */
		delta_x = x2 - x1;
		delta_y = y2 - y1;
		if(delta_x > 0)
		{
			//б��(������)
			incx = 1;
		}
		else if(delta_x == 0)
		{
			//��ֱб��(����)
			incx = 0;
		}
		else
		{
			//б��(���ҵ���)
			incx = -1;
			delta_x = -delta_x;
		}
		if(delta_y > 0)
		{
			//б��(������)
			incy = 1;
		}
		else if(delta_y == 0)
		{
			//ˮƽб��(ˮƽ��)
			incy = 0;
		}
		else
		{
			//б��(���ҵ���)
			incy = -1;
			delta_y = -delta_y;
		}			
		
		/* ���㻭�ʴ�����(ȡ��������е����ֵ) */
		if(delta_x > delta_y)
		{
			distance = delta_x;
		}
		else
		{
			distance = delta_y;
		}
		
		/* ��ʼ��� */
		x = x1;
		y = y1;
		//��һ������Ч������t�Ĵ�����һ
		for(t = 0; t <= distance + 1;t++)
		{
			lcd_draw_point(x, y, color);
		
			/* �ж���ʵ��ֵ��������ص� */
			x_temp += delta_x;	
			if(x_temp > distance)
			{
				//x����Խ�磬��ȥ����ֵ��Ϊ��һ�μ����׼��
				x_temp -= distance;		
				//��x����������
				x += incx;
					
			}
			y_temp += delta_y;
			if(y_temp > distance)
			{
				//y����Խ�磬��ȥ����ֵ��Ϊ��һ�μ����׼��
				y_temp -= distance;
				//��y����������
				y += incy;
			}
		}
	}
}

/**
 * @breif	LCD������
 * @param   x1 x������ʼ����
 * @param   x2 x������ֹ����
 * @param   y1 y������ʼ����
 * @param   y2 y������ֹ����
 * @param	color ��ɫ
 * @retval	none
 */
void lcd_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	lcd_draw_line(x1,y1,x2,y1,color);
	lcd_draw_line(x1,y2,x2,y2,color);
	lcd_draw_line(x2,y1,x2,y2,color);
        lcd_draw_line(x1,y1,x1,y2,color);

        
}

/**
 * @breif	LCD��Բ����
 * @param   x x��������
 * @param	y ��������
 * @param   r �뾶
 * @param	color ��ɫ
 * @retval	none
 */
void lcd_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
	/* Bresenham��Բ�㷨 */
	int16_t a = 0, b = r;
    int16_t d = 3 - (r << 1);		//�㷨���߲���
		
	/* ���Բ����Ļ�ɼ������⣬ֱ���˳� */
//    if (x - r < 0 || x + r > lcd_params.lcd_width || y - r < 0 || y + r > lcd_params.lcd_height) {
//        return;
//    }
		
	/* ��ʼ��Բ */
    while(a <= b)
    {
        lcd_draw_point(x - b, y - a, color);
        lcd_draw_point(x + b, y - a, color);
        lcd_draw_point(x - a, y + b, color);
        lcd_draw_point(x - b, y - a, color);
        lcd_draw_point(x - a, y - b, color);
        lcd_draw_point(x + b, y + a, color);
        lcd_draw_point(x + a, y - b, color);
        lcd_draw_point(x + a, y + b, color);
        lcd_draw_point(x - b, y + a, color);
 	         
        a++;

        if(d < 0)
			d += 4 * a + 6;
        else
        {
            d += 10 + 4 * (a - b);
            b--;
        }

        lcd_draw_point(x + a, y + b, color);
    }
}

/**
 * @breif   LCD���һ����������
 * @param   x1 x������ʼ����
 * @param   x2 x������ֹ����
 * @param   y1 y������ʼ����
 * @param   y2 y������ֹ����
 * @param	color ��ɫ
 * @retval	none
 */
void lcd_fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t i, j;
    uint32_t xlen = 0;
    
    xlen = x2 - x1 + 1;
    for (i = y1; i <= y2; i++) {
        lcd_set_cursor(x1, i);
        lcd_write_ram_start();
        for (j = 0; j < xlen; j++) {
            lcd_write_ram(color);
        }
    }
}

/**
 * @breif   LCD���һ��Բ������
 * @param   x x����Բ������
 * @param   y y����Բ������
 * @param   r �뾶
 * @param	color ��ɫ
 * @retval	none
 */
void lcd_fill_circle(uint16_t x,uint16_t y,uint16_t r, uint16_t color)
{
    uint16_t a,b;
    
    for (b = y - r;b < y + r;b++) {
        for (a= x - r;a < x + r;a++) {
            if (((a-x) * (a-x) + (b-y) * (b-y)) <= r*r){
                lcd_draw_point(a, b, color);
            }
        }
    }
}