#include "main.h"

//*********************MP3ģ�鲿��*******************************************************************

#define OSC_FREQ        11059200  //11059200  


static uint8_t Send_buf[10] = {0} ;






/*****************************************************************************************************
 - ���������� ���ڷ���һ֡����
 - ����ģ�飺 �ڲ� 
 - ����˵���� 
 - ����˵���� 
 - ע����     
*****************************************************************************************************/
void SendCmd(uint8_t len)
{
    uint8_t i = 0x7E, j =  0xEF;

    
    HAL_UART_Transmit(&huart3, (uint8_t *)&i, 1, 0xFFFF);
    HAL_UART_Transmit(&huart3, (uint8_t*)Send_buf, len, 0xFFFF);
    HAL_UART_Transmit(&huart3, (uint8_t *)&j, 1, 0xFFFF);
}

/********************************************************************************************
 - �������������У��
 - ����ģ�飺
 - ����˵����
 - ����˵����
 - ע��      ��У���˼·����
             ���͵�ָ�ȥ����ʼ�ͽ��������м��6���ֽڽ����ۼӣ����ȡ����
             ���ն˾ͽ����յ���һ֡���ݣ�ȥ����ʼ�ͽ��������м�������ۼӣ��ټ��Ͻ��յ���У��
             �ֽڡ��պ�Ϊ0.�����ʹ�����յ���������ȫ��ȷ��
********************************************************************************************/
void DoSum( uint8_t *Str, uint8_t len)
{
    uint16_t xorsum = 0;
    uint8_t i;

    for(i=0; i<len; i++)
    {
        xorsum  = xorsum + Str[i];
    }
	xorsum     = 0 -xorsum;
	*(Str+i)   = (uint8_t)(xorsum >>8);
	*(Str+i+1) = (uint8_t)(xorsum & 0x00ff);
}


/********************************************************************************************
 - ���������� �������ⷢ������[�������ƺͲ�ѯ]
 - ����ģ�飺 �ⲿ
 - ����˵���� CMD:��ʾ����ָ������ָ�����������ѯ�����ָ��
              feedback:�Ƿ���ҪӦ��[0:����ҪӦ��1:��ҪӦ��]
              data:���͵Ĳ���
 - ����˵����
 - ע��       
********************************************************************************************/
void Uart_SendCMD(uint8_t CMD ,uint8_t feedback , uint16_t dat)
{
    Send_buf[0] = 0xff;    //�����ֽ� 
    Send_buf[1] = 0x06;    //����
    Send_buf[2] = CMD;     //����ָ��
    Send_buf[3] = feedback;//�Ƿ���Ҫ����
    Send_buf[4] = (uint8_t)(dat >> 8);//datah
    Send_buf[5] = (uint8_t)(dat);     //datal
    DoSum(&Send_buf[0],6);        //У��
    SendCmd(8);       //���ʹ�֡����
}

void mp3_test()
{ 


    vTaskDelay( 2000 / portTICK_PERIOD_MS);
    
    Uart_SendCMD(0x03 , 0 , 0x02) ;//���ŵ�һ��
    
    vTaskDelay( 2000 / portTICK_PERIOD_MS);
    
    Uart_SendCMD(0x03 , 0 , 0x01) ;//���ŵڶ���

    vTaskDelay( 2000 / portTICK_PERIOD_MS);

    Uart_SendCMD(0x03 , 0 , 0x03) ;//���ŵ�����

}

void mp3_control(uint8_t num)
{
  Uart_SendCMD(0x03 , 0 , num) ;//���ŵ�һ��
}

//******************************************sgp30����******************************************

sgp30_data_t sgp30_data;

/**
 * @brief    ��SGP30����һ��ָ��(16bit)
 * @param    cmd SGP30ָ��
 * @retval    �ɹ�����HAL_OK
*/
static uint8_t sgp30_send_cmd(sgp30_cmd_t cmd)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
    return HAL_I2C_Master_Transmit(&hi2c1, SGP30_ADDR_WRITE, (uint8_t*) cmd_buffer, 2, 0xFFFF);
}

/**
 * @brief    ��λSGP30
 * @param    none
 * @retval    �ɹ�����HAL_OK
*/
static int sgp30_soft_reset(void)
{
    uint8_t cmd = 0x06;
    return HAL_I2C_Master_Transmit(&hi2c1, 0x00, &cmd, 1, 0xFFFF);
}

/**
 * @brief    ��ʼ��SGP30������������ģʽ
 * @param    none
 * @retval    �ɹ�����0��ʧ�ܷ���-1
*/
int sgp30_init(void)
{
    int status;

    status = sgp30_soft_reset();
    if (status != HAL_OK) {
        return -1;
    }

    HAL_Delay(100);

    status = sgp30_send_cmd(INIT_AIR_QUALITY);

    HAL_Delay(100);

    return status == 0 ? 0 : -1;
}

/**
 * @brief    ��ʼ��SGP30������������ģʽ
 * @param    none
 * @retval    �ɹ�����HAL_OK
*/
static int sgp30_start(void)
{
    return sgp30_send_cmd(MEASURE_AIR_QUALITY);
}

#define CRC8_POLYNOMIAL 0x31

uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;        //����
    uint8_t  i = 0, j = 0;  //ѭ������

    /* ��ʼ�� */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* �����λ��ʼ���μ���  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* ���ؼ����CRC�� */
    return remainder;
}


/**
 * @brief    ��ȡһ�ο�����������
 * @param    none
 * @retval    �ɹ�����0��ʧ�ܷ���-1
*/
int spg30_read(void)
{
    int status;
    uint8_t recv_buffer[6]={0};

    /* �������� */
    status = sgp30_start();
    if (status != 0) {
        printf("sgp30 start fail\r\n");
        return -1;
    }

    vTaskDelay( 100 / portTICK_PERIOD_MS);

    /* ��ȡ�������� */
    status = HAL_I2C_Master_Receive(&hi2c1, SGP30_ADDR_READ, (uint8_t*)recv_buffer, 6, 0xFFFF);
    if (status != HAL_OK) {
        printf("I2C Master Receive fail\r\n");
        return -1;
    }

    /* У����յĲ������� */
    if (CheckCrc8(&recv_buffer[0], 0xFF) != recv_buffer[2]) {
        printf("co2 recv data crc check fail\r\n");
        return -1;
    }
    if (CheckCrc8(&recv_buffer[3], 0xFF) != recv_buffer[5]) {
        printf("tvoc recv data crc check fail\r\n");
        return -1;
    }


    /* ת���������� */
    sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
    sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];

    return 0;
}