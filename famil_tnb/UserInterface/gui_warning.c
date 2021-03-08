#include "gui_common.h"
#include "main.h"
#include "stdlib.h"
#include "string.h"
#include "gui_common.h"

static int warnType;
static char warnStr[20],warnTitle[20];


//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/

WM_HWIN ui_keyWarn_hWin = NULL;
WM_HWIN ui_keyWarnParent_hWin = NULL;
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { FRAMEWIN_CreateIndirect,  "提示",   0,	40,	50,	240,	140,0,0},
    { TEXT_CreateIndirect,       NULL,    GUI_ID_TEXT0,     7,	6,	220,	35, 0,0},
    { BUTTON_CreateIndirect,    "确认",   GUI_ID_BUTTON0,   80,	56,	75, 	35, 0,0}
};



/*****************************************************************
**      FunctionName:void PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_PAINT
*****************************************************************/

static void PaintDialog(WM_MESSAGE * pMsg)
{
    //WM_HWIN hWin = pMsg->hWin;

}



/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**                                                      
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

static void InitDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
//	WM_HWIN hItem;
	//
	//FRAMEWIN
	//
	FRAMEWIN_SetFont(hWin,&GUI_Font_micro_25);
	FRAMEWIN_SetTextAlign(hWin,GUI_TA_VCENTER|GUI_TA_CENTER);
	FRAMEWIN_SetTitleHeight(hWin,30);
	FRAMEWIN_SetTextColor(hWin,UCLOR_WHITE);
	//
	//GUI_ID_TEXT0
	//
	TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_Font_micro_25);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin,GUI_ID_TEXT0),GUI_TA_VCENTER|GUI_TA_CENTER);


}

static void drawWarn(WM_HWIN hWin)
{
	FRAMEWIN_SetText(hWin,warnTitle);
	switch(warnType&WARN_COLOUR_MASK)
	{
		case WARN_BLACK_TEXT:
			TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0),GUI_BLACK);
			TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0),warnStr);
			break;
		case WARN_RED_TEXT:
			TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0),UCLOR_BLACK);
			TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT0),warnStr);
			break;
		default:
			break;
	}
}


void endWarningShow(void)
{    
        xSemaphoreGive(ButtonArmSemaphore);
	WM_HideWindow(ui_keyWarn_hWin);
	WM_MakeModal(ui_keyWarnParent_hWin);		
	WM_SelectWindow(ui_keyWarnParent_hWin);
	WM_SetFocus(ui_keyWarnParent_hWin);

}
/*********************************************************************
*
*       Dialog callback routine xSemaphoreGive(ClockArmSemaphore);
*/
static void _cbCallback(WM_MESSAGE * pMsg) 
{
    int Id;
    WM_HWIN hItem;
//    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
		drawWarn(ui_keyWarn_hWin);	
            break;
        case WM_INIT_DIALOG:
	    
            InitDialog(pMsg);	
		hItem= WM_GetDialogItem(pMsg->hWin, GUI_ID_BUTTON0);
		button_user_skin_set(hItem,"确定",NULL,0);
            break;

        case WM_NOTIFY_PARENT:
           if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
		{
			Id = WM_GetId(pMsg->hWinSrc);      
			switch (Id) 
			{
				case GUI_ID_BUTTON0:
					endWarningShow();
                                      
					break;
			}
		}
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}
void CreateUiWarning(void)
{
	ui_keyWarn_hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, WM_HBKWIN, 0, 0);
	WM_HideWindow(ui_keyWarn_hWin);
}
int uiWarning(WM_HWIN hWinParent,int type,char *title,char *str,int timeOut )
{
	ui_keyWarnParent_hWin = hWinParent;
	int status = 0;
	WM_HWIN hWin;
	
	warnType = type;
	memset(warnTitle,0,20);
	strcpy(warnTitle,title);
	memset(warnStr,0,20);
	strcpy(warnStr,str);
	
	CreateUiWarning();
	hWin = ui_keyWarn_hWin;
	
	
	WM_ShowWindow(hWin);
	WM_BringToTop(hWin);
	WM_MakeModal(hWin);		
	WM_SelectWindow(hWin);
	WM_SetFocus(hWin);

	return status;
}


