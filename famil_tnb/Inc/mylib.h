#ifndef __MYLIB_H
#define __MYLIB_H
#include "main.h"
//mp3����
void mp3_test();

//sgp30����
#define SGP30_ADDR          0x58
#define    SGP30_ADDR_WRITE    SGP30_ADDR<<1       //0xb0
#define    SGP30_ADDR_READ        (SGP30_ADDR<<1)+1   //0xb1

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
#endif 