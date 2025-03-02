#ifndef __TOUCH_H__
#define __TOUCH_H__
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "stm32f4xx.h"
#include "stm32f4xx_ll_usart.h"   
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046/OTT2001A等） 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/7/19
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									   
//********************************************************************************
//修改说明 
//V1.1 	20170606
//1，新增对GT9271触摸IC的支持
//2，新增对10点触摸的支持
////////////////////////////////////////////////////////////////////////////////// 


#define TP_PRES_DOWN 0x80  //触屏被按下	  
#define TP_CATH_PRES 0x40  //有按键按下了 
#define CT_MAX_TOUCH 10    //电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct
{
	uint8_t (*init)(void);			//初始化触摸屏控制器
	uint8_t (*scan)(uint8_t);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);		//触摸屏校准 
	uint16_t x[CT_MAX_TOUCH]; 		//当前坐标
	uint16_t y[CT_MAX_TOUCH];		//电容屏有最多10组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[9],y[9]存储第一次按下时的坐标. 
	uint16_t sta;					//笔的状态 
								//b15:按下1/松开0; 
	                            //b14:0,没有按键按下;1,有按键按下. 
								//b13~b10:保留
								//b9~b0:电容触摸屏按下的点数(0,表示未按下,1表示按下)
/////////////////////触摸屏校准参数(电容屏不需要校准)//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//b0:0,竖屏(适合左右为X坐标,上下为Y坐标的TP)
//   1,横屏(适合左右为Y坐标,上下为X坐标的TP) 
//b1~6:保留.
//b7:0,电阻屏
//   1,电容屏 
	uint8_t touchtype;

}_m_tp_dev;

typedef struct
{
    uint16_t xfac_f;
    uint16_t yfac_f;
    short  xoff_f;
    short  yoff_f;
    uint8_t  touchtype_f;
    uint8_t  savepass_f;
    int obligate;
    
}s_flash_tp_dve;

typedef union 
{
  s_flash_tp_dve f_tp_dve;
  uint32_t f_tp_buf[3];
  uint8_t f_tp_cbuf[12];
}un_tp_dve;

extern un_tp_dve   flash_tp_dve;
extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

//电阻屏芯片连接引脚	   

#define PEN  		PCin(5)  	//T_PEN
#define DOUT 		PBin(14)   	//T_MISO
#define TDIN 		PBout(15)  	//T_MOSI
#define TCLK 		PBout(13)  	//T_SCK
#define TCS  		PBout(12)  	//T_CS     
//电阻屏函数
void TP_Write_Byte(uint8_t num);						//向控制芯片写入一个数据
uint16_t TP_Read_AD(uint8_t CMD);							//读取AD转换值
uint16_t TP_Read_XOY(uint8_t xy);							//带滤波的坐标读取(X/Y)
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);					//双方向读取(X+Y)
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);					//带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(uint16_t x,uint16_t y,uint16_t color);//画一个坐标校准点
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);	//画一个大点
void TP_Save_Adjdata(void);						//保存校准参数
uint8_t TP_Get_Adjdata(void);						//读取校准参数
void TP_Adjust(void);							//触摸屏校准
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);//显示校准信息
//电阻屏/电容屏 共用函数
uint8_t TP_Scan(uint8_t tp);								//扫描
extern uint8_t TP_Init(void);								//初始化
 
uint16_t  read_input_X(void);
uint16_t  read_input_Y(void);


typedef struct 
{
	uint16_t x_val;
	uint16_t y_val;
	uint8_t  key_stat_fg;    //按键状态标志
	uint8_t key_hold_time;   //按键按下时间
} s_key_dev;

void read_input_XY(void);

extern s_key_dev s_key_dv;

#endif

















