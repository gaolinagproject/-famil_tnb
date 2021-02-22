#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"



typedef uint16_t u16;
typedef uint32_t u32;



//IO��������
void DHT11_DQ_OUT(unsigned char i);
unsigned char DHT11_DQ_IN();

uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi); //��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11
uint8_t DHT11_Read_Data_Float(float *temp,float *humi);


#endif