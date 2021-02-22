#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"



typedef uint16_t u16;
typedef uint32_t u32;



//IO操作函数
void DHT11_DQ_OUT(unsigned char i);
unsigned char DHT11_DQ_IN();

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi); //读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11
uint8_t DHT11_Read_Data_Float(float *temp,float *humi);


#endif