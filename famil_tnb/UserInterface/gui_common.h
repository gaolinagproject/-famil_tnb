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

#define	WARN_CHOICE								(1<<31)
#define	WARN_DOUBLE_TEXT						(1<<8)

#define	WARN_BLACK_TEXT							(0<<0)
#define	WARN_RED_TEXT							(1<<0)
#define	WARN_COLOUR_MASK						(0xFF)

extern int *setValue_data;



typedef struct data_set{

    unsigned short  month;
    unsigned short  date;
    unsigned short  week; 
    unsigned short  hour;
    unsigned short  min;
    unsigned short  a_clock_num[3];
    unsigned short  a_clock_hour[3];
    unsigned short  a_clock_min[3];
    unsigned short  year;
    
}data_set_t;

extern data_set_t data_set_lcd;

typedef  struct user_data{
	void *data;
	unsigned short judge;
} user_data_t;


/*******************��������**********************/


#define	KEYPAD_TITLE_MAX						32
#define	KEYPAD_STRING_MAX						32

typedef enum passwordOperateType
{
	eSetPassword,
	eConfirmPassword
}passwordOperateType_t;



typedef enum keypadType
{
	eKeypadString,
	eKeypadInt,
	eKeypadInt32,
	eKeypadFloat,
	eKeypadIP
}keypadType_t;

typedef struct keypad
{
	WM_HWIN hWinParent;			//����Ĵ��� 
	int itemId;					//edit ID
	char title[KEYPAD_TITLE_MAX];		//���̵ı���
	keypadType_t type;			//���̻�ȡֵ������
	union 
	{
		char *valueStr;			//�ַ���
		unsigned short int *valueInt;			//����
		unsigned  int *valueInt32;			//����
		float *valueFloat;		//������(������Ĭ�϶���2λ��Ч����)
	}value;
	int min;					//�����ַ������ͣ���ʾ��С�ַ��������������κ͸�����Ϊ��Сֵ
	int max;					//�����ַ������ͣ���ʾ�����ַ��������������κ͸�����Ϊ���ֵ
	//���ֵ��Сֵ��Ϊ0ʱ�������������������ж�
}keypad_t;

extern keypad_t keypad;

extern WM_HWIN ui_main(void);
extern WM_HWIN ui_clock(void);


void CreateUiKeypad(void);
void CreateUiWarning(void);

extern int uiWarning(WM_HWIN hWinParent,int type,char *title,char *str,int timeOut );
extern void keypadEditFloat(WM_HWIN hWin,int id,int min,int max,float *setValue);
extern void keypadEditInt(WM_HWIN hWin,int id,int min,int max,unsigned short int *value);
extern void keypadEditInt32(WM_HWIN hWin,int id,int min,int max,unsigned  int *value);
//void keypadEditFloat(WM_HWIN hWin,int id,int min,int max,float *setValue);
extern int uiKeypad(WM_HWIN hWinParent,int id);

extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_64;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_32;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font_micro_25;

void button_user_skin_set(BUTTON_Handle hItem,char *str,void *dataSrc,unsigned short judge);
void button_user_clock_set(BUTTON_Handle hItem,char *str,void *dataSrc,unsigned short judge);

#endif 

