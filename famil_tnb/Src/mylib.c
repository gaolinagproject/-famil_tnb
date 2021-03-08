#include "main.h"

//*********************MP3模块部分*******************************************************************

#define OSC_FREQ        11059200  //11059200  


static uint8_t Send_buf[10] = {0} ;






/*****************************************************************************************************
 - 功能描述： 串口发送一帧数据
 - 隶属模块： 内部 
 - 参数说明： 
 - 返回说明： 
 - 注：无     
*****************************************************************************************************/
void SendCmd(uint8_t len)
{
    uint8_t i = 0x7E, j =  0xEF;

    
    HAL_UART_Transmit(&huart3, (uint8_t *)&i, 1, 0xFFFF);
    HAL_UART_Transmit(&huart3, (uint8_t*)Send_buf, len, 0xFFFF);
    HAL_UART_Transmit(&huart3, (uint8_t *)&j, 1, 0xFFFF);
}

/********************************************************************************************
 - 功能描述：求和校验
 - 隶属模块：
 - 参数说明：
 - 返回说明：
 - 注：      和校验的思路如下
             发送的指令，去掉起始和结束。将中间的6个字节进行累加，最后取反码
             接收端就将接收到的一帧数据，去掉起始和结束。将中间的数据累加，再加上接收到的校验
             字节。刚好为0.这样就代表接收到的数据完全正确。
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
 - 功能描述： 串口向外发送命令[包括控制和查询]
 - 隶属模块： 外部
 - 参数说明： CMD:表示控制指令，请查阅指令表，还包括查询的相关指令
              feedback:是否需要应答[0:不需要应答，1:需要应答]
              data:传送的参数
 - 返回说明：
 - 注：       
********************************************************************************************/
void Uart_SendCMD(uint8_t CMD ,uint8_t feedback , uint16_t dat)
{
    Send_buf[0] = 0xff;    //保留字节 
    Send_buf[1] = 0x06;    //长度
    Send_buf[2] = CMD;     //控制指令
    Send_buf[3] = feedback;//是否需要反馈
    Send_buf[4] = (uint8_t)(dat >> 8);//datah
    Send_buf[5] = (uint8_t)(dat);     //datal
    DoSum(&Send_buf[0],6);        //校验
    SendCmd(8);       //发送此帧数据
}

void mp3_test()
{ 


    vTaskDelay( 2000 / portTICK_PERIOD_MS);
    
    Uart_SendCMD(0x03 , 0 , 0x02) ;//播放第一首
    
    vTaskDelay( 2000 / portTICK_PERIOD_MS);
    
    Uart_SendCMD(0x03 , 0 , 0x01) ;//播放第二首

    vTaskDelay( 2000 / portTICK_PERIOD_MS);

    Uart_SendCMD(0x03 , 0 , 0x03) ;//播放第四首

}

void mp3_control(uint8_t num)
{
  Uart_SendCMD(0x03 , 0 , num) ;//播放第一首
}

//******************************************sgp30部分******************************************

sgp30_data_t sgp30_data;

/**
 * @brief    向SGP30发送一条指令(16bit)
 * @param    cmd SGP30指令
 * @retval    成功返回HAL_OK
*/
static uint8_t sgp30_send_cmd(sgp30_cmd_t cmd)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
    return HAL_I2C_Master_Transmit(&hi2c1, SGP30_ADDR_WRITE, (uint8_t*) cmd_buffer, 2, 0xFFFF);
}

/**
 * @brief    软复位SGP30
 * @param    none
 * @retval    成功返回HAL_OK
*/
static int sgp30_soft_reset(void)
{
    uint8_t cmd = 0x06;
    return HAL_I2C_Master_Transmit(&hi2c1, 0x00, &cmd, 1, 0xFFFF);
}

/**
 * @brief    初始化SGP30空气质量测量模式
 * @param    none
 * @retval    成功返回0，失败返回-1
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
 * @brief    初始化SGP30空气质量测量模式
 * @param    none
 * @retval    成功返回HAL_OK
*/
static int sgp30_start(void)
{
    return sgp30_send_cmd(MEASURE_AIR_QUALITY);
}

#define CRC8_POLYNOMIAL 0x31

uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;        //余数
    uint8_t  i = 0, j = 0;  //循环变量

    /* 初始化 */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* 从最高位开始依次计算  */
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

    /* 返回计算的CRC码 */
    return remainder;
}


/**
 * @brief    读取一次空气质量数据
 * @param    none
 * @retval    成功返回0，失败返回-1
*/
int spg30_read(void)
{
    int status;
    uint8_t recv_buffer[6]={0};

    /* 启动测量 */
    status = sgp30_start();
    if (status != 0) {
        printf("sgp30 start fail\r\n");
        return -1;
    }

    vTaskDelay( 100 / portTICK_PERIOD_MS);

    /* 读取测量数据 */
    status = HAL_I2C_Master_Receive(&hi2c1, SGP30_ADDR_READ, (uint8_t*)recv_buffer, 6, 0xFFFF);
    if (status != HAL_OK) {
        printf("I2C Master Receive fail\r\n");
        return -1;
    }

    /* 校验接收的测量数据 */
    if (CheckCrc8(&recv_buffer[0], 0xFF) != recv_buffer[2]) {
        printf("co2 recv data crc check fail\r\n");
        return -1;
    }
    if (CheckCrc8(&recv_buffer[3], 0xFF) != recv_buffer[5]) {
        printf("tvoc recv data crc check fail\r\n");
        return -1;
    }


    /* 转换测量数据 */
    sgp30_data.co2  = recv_buffer[0] << 8 | recv_buffer[1];
    sgp30_data.tvoc = recv_buffer[3] << 8 | recv_buffer[4];

    return 0;
}