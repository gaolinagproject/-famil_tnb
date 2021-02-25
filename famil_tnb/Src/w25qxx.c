#include "main.h"

/* W25Q64��ָ�� */
uint8_t w25x_read_id = 0x90;					// ��ID
uint8_t m_addr[3]    = {0,0,0};					// ���Ե�ַ0x000000
uint8_t check_addr   = 0x05;					// �����·�Ƿ�æ
uint8_t enable_write = 0x06;					// ʹ���˲��ܸı�оƬ����
uint8_t erase_addr   = 0x20;					// ��������
uint8_t write_addr   = 0x02;					// д��������
uint8_t read_addr    = 0x03;					// ����������

/* ��ID */
void ReadID(void)
{
	uint8_t temp_ID[5] = {0,0,0,0,0};						// ���ջ���
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &w25x_read_id, 1, 100);		// ��ID����ָ��
	HAL_SPI_Receive(&hspi1, temp_ID, 5, 100);				// ��ȡID
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
	
	/* ���Դ�ӡ */
	printf("readID is %x%x\n",temp_ID[3],temp_ID[4]);
}

/* ����Ƿ�æ */
void CheckBusy(void)
{
	uint8_t status=1;
	uint32_t timeCount=0;
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF) //�ȴ���ʱ
		{
			return ;
		}
		
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS	
		HAL_SPI_Transmit(&hspi1, &check_addr, 1, 100);			// ����ָ��
		HAL_SPI_Receive(&hspi1, &status, 1, 100);				// ��ȡ
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
		
	}while((status&0x01)==0x01);
}


/* д������ */
void ReadData(void)
{
	uint8_t temp_wdata[5] = {0x99,0x88,0x77,0x66,0x55};		// ��Ҫд�������
	
	/* ����Ƿ�æ */
	CheckBusy();
	
	/* дʹ�� */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// ����ָ��
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
	
	/* ���� */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &erase_addr, 1, 100);			// ����ָ��
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// ���͵�ַ
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
	
	/* �ٴμ���Ƿ�æ */
	CheckBusy();
	
	/* дʹ�� */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &enable_write, 1, 100);		// ����ָ��
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS

	/* д���� */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &write_addr, 1, 100);			// ����ָ��
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// ��ַ
	HAL_SPI_Transmit(&hspi1, temp_wdata, 5, 100);			// д������
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
}

/* ��ȡ���� */
void WriteData(void)
{
	uint8_t temp_rdata[5] = {0,0,0,0,0};					// �������ݱ����buff
	
	
	/* ����Ƿ�æ */
	CheckBusy();	
	
	/* ��ʼ������ */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);	// ʹ��CS
	HAL_SPI_Transmit(&hspi1, &read_addr, 1, 100);			// ������ָ��
	HAL_SPI_Transmit(&hspi1, m_addr, 3, 100);				// ��ַ
	HAL_SPI_Receive(&hspi1, temp_rdata, 5, 100);			// �õ�����
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);	// ʧ��CS
	
	/* ���Դ�ӡ */
	printf("Read flash data is:%x %x %x %x %x\n",temp_rdata[0],temp_rdata[1],temp_rdata[2],temp_rdata[3],temp_rdata[4]);
}
