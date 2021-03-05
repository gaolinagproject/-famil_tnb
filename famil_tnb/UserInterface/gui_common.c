#include "gui_common.h"
#include "stdio.h"
#include "string.h"


data_set_t data_set_lcd;


static int _cbButtonUser(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{ //--------------£¨1£©
	WM_HWIN hWin;
	hWin = pDrawItemInfo->hWin;
	int lenth;

	user_data_t udata;
	lenth = BUTTON_GetUserData(hWin,&udata,sizeof(user_data_t));
//	printf("GetUserData lenth : %d\r\n",lenth);
//	printf("Judge : %d\r\n",udata.judge);
	
	
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND: //--------------£¨2£©
		 
        if(udata.data == NULL){
          if (BUTTON_IsPressed(pDrawItemInfo->hWin)) { //--------------£¨3£©

            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GREEN, GUI_CYAN);
          } else {
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GREEN, GUI_CYAN);
          }
        }

        else if((*(unsigned short *)udata.data) != udata.judge){	
          if (BUTTON_IsPressed(pDrawItemInfo->hWin)) { //--------------£¨3£©
            GUI_SetColor(UCLOR_BTN_PRS);
            //GUI_AA_FillRoundedRect(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, 6);	
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GREEN, GUI_CYAN);
          } else {
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GREEN, GUI_CYAN);
          }
        }

        else{	//×´Ì¬Æ¥Åä£¬»ÒÉ«ÏÔÊ¾

          GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GRAY, GUI_GRAY);
        }

	break;
	default:

		return BUTTON_DrawSkinFlex(pDrawItemInfo); //--------------£¨4£©
	}
	return 0;
}
	
	

			
void button_user_skin_set(BUTTON_Handle hItem,char *str,void *dataSrc,unsigned short judge)
{   	
	int lenth;
	user_data_t udata;
	
	udata.data = dataSrc;
	udata.judge = judge;
	lenth = BUTTON_SetUserData(hItem,&udata,sizeof(user_data_t));
		
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_RED);
	BUTTON_SetFont(hItem, &GUI_Font_micro_25);
	BUTTON_SetText(hItem, str);
		
	BUTTON_SetSkin(hItem, _cbButtonUser);
}

static int _cbClockUser(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{ //--------------£¨1£©
	WM_HWIN hWin;
	hWin = pDrawItemInfo->hWin;
	int lenth;

	user_data_t udata;
	lenth = BUTTON_GetUserData(hWin,&udata,sizeof(user_data_t));

	
	
	switch (pDrawItemInfo->Cmd) {
	case WIDGET_ITEM_DRAW_BACKGROUND: //--------------£¨2£©
		 
        if(udata.data == NULL){
          if (BUTTON_IsPressed(pDrawItemInfo->hWin)) { //--------------£¨3£©

            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_LIGHTYELLOW, GUI_LIGHTRED);
          } else {
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_LIGHTYELLOW, GUI_LIGHTRED);
          }
        }

        else if((*(unsigned short *)udata.data) != udata.judge){	
          if (BUTTON_IsPressed(pDrawItemInfo->hWin)) { //--------------£¨3£©
            GUI_SetColor(UCLOR_BTN_PRS);
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_LIGHTYELLOW, GUI_LIGHTRED);
          } else {
            GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_LIGHTYELLOW, GUI_LIGHTRED);
          }
        }

        else{	//×´Ì¬Æ¥Åä£¬»ÒÉ«ÏÔÊ¾

          GUI_DrawGradientV(pDrawItemInfo->x0, pDrawItemInfo->y0, pDrawItemInfo->x1, pDrawItemInfo->y1, GUI_GRAY, GUI_GRAY);
        }

	break;
	default:

		return BUTTON_DrawSkinFlex(pDrawItemInfo); //--------------£¨4£©
	}
	return 0;
}

void button_user_clock_set(BUTTON_Handle hItem,char *str,void *dataSrc,unsigned short judge)
{   	
	int lenth;
	user_data_t udata;
	
	udata.data = dataSrc;
	udata.judge = judge;
	lenth = BUTTON_SetUserData(hItem,&udata,sizeof(user_data_t));
		
	BUTTON_SetTextColor(hItem,BUTTON_CI_UNPRESSED,GUI_BLACK);
	BUTTON_SetTextColor(hItem,BUTTON_CI_PRESSED,GUI_RED);
	BUTTON_SetFont(hItem, &GUI_Font_micro_25);
	BUTTON_SetText(hItem, str);
		
	BUTTON_SetSkin(hItem, _cbClockUser);
}


