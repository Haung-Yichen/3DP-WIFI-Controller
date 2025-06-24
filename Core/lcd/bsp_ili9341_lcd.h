#ifndef __BSP_ILI9341_LCD_H
#define __BSP_ILI9341_LCD_H

#include "stm32f1xx_hal.h"

/***************************************************************************************
ILI9341 LCD 的 FSMC 配置
使用 FSMC Bank1 (NOR/PSRAM)
FSMC_A16 用於 DC (資料/命令) 控制
****************************************************************************************/

/******************************* ILI9341 顯示屏的 FSMC 參數定義 ***************************/
// 用於 LCD 命令操作的 FSMC 地址
#define FSMC_Addr_ILI9341_CMD         ((uint32_t)0x60000000)

// 用於 LCD 資料操作的 FSMC 地址
#define FSMC_Addr_ILI9341_DATA        ((uint32_t)0x60020000)

#define FSMC_Bank1_NORSRAMx           FSMC_NORSRAM_BANK1

// 方便 emWin 使用
#define macFSMC_ILI9341_REG           *(__IO uint16_t *)(0x60000000)
#define macFSMC_ILI9341_RAM           *(__IO uint16_t *)(0x60020000)

/******************************* ILI9341 顯示屏 8080 通訊引腳定義 ***************************/
/******控制信號線******/
// 片選
#define ILI9341_CS_PORT               GPIOD
#define ILI9341_CS_PIN                GPIO_PIN_7

// DC 引腳，使用 FSMC 的地址信號控制
#define ILI9341_DC_PORT               GPIOD
#define ILI9341_DC_PIN                GPIO_PIN_11

// 寫使能
#define ILI9341_WR_PORT               GPIOD
#define ILI9341_WR_PIN                GPIO_PIN_5

// 讀使能
#define ILI9341_RD_PORT               GPIOD
#define ILI9341_RD_PIN                GPIO_PIN_4

// 復位引腳
#define ILI9341_RST_PORT              GPIOE
#define ILI9341_RST_PIN               GPIO_PIN_1

// 背光引腳
#define ILI9341_BK_PORT               GPIOD
#define ILI9341_BK_PIN                GPIO_PIN_12

/********資料信號線***************/
#define ILI9341_D0_PORT               GPIOD
#define ILI9341_D0_PIN                GPIO_PIN_14

#define ILI9341_D1_PORT               GPIOD
#define ILI9341_D1_PIN                GPIO_PIN_15

#define ILI9341_D2_PORT               GPIOD
#define ILI9341_D2_PIN                GPIO_PIN_0

#define ILI9341_D3_PORT               GPIOD
#define ILI9341_D3_PIN                GPIO_PIN_1

#define ILI9341_D4_PORT               GPIOE
#define ILI9341_D4_PIN                GPIO_PIN_7

#define ILI9341_D5_PORT               GPIOE
#define ILI9341_D5_PIN                GPIO_PIN_8

#define ILI9341_D6_PORT               GPIOE
#define ILI9341_D6_PIN                GPIO_PIN_9

#define ILI9341_D7_PORT               GPIOE
#define ILI9341_D7_PIN                GPIO_PIN_10

#define ILI9341_D8_PORT               GPIOE
#define ILI9341_D8_PIN                GPIO_PIN_11

#define ILI9341_D9_PORT               GPIOE
#define ILI9341_D9_PIN                GPIO_PIN_12

#define ILI9341_D10_PORT              GPIOE
#define ILI9341_D10_PIN               GPIO_PIN_13

#define ILI9341_D11_PORT              GPIOE
#define ILI9341_D11_PIN               GPIO_PIN_14

#define ILI9341_D12_PORT              GPIOE
#define ILI9341_D12_PIN               GPIO_PIN_15

#define ILI9341_D13_PORT              GPIOD
#define ILI9341_D13_PIN               GPIO_PIN_8

#define ILI9341_D14_PORT              GPIOD
#define ILI9341_D14_PIN               GPIO_PIN_9

#define ILI9341_D15_PORT              GPIOD
#define ILI9341_D15_PIN               GPIO_PIN_10

/*************************************** 除錯用宏 ******************************************/
#define DEBUG_DELAY()

/***************************** ILI934 顯示區域的起始座標和總行列數 ***************************/
#define ILI9341_DispWindow_X_Star     0       // 起始點的 X 座標
#define ILI9341_DispWindow_Y_Star     0       // 起始點的 Y 座標

#define ILI9341_LESS_PIXEL            240     // 液晶屏較短方向的像素寬度
#define ILI9341_MORE_PIXEL            320     // 液晶屏較長方向的像素寬度

// 根據液晶掃描方向而變化的 XY 像素寬度
// 調用 ILI9341_GramScan 函數設置方向時會自動更改
extern uint16_t LCD_X_LENGTH, LCD_Y_LENGTH;
// 液晶屏掃描模式 (0-7)
extern uint8_t LCD_SCAN_MODE;

/***************** ILI934 顯示屏全屏默認（掃描方向為1時）最大寬度和最大高度 *********************/
#define macILI9341_Default_Max_Width   240     // 默認 X 最大寬度
#define macILI9341_Default_Max_Heigth  320     // 默認 Y 最大高度

/********************************** 液晶背光控制宏 ***************************************/
#define LCD_BK_EN()                   HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_RESET)  // 開啟背光
#define LCD_BK_DIS()                  HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_SET)    // 關閉背光

/******************************* 定義 ILI934 常用命令 ********************************/
#define CMD_SetCoordinateX            0x2A     // 設置 X 座標
#define CMD_SetCoordinateY            0x2B     // 設置 Y 座標
#define CMD_SetPixel                 0x2C     // 填充像素

/********************************** 聲明 ILI934 函數 ***************************************/
void ILI9341_Write_Cmd(uint16_t usCmd);
void ILI9341_Write_Data(uint16_t usData);
void ILI9341_Init(void);
void ILI9341_Rst(void);
void ILI9341_BackLed_Control(FunctionalState enumState);
void ILI9341_GramScan(uint8_t ucOtion);
void ILI9341_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);
void ILI9341_Clear(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);

#endif /* __BSP_ILI9341_LCD_H */