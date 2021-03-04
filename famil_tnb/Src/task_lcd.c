#include "main.h"

sensor_data_t sensor_data_lcd;

extern GUI_BITMAP bmb_image;  


void draw_bitmap(void)
{
  
	GUI_DrawBitmap(&bmb_image,100,100);  
}

void zoom_bitmap(int Xmag,int Ymag)
{
//	GUI_SetBkColor(GUI_BLUE);
//	GUI_Clear();
	GUI_DrawBitmapEx(&bmb_image,0,0,0,0,Xmag,Ymag); 
}

void Task_Lcd(void *pvParameters)
{
  ILI93XX_LCD_Init();
  LCD_Clear(BLACK);
  TP_Init();

  GUI_Init();
  GUI_Clear();
  GUI_CURSOR_Show();
  WM_MULTIBUF_Enable(1);

  GUI_SetBkColor(UCLOR_TITLE);
  GUI_UC_SetEncodeUTF8();
  ui_main();
  while(1)
  {
    GUI_Delay(100);
  }
}