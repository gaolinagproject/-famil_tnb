#ifndef __MYLIB_H
#define __MYLIB_H
#include "main.h"
//mp3����
void mp3_test();

//sgp30����
#define SGP30_ADDR          0x58
#define SGP30_ADDR_WRITE    SGP30_ADDR<<1       //0xb0
#define SGP30_ADDR_READ        (SGP30_ADDR<<1)+1   //0xb1

typedef struct sgp30_data_st {
    uint16_t co2;
    uint16_t tvoc;
}sgp30_data_t;

typedef enum sgp30_cmd_en {
    /* ��ʼ�������������� */
    INIT_AIR_QUALITY = 0x2003,

    /* ��ʼ������������ */
    MEASURE_AIR_QUALITY = 0x2008

} sgp30_cmd_t;
extern sgp30_data_t sgp30_data;
int spg30_read(void);
int sgp30_init(void);
//ϵͳ����������
typedef struct sensor_data{

  uint8_t  min;
  uint8_t  month;
  uint8_t  date;
  uint8_t  week; 
  uint8_t  hour;
  uint8_t  sec;            
  
  uint8_t  temp;
  uint8_t  humi;
  uint8_t  ds3231;
  uint8_t  dht11;
  uint8_t  sgp30;
  uint16_t co2;
  uint16_t tvoc;
  uint32_t year;
  
}sensor_data_t;
extern sensor_data_t sensor_data_core;
extern sensor_data_t sensor_data_lcd;
#endif 