/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.44 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license SLA0044,
  * the "License"; You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *                      http://www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "ili93xx.h"
/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
//与触摸屏有关定义，根据实际情况填写
#define TOUCH_AD_TOP		3600  	//按下触摸屏的顶部，写下 Y 轴模拟输入值。
#define TOUCH_AD_BOTTOM		450 	//按下触摸屏的底部，写下 Y 轴模拟输入值。
#define TOUCH_AD_LEFT 		3520		//按下触摸屏的左侧，写下 X 轴模拟输入值。
#define TOUCH_AD_RIGHT		310	//按下触摸屏的右侧，写下 X 轴模拟输入值。
//
// Physical display size
//
#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size
#define Height  240 // To be adapted to x-screen size
#define Width  320 // To be adapted to y-screen size
/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/

static void LcdWriteReg(U16 Data) {
  // ... TBD by user
	LCD->LCD_REG = Data;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
  // ... TBD by user
	LCD->LCD_RAM = Data;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
		LCD->LCD_RAM = *pData;
		pData++;
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
		*pData = LCD->LCD_RAM;
		pData++;
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};
  

  //
  // Set display driver and color conversionGUICC_M565
  // GUICC_565
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
//	LCD_SetSizeEx (0, lcddev.width , lcddev.height);
//	LCD_SetVSizeEx(0, lcddev.width, lcddev.height);
  //
  // Orientation
  //
	Config.NumDummyReads = 2; 
  Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y;
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //
  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
  
//			GUI_TOUCH_Calibrate(GUI_COORD_X,0,240,3518,3648); 	
//			GUI_TOUCH_Calibrate(GUI_COORD_Y,0,320,305,428);
                        
  GUI_TOUCH_Calibrate(GUI_COORD_X,0,Width,TOUCH_AD_LEFT,TOUCH_AD_RIGHT);
  GUI_TOUCH_Calibrate(GUI_COORD_Y,0,Height,TOUCH_AD_TOP,TOUCH_AD_BOTTOM);
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
//		ILI93XX_LCD_Init();
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}


/*************************** End of file ****************************/

