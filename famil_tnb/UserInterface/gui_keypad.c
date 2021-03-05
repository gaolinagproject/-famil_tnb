#include "gui_common.h"

#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "main.h"

#define	KEYPAD__DEBUG									0
#define	KEYPAD_RELEASE									1

keypad_t keypad;

//EventsFunctionList
//EndofEventsFunctionList

int *setValue_data;
/*********************************************************************
*
*       static data
*
**********************************************************************
*/

#define GUI_ID_BUTTON10   GUI_ID_USER+1
#define GUI_ID_BUTTON11   GUI_ID_USER+2
#define GUI_ID_BUTTON12   GUI_ID_USER+3
#define GUI_ID_BUTTON13   GUI_ID_USER+4
//#define GUI_ID_BUTTON14   GUI_ID_USER+5

#define KEYPAD_BUTTON_Y_SIZE	50
#define KEYPAD_BUTTON_X_SIZE	50

WM_HWIN ui_keyPad_hWin = NULL;

uint8_t T_decimals=0;

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/
 
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "set",               0,                        0,  -40, 320,280,0,0},
    { EDIT_CreateIndirect,       "",                 GUI_ID_EDIT0,            10,  10, 290,40, KEYPAD_STRING_MAX,0},
    
    { BUTTON_CreateIndirect,    "1",                 GUI_ID_BUTTON1,          7,  60, KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "2",                 GUI_ID_BUTTON2,          64, 60, KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "3",                 GUI_ID_BUTTON3,          121,60, KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "4",                 GUI_ID_BUTTON4,          178,60, KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "5",                 GUI_ID_BUTTON5,          7,120,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "6",                 GUI_ID_BUTTON6,          64,120,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "7",                 GUI_ID_BUTTON7,          121,120,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "8",                 GUI_ID_BUTTON8,          178,120,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "9",                 GUI_ID_BUTTON9,          7,180,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "0",                 GUI_ID_BUTTON0,          64,180,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    ".",                 GUI_ID_BUTTON10,         121,180,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    
    { BUTTON_CreateIndirect,    "<--",               GUI_ID_BUTTON11,         235,60,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
    { BUTTON_CreateIndirect,    "确定",              GUI_ID_BUTTON12,         178,180,50,50, 0,0},
    { BUTTON_CreateIndirect,    "返回",              GUI_ID_BUTTON13,         235,180,50,50, 0,0},
    
  // { BUTTON_CreateIndirect,    "00",                 GUI_ID_BUTTON14,         335,389,KEYPAD_BUTTON_X_SIZE, KEYPAD_BUTTON_Y_SIZE, 0,0},
};

/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
//  	WM_HWIN hWin = pMsg->hWin;
	

}

void initEditShowStr()
{

	WM_HWIN hWin = ui_keyPad_hWin;
	
	char buf[KEYPAD_STRING_MAX];
	
	switch(keypad.type)
	{
	  
		case eKeypadString:
			EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),keypad.value.valueStr);
		break;
		case eKeypadInt:
		{
			memset(buf,0,sizeof(buf));
			snprintf(buf,sizeof(buf)-1,"%d",*keypad.value.valueInt);
			EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf);	
		}
		break;
		case eKeypadInt32:
		{
			memset(buf,0,sizeof(buf));
			snprintf(buf,sizeof(buf)-1,"%d",*keypad.value.valueInt32);
			EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf);	
		}
		break;
		case eKeypadFloat:
		{
			memset(buf,0,sizeof(buf));
//                        if(ex_cllct_data.Sign_decimals_data){
//			         snprintf(buf,sizeof(buf)-1,"%.1f",*keypad.value.valueFloat);
//                          T_decimals = 2;}
//                        else{
//                          
//                          T_decimals = 3;}
                        snprintf(buf,sizeof(buf)-1,"%.2f",*keypad.value.valueFloat);
			EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf);
		}	
		break;
		case eKeypadIP:
			EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),keypad.value.valueStr);
		break;
                
	}
}

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/


static void InitDialog(WM_MESSAGE * pMsg)
{
	initEditShowStr();
	
	WM_HWIN hWin = pMsg->hWin;
	
	//
	//FRAMEWIN
	//
	FRAMEWIN_SetFont(hWin,&GUI_Font_micro_25);
	FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
	FRAMEWIN_SetTitleHeight(hWin,40);
	FRAMEWIN_SetText(hWin,keypad.title);

	//
	//GUI_ID_EDIT0
	//
	GUI_SetColor(UCLOR_TITLE);
	EDIT_SetFont(WM_GetDialogItem(hWin,GUI_ID_EDIT0),&GUI_Font_micro_25);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_EDIT0),GUI_TA_VCENTER|GUI_TA_CENTER);
	EDIT_EnableBlink(WM_GetDialogItem(hWin,GUI_ID_EDIT0), 300, 1);
	//
	//GUI_ID_BUTTON0
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON1
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON2
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON2),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON3
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON3),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON4
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON4),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON5
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON5),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON6
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON6),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON7
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON7),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON8
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON8),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON9
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON9),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON10
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON10),&GUI_Font_micro_25);
	//
	//GUI_ID_BUTTON11
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON11),&GUI_Font_micro_25);
        
        
	//
	//GUI_ID_BUTTON12
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON12),&GUI_Font_micro_25);
     //   BUTTON_SetText(hItem, "确定");
	//
	//GUI_ID_BUTTON13
	//
	BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON13),&GUI_Font_micro_25);
     //   BUTTON_SetText(hItem, "返回");

        
        

	EDIT_SetMaxLen(WM_GetDialogItem(hWin,GUI_ID_EDIT0),KEYPAD_STRING_MAX -1);//编辑字段的最大字符数
	

}





static int checkIP(char *p)
{
	int n[4];
	char c[4];
	if (sscanf(p, "%d%c%d%c%d%c%d%c",
             &n[0], &c[0], &n[1], &c[1],
             &n[2], &c[2], &n[3], &c[3])
      == 7)
	{
		int i;
		for(i = 0; i < 3; ++i)
		  if (c[i] != '.')
			return 0;
		for(i = 0; i < 4; ++i)
		  if (n[i] > 255 || n[i] < 0)
			return 0;
		return 1;
	} else
    return 0;
}


void endKeyPadShow(void)
{
	WM_HideWindow(ui_keyPad_hWin);
	WM_MakeModal(keypad.hWinParent);		
	WM_SelectWindow(keypad.hWinParent);
	WM_SetFocus(keypad.hWinParent);
}


static void exitCancel(WM_HWIN hWin)
{

	endKeyPadShow();
	
}

static void exitConfirm(WM_HWIN hWin)
{
	char buf[KEYPAD_STRING_MAX];
	
	
	memset(buf,0,sizeof(buf));
	EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf,KEYPAD_STRING_MAX -1 );
	switch(keypad.type)
	{
		case eKeypadString:
			memset(keypad.value.valueStr,0,KEYPAD_STRING_MAX);	//此处有坑！！！
			if((keypad.min == 0) && (keypad.max == 0))
			{
				memcpy(keypad.value.valueStr,buf,KEYPAD_STRING_MAX);
			}else
			{
				if((strlen(buf) > keypad.max) || (strlen(buf) < keypad.min))
				{
				   //   WM_HideWindow(ui_keyPad_hWin);
					uiWarning(hWin,WARN_RED_TEXT,"警告","设置超限!",0);
					return;
				}
				memcpy(keypad.value.valueStr,buf,strlen(buf));		//2018.07.02 change lenth
			}
			break;
		case eKeypadInt:
			if((keypad.min == 0) && (keypad.max == 0))
				*keypad.value.valueInt = atoi(buf);
			else
			{
				if((atoi(buf) > keypad.max) || (atoi(buf) < keypad.min))
				{
				    //  WM_HideWindow(ui_keyPad_hWin);
					uiWarning(hWin,WARN_RED_TEXT,"警告","设置超限!",0);
					return;
				}				
				//以下三步滤除字母
				*keypad.value.valueInt = atoi(buf);
			}
			break;
		  case eKeypadInt32:
			if((keypad.min == 0) && (keypad.max == 0))
				*keypad.value.valueInt32 = atoi(buf);
			else
			{
				if((atoi(buf) > keypad.max) || (atoi(buf) < keypad.min))
				{
				    //  WM_HideWindow(ui_keyPad_hWin);
					uiWarning(hWin,WARN_RED_TEXT,"警告","设置超限!",0);
					return;
				}				
				//以下三步滤除字母
				*keypad.value.valueInt32 = atoi(buf);
			}
			break;
		case eKeypadFloat:
			if((keypad.min == 0) && (keypad.max == 0))
				*keypad.value.valueFloat = atof(buf);
			else
			{
				if((atof(buf) > keypad.max) || (atof(buf) < keypad.min))
				{
				     // WM_HideWindow(ui_keyPad_hWin);
					uiWarning(hWin,WARN_RED_TEXT,"警告","设置超限!",0);
					return;
				}
				*keypad.value.valueFloat = atof(buf);
			}
			break;
		case eKeypadIP:
			
			if(strlen(buf) < 7 || strlen(buf) > 15)
			{
			      // WM_HideWindow(ui_keyPad_hWin);
				uiWarning(hWin,WARN_RED_TEXT,"警告","IP长度错误！",0);
				return;
			}
			if(checkIP(buf) != 1)
			{
			    //  WM_HideWindow(ui_keyPad_hWin);
				uiWarning(hWin,WARN_RED_TEXT,"警告","IP格式错误！",0);
				return;
			}				
			memcpy(keypad.value.valueStr,buf,KEYPAD_STRING_MAX);
			break;
	}
	
	endKeyPadShow();
		
//	EDIT_SetText(WM_GetDialogItem(keypad.hWinParent,itemId),buf);
}
	



/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
	int Id;
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN hItem;
	
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
		PaintDialog(pMsg);

		break;

		case WM_INIT_DIALOG:
		InitDialog(pMsg);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON1);
		button_user_skin_set(hItem,"1",NULL,0);

		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON2);
		button_user_skin_set(hItem,"2",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON3);
		button_user_skin_set(hItem,"3",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON4);
		button_user_skin_set(hItem,"4",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON5);
		button_user_skin_set(hItem,"5",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON6);
		button_user_skin_set(hItem,"6",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON7);
		button_user_skin_set(hItem,"7",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON8);
		button_user_skin_set(hItem,"8",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON9);
		button_user_skin_set(hItem,"9",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
		button_user_skin_set(hItem,"0",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON10);
		button_user_skin_set(hItem,".",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON11);
		button_user_skin_set(hItem,"<--",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON12);
		button_user_skin_set(hItem,"确定",NULL,0);
		
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON13);
		button_user_skin_set(hItem,"返回",NULL,0);
		
		break;
		
		case WM_NOTIFY_PARENT:
		if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
		{
			Id = WM_GetId(pMsg->hWinSrc);      
			switch (Id) 
			{
				case GUI_ID_BUTTON0:
				case GUI_ID_BUTTON1:
				case GUI_ID_BUTTON2:
				case GUI_ID_BUTTON3:
				case GUI_ID_BUTTON4:
				case GUI_ID_BUTTON5:
				case GUI_ID_BUTTON6:
				case GUI_ID_BUTTON7:
				case GUI_ID_BUTTON8:
				case GUI_ID_BUTTON9:
				switch(keypad.type)
				{
					case eKeypadInt:
					{
						//第一个开头不能为0
						char buf[KEYPAD_STRING_MAX];
						memset(buf,0,sizeof(buf));
						EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf,KEYPAD_STRING_MAX-1);
						if(strlen(buf) == 1 && buf[0] == '0' )
						{
							if(Id == GUI_ID_BUTTON0)
							{
								WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
								return;
							}else
							{
								EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0), GUI_KEY_BACKSPACE);
							}
						}
						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),Id - GUI_ID_BUTTON0 + '0');

					}
					break;
					
					case eKeypadInt32:
					{
						//第一个开头不能为0
						char buf[KEYPAD_STRING_MAX];
						memset(buf,0,sizeof(buf));
						EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf,KEYPAD_STRING_MAX-1);
						if(strlen(buf) == 1 && buf[0] == '0' )
						{
							if(Id == GUI_ID_BUTTON0)
							{
								WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
								return;
							}else
							{
								EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0), GUI_KEY_BACKSPACE);
							}
						}
						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),Id - GUI_ID_BUTTON0 + '0');

					}
					break;
					case eKeypadFloat:
					{
						//如果有小数点，小数点后面已经有2位就不能输入,小数点前面可以正常输入
						char buf[KEYPAD_STRING_MAX];
						int i,len;
						memset(buf,0,sizeof(buf));
						EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf,KEYPAD_STRING_MAX-1);
						len = strlen(buf);
						for(i = 0 ; i < len; i++)
						{
							if(buf[i] == '.')
							{
								if((EDIT_GetCursorCharPos(WM_GetDialogItem(hWin,GUI_ID_EDIT0)) > i)&&((len - i) >= T_decimals))
								{
									WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
									return;
								}
							}
						}
						//若干第一个字符为0，第二字符不能为数字
						if((EDIT_GetCursorCharPos(WM_GetDialogItem(hWin,GUI_ID_EDIT0)) == 1) &&  (buf[0] == '0'))
						{
							WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
							return;
						}

						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),Id - GUI_ID_BUTTON0 + '0');
					}
					break;
					default:
						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),Id - GUI_ID_BUTTON0 + '0');
					break;
				}
				WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
				break;
				case GUI_ID_BUTTON10:
				switch(keypad.type)
				{
					case eKeypadInt:
					break;
					case eKeypadInt32:
					break;
					case eKeypadFloat:
					{
						//避免多个小数点，如果小数点在第一个位置，前面加入一个0
						char buf[KEYPAD_STRING_MAX];
						int i;
						memset(buf,0,sizeof(buf));
						EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf,KEYPAD_STRING_MAX-1);
						for(i = 0 ; i < KEYPAD_STRING_MAX; i++)
						{
							if(buf[i] == '.')
							{
								WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
								return;
							}
						}
						//如果小数点在第一个位置，前面加入一个0
						if(EDIT_GetCursorCharPos(WM_GetDialogItem(hWin,GUI_ID_EDIT0)) == 0)
						{
							char buf2[8];
							memset(buf2,0,sizeof(buf2));
							sprintf(buf2,"0.%c%c",buf[0],buf[1]);
							EDIT_SetText(WM_GetDialogItem(hWin,GUI_ID_EDIT0),buf2);
							break;
						}
						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'.');
					}
					break;
					default:
						EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0),'.');
					break;
				}
				WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
				break;
				case GUI_ID_BUTTON11:
					EDIT_AddKey(WM_GetDialogItem(hWin,GUI_ID_EDIT0), GUI_KEY_BACKSPACE);
					WM_SetFocus(WM_GetDialogItem(hWin,GUI_ID_EDIT0));
				break;
				case GUI_ID_BUTTON12:
					exitConfirm(hWin);
	                    //    WM_HideWindow(ui_keyPad_hWin);
				break;
				case GUI_ID_BUTTON13:
					exitCancel(hWin);
				//	WM_HideWindow(ui_keyPad_hWin);
				break;
			}
		}
		break;
		default:
			WM_DefaultProc(pMsg);
	}
}


int uiKeypad(WM_HWIN hWinParent,int id)
{
	keypad.hWinParent = hWinParent;
	keypad.itemId = id;
		
	printf("Now create ui Keypad ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes());
	int status = 0;
	WM_HWIN hWin;
	hWin = ui_keyPad_hWin;
	
	printf("create ui Keypad ==> %d  mem : %d\r\n",hWin,GUI_ALLOC_GetNumFreeBytes());
	
	WM_ShowWindow(hWin);
	WM_BringToTop(hWin);
	
	printf("ui Keypad to TOP OK ==> %d  mem : %d\r\n",hWin,GUI_ALLOC_GetNumFreeBytes());

	WM_MakeModal(hWin);		
	
	printf("ui Keypad WM_MakeModal OK ==> %d  mem : %d\r\n",hWin,GUI_ALLOC_GetNumFreeBytes());
	
	WM_SelectWindow(hWin);
	WM_SetFocus(hWin);
	
	FRAMEWIN_SetText(hWin,keypad.title);
	printf("set ui Keypad OK ==> %d  mem : %d\r\n",hWin,GUI_ALLOC_GetNumFreeBytes());

	initEditShowStr();	
	
	return status;
}

void keypadEditString(WM_HWIN hWin,int id)
{
    	printf(" keypadEditString ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes()); 
	
	char buf[KEYPAD_TITLE_MAX];
	memset(buf,0,sizeof(buf));
	
	keypad.type = eKeypadString;	
		
	EDIT_GetText(WM_GetDialogItem(hWin,id),buf,KEYPAD_STRING_MAX);
	memcpy(keypad.value.valueStr,buf,KEYPAD_STRING_MAX);
	uiKeypad(hWin,id);

	EDIT_SetText(WM_GetDialogItem(hWin,id),keypad.value.valueStr);

}

void keypadEditFloat(WM_HWIN hWin,int id,int min,int max,float *setValue)
{
	keypad.type = eKeypadFloat;
	keypad.min = min;
	keypad.max = max;
	keypad.value.valueFloat = setValue;
	
	uiKeypad(hWin,id);
}

void keypadEditInt(WM_HWIN hWin,int id,int min,int max,unsigned short int *setValue)
{
  	printf(" keypadEditInt ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes()); 
	keypad.type = eKeypadInt;
	keypad.min = min;
	keypad.max = max;
        
	keypad.value.valueInt = setValue;

	printf("pre to real create keyPad ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes());	
	uiKeypad(hWin,id);

}


void keypadEditInt32(WM_HWIN hWin,int id,int min,int max,unsigned int *setValue)
{
  	printf(" keypadEditInt ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes()); 
	keypad.type = eKeypadInt32;
	keypad.min = min;
	keypad.max = max;
        
     //   setValue_data = setValue;
	keypad.value.valueInt32 = setValue;

	printf("pre to real create keyPad ==> mem : %d\r\n",GUI_ALLOC_GetNumFreeBytes());	
	uiKeypad(hWin,id);

}

void keypadEditIP(WM_HWIN hWin,int id)
{
	char buf[KEYPAD_TITLE_MAX];
	memset(buf,0,sizeof(buf));
	keypad.type = eKeypadIP;
	EDIT_GetText(WM_GetDialogItem(hWin,id),buf,KEYPAD_STRING_MAX);
	memcpy(keypad.value.valueStr,buf,KEYPAD_STRING_MAX);
	uiKeypad(hWin,id);

	EDIT_SetText(WM_GetDialogItem(hWin,id),keypad.value.valueStr);
}


void CreateUiKeypad(void)
{
	ui_keyPad_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, WM_HBKWIN, 0, 0);
	WM_HideWindow(ui_keyPad_hWin);

}











