
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
    gp2y_led(1);//�򿪺��������
    delay_us(280);//��ʱ280us
    HAL_ADC_Start(&hadc1);                    //����ADC����ת��
    HAL_ADC_PollForConversion(&hadc1, 100);    //�ȴ�ADCת�����
    filter_buf[i] = HAL_ADC_GetValue(&hadc1);  //��ȡADCת������  2 us  
    delay_us(38);           //��ʱ19us����Ϊ����������AD��������     ��Ϊ3������ADת��һ�����ʱ2us��38��2�ټ�280�պ���320us,�������˵�ĸߵ�ƽ������ʱ��Ϊ0.32ms//
    gp2y_led(0);//�رպ��������
    delay_us(9680);         //��ʱ9680us
    
    
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

  filter_sum = filter_sum - filter_max - filter_min + i/2;          //����i/2��Ϊ����������//

  filter_sum = filter_sum / i;

  calcVoltage = filter_sum*(3.3/ 4096)*2;  //��Ϊ����ɼ�������ADֵ������Ҫ����ת��Ϊ��ѹֵ//

  // ��ע��Ϊʲô���ڹ�ʽ�����Ҫ����2�أ����2��һ�������ٵģ���Ϊ���ǽ�����32��Ƭ�����ӵ�ʱ�򣬼�������10K�ĵ����ѹ����ȡ���ĵ�ѹֵ�Ѽ�С��һ�룬���ⲻ��ʵ�ʲ�ȡ���ĵ�ѹ������Ҫ��������2����ԭ���ĵ�ѹֵ��

  dust = (0.17*calcVoltage-0.1)*1000;  //����1000��λ����ug/m3//

  if (dust<0)

    dust=0;            //��λ//

  if (dust>500)        

    dust=500;

  printf("\r\n��ѹֵ:%fV\n",calcVoltage);

  printf("\r\n�ҳ�Ũ��:%dug/m3\n",dust);

  
}