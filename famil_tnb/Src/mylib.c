#include "main.h"

//MP3模块部分

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