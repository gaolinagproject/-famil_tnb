#include "main.h"


sensor_data_t sensor_data_core;
data_set_t data_set_core;
un_system_data_t  un_system_data_core;

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
void sensor_init()
{
  DS3231_Init();

  
  if (-1 == sgp30_init()) {
          printf("sgp30 init fail\r\n");
  }
  printf("sgp30 init success\r\n");
  
  DHT11_Init();
}
void sensor_run()
{
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
}

    
void clock_comparison(uint8_t i)
{

  if(un_system_data_core.system_data_core.a_clock_hour[i] == sensor_data_core.hour){
    if(un_system_data_core.system_data_core.a_clock_min[i] == sensor_data_core.min){
      if(sensor_data_core.sec < 2){
       // mp3_control(2);
        xSemaphoreGive(ClockArmSemaphore);
        if(ButtonArmSemaphore != NULL) //消耗信号量
            xSemaphoreTake(ButtonArmSemaphore,0);
      }

      //铃声
    }
  }
}
void clock_core()
{
    BaseType_t err = pdFALSE;
    
    int a = 0;
    uint8_t i = 0;
    static uint16_t j = 0, y = 0, on_off = 0;
    if(xQueueReceive(ParaClockQue, &data_set_core, 0) == pdTRUE){
      if(data_set_core.time_en == 1){
          Set_DS3231_Time(data_set_core.year,data_set_core.month,data_set_core.date,data_set_core.hour,data_set_core.min,0,data_set_core.week);
          data_set_core.time_en = 0;
          
      }
      for(i = 0 ;i < 3 ;i++){
        un_system_data_core.system_data_core.a_clock_hour[i] = data_set_core.a_clock_hour[i];
        un_system_data_core.system_data_core.a_clock_min[i] = data_set_core.a_clock_min[i];
        un_system_data_core.system_data_core.a_clock_num[i] = data_set_core.a_clock_num[i];
      }
      un_system_data_core.system_data_core.start_time[0] = data_set_core.start_time[0];
      un_system_data_core.system_data_core.start_time[1] = data_set_core.start_time[1];
      un_system_data_core.system_data_core.arm_time = data_set_core.arm_time;
      
      W25QXX_Write(un_system_data_core.system_data_buf,ADDR_FLASH_SYSTEM,sizeof(system_data_t));
    } 
    for(i = 0 ;i < 3 ;i++){
      
      if(un_system_data_core.system_data_core.a_clock_num[i] == 0){ 
//        break;
      }else if(un_system_data_core.system_data_core.a_clock_num[i] == sensor_data_core.week){
        clock_comparison(i);
      }else if(un_system_data_core.system_data_core.a_clock_num[i] == 8){
        clock_comparison(i);
        un_system_data_core.system_data_core.a_clock_num[i] = 0;
      }else if(un_system_data_core.system_data_core.a_clock_num[i] == 9){
        clock_comparison(i);
      }else if(un_system_data_core.system_data_core.a_clock_num[i] == 10){
        if(sensor_data_core.week < 6)
          clock_comparison(i);
      }else if(un_system_data_core.system_data_core.a_clock_num[i] == 11){
        if(sensor_data_core.week > 5)
          clock_comparison(i);
      }
    }
    
    if(ClockArmSemaphore != NULL){
      err = xSemaphoreTake(ClockArmSemaphore,0);
      
      if(err == pdTRUE){
        if(j == 0){
          uiWarning(data_set_lcd.inquire_ui_hWin,WARN_RED_TEXT,"提示","时间到!",0);
          a = rand();
          
          a = (a%10)/2 + 1;
          mp3_control(a);
        }
        j++;
        if(j < un_system_data_core.system_data_core.arm_time*60){
          xSemaphoreGive(ClockArmSemaphore);
        }else if(j ==  un_system_data_core.system_data_core.arm_time*60){
          a = rand();
          a = (a%10)/2 + 1;
          mp3_control(a);
          j = 0;
        }else{
          j = 0;
//          mp3_control(6);
        }


      }
      if(ButtonArmSemaphore != NULL){
        err = xSemaphoreTake(ButtonArmSemaphore,0);
        if(err == pdTRUE){
          if(j > 0){
            j = un_system_data_core.system_data_core.arm_time*60 + 1;
          }
          mp3_control(6);
        }
      }
      
    }
//屏幕开关  
    if(on_off == 0){
      
      if((un_system_data_core.system_data_core.start_time[0] < (sensor_data_core.hour + 1)) || (un_system_data_core.system_data_core.start_time[1] > sensor_data_core.hour)){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);//reset
      }else if(un_system_data_core.system_data_core.start_time[1] == sensor_data_core.hour){
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);//set
      }
    }
    
    if(0 == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)){
      
       y++;
       if(y > 20){ //长按5S
        y = 0;
        on_off = 1;
        mp3_control(6);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);//set
      }
    }else{
      y++;
      if(y > 20){
        y = 0;
        on_off = 0;
      }
    }
    
}

void flash_cope()
{
  uint8_t i = 0;
  W25QXX_Read(un_system_data_core.system_data_buf,ADDR_FLASH_SYSTEM,sizeof(system_data_t));
  for(i = 0 ;i < 3 ;i++){
    data_set_lcd.a_clock_hour[i] = un_system_data_core.system_data_core.a_clock_hour[i];
    data_set_lcd.a_clock_min[i] = un_system_data_core.system_data_core.a_clock_min[i];
    data_set_lcd.a_clock_num[i] = un_system_data_core.system_data_core.a_clock_num[i];
  }
  data_set_lcd.start_time[0] = un_system_data_core.system_data_core.start_time[0];
  data_set_lcd.start_time[1] = un_system_data_core.system_data_core.start_time[1];
  data_set_lcd.arm_time = un_system_data_core.system_data_core.arm_time;
}
void Task_Core(void *pvParameters)
{
  unsigned int seed = 0;
  
  mp3_control(6);
  sensor_init();
  flash_cope();
  seed =  calendar.w_year + calendar.w_month + calendar.w_date + calendar.hour + calendar.min + calendar.sec;
  srand (seed); //随机函数种子播种
  while(1)
  {
    
//    mp3_test();
    sensor_run();
    clock_core();
    vTaskDelay( 500 / portTICK_PERIOD_MS);
  }
}