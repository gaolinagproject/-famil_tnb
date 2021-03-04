#ifndef	__GUI_COMMON_H__
#define	__GUI_COMMON_H__

#include "DIALOG.h"
#include "stdint.h"


#define UCLOR_WHITE       0xFFF8F8F8
#define UCLOR_GREEN       0xFF00CC30
#define UCLOR_RED         0xFF3500CC
#define UCLOR_CYAN        0xFFD1C238
#define UCLOR_BLACK       0x030303
#define UCLOR_CYAN2       0xFFD9CB40

#define UCLOR_TITLE       0xFF938500

#define UCLOR_BTN_PRS     UCLOR_GREEN//0xFF787000

#define UCLOR_DARK_WHITE      0xFFF3F3F3
#define UCLOR_BLK_WHITE     0xFFE8E8E8



extern int *setValue_data;



typedef  struct user_data{
	void *data;
	unsigned short judge;
} user_data_t;



extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_64;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_32;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_25;

void button_user_skin_set(BUTTON_Handle hItem,char *str,void *dataSrc,unsigned short judge);


#endif 

