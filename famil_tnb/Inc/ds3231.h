#ifndef __DS3231_H
#define __DS323_H

#define DS3231_WriteAddress        0xD0  
#define DS3231_ReadAddress         0xD1

typedef struct
{
    uint8_t    hour;
    uint8_t    min;
    uint8_t    sec;            
    uint32_t w_year;
    uint8_t  w_month;
    uint8_t  w_date;
    uint8_t  week;        
    uint8_t    temper_H;
    uint8_t    temper_L;
}Calendar_OBJ;
extern Calendar_OBJ calendar;    //日历结构体
extern uint8_t const mon_table[12];    //月份日期数据表
void DS3231_Init(void);
void Get_DS3231_Time(void);      
void Set_DS3231_Time(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec,uint8_t week);//设置时间   

#endif

