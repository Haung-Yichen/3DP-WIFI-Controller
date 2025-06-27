/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
#include "bsp_xpt2046_lcd.h"
#include "bsp_ili9341_lcd.h"

//modify by fire 此文件的函數為將觸摸屏與emWin對接的函數

// 全局變量用於儲存觸摸狀態
static int LastTouchX = 0;
static int LastTouchY = 0;
static uint8_t TouchDetected = 0;

/*********************************************************************
*
*       GUI_TOUCH_X_ActivateX
*
* Purpose:
*   Called from GUI, if touch support is enabled.
*   Switches on voltage on X-axis,
*   prepares measurement of Y-axis.
*   Voltage on Y-axis is switched off.
*/
void GUI_TOUCH_X_ActivateX(void)
{
    // XPT2046使用差分測量方式，不需要單獨激活X軸
    // 這個函數在XPT2046中可以留空
}

/*********************************************************************
*
*       GUI_TOUCH_X_ActivateY
*
* Purpose:
*   Called from GUI, if touch support is enabled.
*   Switches on voltage on Y-axis,
*   prepares measurement of X-axis.
*   Voltage on X-axis is switched off.
*/
void GUI_TOUCH_X_ActivateY(void)
{
    // XPT2046使用差分測量方式，不需要單獨激活Y軸
    // 這個函數在XPT2046中可以留空
}

/*********************************************************************
*
*       GUI_TOUCH_X_MeasureX
*
* Purpose:
*   Return the X-value of the currently selected position.
*   Return value is either the X-value in pixels, or -1 if not pressed.
*
* Notes:
*   This routine is called periodically by the GUI.
*   It is therefore recommended to:
*   a) Return 0 if no measurement is possible
*   b) Make sure that measurements are consistent
*/
int GUI_TOUCH_X_MeasureX(void)
{
    strType_XPT2046_Coordinate touchCoordinate;

    // 檢測觸摸狀態
    if(XPT2046_TouchDetect() == TOUCH_PRESSED && TouchDetected)
    {
        // 獲取觸摸坐標
        if(XPT2046_Get_TouchedPoint(&touchCoordinate, strXPT2046_TouchPara))
        {
            // 檢查坐標是否在有效範圍內
            if(touchCoordinate.y >= 0 && touchCoordinate.y < LCD_Y_LENGTH)
            {
                LastTouchY = touchCoordinate.y;
                return LastTouchY;
            }
        }
    }
    else if(!TouchDetected)
    {
        LastTouchY = 0;
    }

    return LastTouchY;  // 返回上次的Y坐標或-1
}

/*********************************************************************
*
*       GUI_TOUCH_X_MeasureY
*
* Purpose:
*   Return the Y-value of the currently selected position.
*   Return value is either the Y-value in pixels, or -1 if not pressed.
*
* Notes:
*   This routine is called periodically by the GUI.
*   It is therefore recommended to:
*   a) Return 0 if no measurement is possible
*   b) Make sure that measurements are consistent
*/
int GUI_TOUCH_X_MeasureY(void)
{
    strType_XPT2046_Coordinate touchCoordinate;

    // 檢測觸摸狀態
    if(XPT2046_TouchDetect() == TOUCH_PRESSED)
    {
        // 獲取觸摸坐標
        if(XPT2046_Get_TouchedPoint(&touchCoordinate, strXPT2046_TouchPara))
        {
            // 檢查坐標是否在有效範圍內
            if(touchCoordinate.x >= 0 && touchCoordinate.x < LCD_X_LENGTH)
            {
                LastTouchX = touchCoordinate.x;
                TouchDetected = 1;
                return LastTouchX;
            }
        }
    }
    else
    {
        TouchDetected = 0;
        LastTouchX = 0;
    }

    return -1;  // 無觸摸或無效觸摸
}