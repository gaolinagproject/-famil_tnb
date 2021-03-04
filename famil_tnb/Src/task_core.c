#include "main.h"

sensor_data_t sensor_data_core;
uint8_t temp = 0,humi = 0;
void data_copy()
{
  sensor_data_core.year = calendar.w_year;
  sensor_data_core.month = calendar.w_month;
  sensor_data_core.date = calendar.w_date;
  sensor_data_core.hour = calendar.hour;
  sensor_data_core.min = calendar.min;
  sensor_data_core.sec = calendar.sec;
  sensor_data_core.week = calendar.week;
  
  
  sensor_data_core.temp = temp;
  sensor_data_core.humi = humi;
  sensor_data_core.co2 = sgp30_data.co2;
  sensor_data_core.tvoc = sgp30_data.tvoc;
}

void Task_Core(void *pvParameters)
{
  
  
  DS3231_Init();
  Set_DS3231_Time(2021,2,25,10,10,10,4);
  
  if (-1 == sgp30_init()) {
          printf("sgp30 init fail\r\n");
  }
  printf("sgp30 init success\r\n");
  
  DHT11_Init();
  while(1)
  {
    
//    mp3_test();
    Get_DS3231_Time(); 
    sensor_data_core.ds3231 = 1;
    
    printf("syear %d - smon %d  - sday %d - hour %d - min %d - sec %d -s week %d \n",
           calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec,calendar.week);
    
    if( -1 == spg30_read()) {
      sensor_data_core.sgp30 = 0;
      printf("sgp30 read fail\r\n");
    }else {
      sensor_data_core.sgp30 = 1;
      printf("sgp30 read success, co2:%4d ppm, tvoc:%4d ppd\r\n", sgp30_data.co2, sgp30_data.tvoc);
    }

    
    if(DHT11_Read_Data(&temp,&humi) == 0){
      sensor_data_core.dht11 = 1;
      printf("temp = %d\n humi = %d\n",temp,humi);
    }else{
      sensor_data_core.dht11 = 0;
      printf("dht11 read fail\r\n");
    }
    data_copy();
    xQueueSend(ParaSensorQue,( void * ) &sensor_data_core, 0);
    
    vTaskDelay( 1000 / portTICK_PERIOD_MS);
  }
}