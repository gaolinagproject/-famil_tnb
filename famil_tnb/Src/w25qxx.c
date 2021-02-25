#include "main.h"

/* W25Q64的指令 */
uint8_t w25x_read_id = 0x90;					// 读ID
uint8_t m_addr[3]    = {0,0,0};					// 测试地址0x000000
uint8_t check_addr   = 0x05;					// 检查线路是否繁忙
uint8_t enable_write = 0x06;					// 使能了才能改变芯片数据
uint8_t erase_addr   = 0x20;					// 擦除命令
uint8_t write_addr   = 0x02;					// 写数据命令
uint8_t read_addr    = 0x03;					// 读数据命令

/* 读ID */
void ReadID(void)
{
	uint8_t temp_ID[5] = {0,0,0,0,0};						// 接收缓存
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &w25x_read_id, 1, 100);		// 读ID发送指令
	HAL_SPI_Receive(&hspi1, temp_ID, 5, 100);				// 读取ID
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
	
	/* 测试打印 */
	printf("readID is %x%x\n",temp_ID[3],temp_ID[4]);
}

/* 检查是否繁忙 */
void CheckBusy(void)
{
	uint8_t status=1;
	uint32_t timeCount=0;
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF) //等待超时
		{
			return ;
		}
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS	
		HAL_SPI_Transmit(&hspi1, &check_addr, 1, 100);			// 发送指令
		HAL_SPI_Receive(&hspi1, &status, 1, 100);				// 读取
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
		
	}while((status&0x01)==0x01);
}


/* 写入数据 */
void ReadData(void)
{
	uint8_t temp_wdata[5] = {0x99,0x88,0x77,0x66,0x55};		// 需要写入的数据
	
	/* 检查是否繁忙 */
	CheckBusy();
	
	/* 写使能 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// 发送指令
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
	
	/* 擦除 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &erase_addr, 1, 100);			// 发送指令
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// 发送地址
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
	
	/* 再次检查是否繁忙 */
	CheckBusy();
	
	/* 写使能 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// 发送指令
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS

	/* 写数据 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// 发送指令
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// 地址
	HAL_SPI_Transmit(&hspi1, temp_wdata, 5, 100);			// 写入数据
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
}

/* 读取数据 */
void WriteData(void)
{
	uint8_t temp_rdata[5] = {0,0,0,0,0};					// 读出数据保存的buff
	
	
	/* 检查是否繁忙 */
	CheckBusy();	
	
	/* 开始读数据 */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// 使能CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// 读发送指令
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// 地址
	HAL_SPI_Receive(&hspi1, temp_rdata, 5, 100);			// 拿到数据
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// 失能CS
	
	/* 测试打印 */
	printf("Read flash data is:%x %x %x %x %x\n",temp_rdata[0],temp_rdata[1],temp_rdata[2],temp_rdata[3],temp_rdata[4]);
}
