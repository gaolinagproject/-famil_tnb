
#include "main.h"



      
void gp2y_led(uint8_t io)
{
  if(io == 1)
  {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET);   	
  }
  else
  {
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
  }
}
void gp2y_data()
{
  int filter_buf[FILTER_N] = {0}, filter_max = 0,filter_min = 0,filter_sum = 0,dust = 0,i = 0;
  float calcVoltage = 0;
  
  for(i = 0;i < FILTER_N; i++)
  {
    gp2y_led(1);//打开红外二极管
    delay_us(280);//延时280us
    HAL_ADC_Start(&hadc1);                    //启动ADC单次转换
    HAL_ADC_PollForConversion(&hadc1, 100);    //等待ADC转换完成
    filter_buf[i] = HAL_ADC_GetValue(&hadc1);  //读取ADC转换数据  2 us  
    delay_us(38);           //延时19us，因为这里我设了AD采样的周     期为3，所以AD转换一次需耗时2us，38加2再加280刚好是320us,这跟上面说的高电平持续的时间为0.32ms//
    gp2y_led(0);//关闭红外二极管
    delay_us(9680);         //延时9680us
    
    
  }
  
  filter_max = filter_buf[0];

  filter_min = filter_buf[0];

  filter_sum = filter_buf[0];
  
  for(i = FILTER_N - 1; i > 0; i--)

  {

    if(filter_buf[i] > filter_max)

      filter_max=filter_buf[i];

    else if(filter_buf[i] < filter_min)

      filter_min=filter_buf[i];

    filter_sum = filter_sum + filter_buf[i];

    filter_buf[i] = filter_buf[i - 1];

  }

  i = FILTER_N - 2;

  filter_sum = filter_sum - filter_max - filter_min + i/2;          //加上i/2是为了四舍五入//

  filter_sum = filter_sum / i;

  calcVoltage = filter_sum*(3.3/ 4096)*2;  //因为上面采集到的是AD值，这里要把它转换为电压值//

  // （注：为什么我在公式的最后要乘以2呢？这个2是一定不能少的，因为我们将它与32单片机连接的时候，加了两个10K的电阻分压，读取到的电压值已减小了一半，但这不是实际采取到的电压，所以要在最后乘以2返回原来的电压值）

  dust = (0.17*calcVoltage-0.1)*1000;  //乘以1000单位换成ug/m3//

  if (dust<0)

    dust=0;            //限位//

  if (dust>500)        

    dust=500;

  printf("\r\n电压值:%fV\n",calcVoltage);

  printf("\r\n灰尘浓度:%dug/m3\n",dust);

  
}