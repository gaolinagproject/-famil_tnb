#include "main.h"


Calendar_OBJ calendar;
//#define DS3231_WriteAddress        0xD0  
//#define DS3231_ReadAddress         0xD1
uint8_t BCD2HEX(uint8_t val)
{
    uint8_t i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}
uint16_t B_BCD(uint8_t val)
{
    uint8_t i,j,k;
    i=val/10;
    j=val%10;
    k=j+(i<<4);
    return k;
}
void DS3231_WR_Byte(uint8_t addr,uint8_t bytedata)
{
    HAL_I2C_Mem_Write(&hi2c1, DS3231_WriteAddress, addr, I2C_MEMADD_SIZE_8BIT, &bytedata, 1, 0xFFFF);
}    
uint8_t DS3231_RD_Byte(uint8_t addr)
{
    uint8_t Dat=0;
  
    HAL_I2C_Mem_Read(&hi2c1, DS3231_ReadAddress, addr, I2C_MEMADD_SIZE_8BIT, &Dat, 1, 0xFFFF);
    
    return Dat;
}
void DS3231_Init(void)
{

    DS3231_WR_Byte(0x0e,0);
    HAL_Delay(2);
    DS3231_WR_Byte(0x0f,0x0);
    HAL_Delay(2);
}
void Set_DS3231_Time(uint16_t yea,uint8_t mon,uint8_t da,uint8_t hou,uint8_t min,uint8_t sec,uint8_t week)
{
    uint8_t temp=0;
 
    yea = yea%100;
    temp=B_BCD(yea);
    DS3231_WR_Byte(0x06,temp);
  
    temp=B_BCD(mon);
    DS3231_WR_Byte(0x05,temp);
  
    temp=B_BCD(da);
    DS3231_WR_Byte(0x04,temp);
  
    temp=B_BCD(hou);
    DS3231_WR_Byte(0x02,temp);
  
    temp=B_BCD(min);
    DS3231_WR_Byte(0x01,temp);
  
    temp=B_BCD(sec);
    DS3231_WR_Byte(0x00,temp);
    
    temp=B_BCD(week);
    DS3231_WR_Byte(0x03,temp);
}
void Get_DS3231_Time(void)
{
    calendar.w_year=DS3231_RD_Byte(0x06);  
    calendar.w_year=BCD2HEX(calendar.w_year);
    calendar.w_year = calendar.w_year + 2000;
    calendar.w_month=DS3231_RD_Byte(0x05);
    calendar.w_month=BCD2HEX(calendar.w_month);
    calendar.w_date=DS3231_RD_Byte(0x04);  
    calendar.w_date=BCD2HEX(calendar.w_date);
    
    calendar.hour=DS3231_RD_Byte(0x02);
    calendar.hour&=0x3f;                  
    calendar.hour=BCD2HEX(calendar.hour);
    calendar.min=DS3231_RD_Byte(0x01);
    calendar.min=BCD2HEX(calendar.min);
    calendar.sec=DS3231_RD_Byte(0x00);
    calendar.sec=BCD2HEX(calendar.sec);
    
    calendar.week=DS3231_RD_Byte(0x03);
    calendar.week=BCD2HEX(calendar.week);
    
    DS3231_WR_Byte(0x0e,0x20);
    calendar.temper_H=DS3231_RD_Byte(0x11);
    calendar.temper_L=(DS3231_RD_Byte(0x12)>>6)*25;
}