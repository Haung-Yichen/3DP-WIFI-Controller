#ifndef __BSP_XPT2046_LCD_H
#define	__BSP_XPT2046_LCD_H

#include "stm32f1xx_hal.h"

/******************************* XPT2046 觸控中斷腳定義 (若有使用中斷) ***************************/
#define XPT2046_PENIRQ_GPIO_PORT      GPIOE
#define XPT2046_PENIRQ_GPIO_PIN       GPIO_PIN_4

// 觸控有效電平
#define XPT2046_PENIRQ_ActiveLevel    GPIO_PIN_RESET
#define XPT2046_PENIRQ_Read()         HAL_GPIO_ReadPin(XPT2046_PENIRQ_GPIO_PORT, XPT2046_PENIRQ_GPIO_PIN)


/******************************* XPT2046 軟體模擬 SPI 腳位定義 ***************************/
#define XPT2046_SPI_CS_PORT           GPIOD
#define XPT2046_SPI_CS_PIN            GPIO_PIN_13

#define XPT2046_SPI_CLK_PORT          GPIOE
#define XPT2046_SPI_CLK_PIN           GPIO_PIN_0

#define XPT2046_SPI_MOSI_PORT         GPIOE
#define XPT2046_SPI_MOSI_PIN          GPIO_PIN_2

#define XPT2046_SPI_MISO_PORT         GPIOE
#define XPT2046_SPI_MISO_PIN          GPIO_PIN_3

#define XPT2046_CS_ENABLE()           HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_SET)
#define XPT2046_CS_DISABLE()          HAL_GPIO_WritePin(XPT2046_SPI_CS_PORT, XPT2046_SPI_CS_PIN, GPIO_PIN_RESET)

#define XPT2046_CLK_HIGH()            HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_SET)
#define XPT2046_CLK_LOW()             HAL_GPIO_WritePin(XPT2046_SPI_CLK_PORT, XPT2046_SPI_CLK_PIN, GPIO_PIN_RESET)

#define XPT2046_MOSI_1()              HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_SET)
#define XPT2046_MOSI_0()              HAL_GPIO_WritePin(XPT2046_SPI_MOSI_PORT, XPT2046_SPI_MOSI_PIN, GPIO_PIN_RESET)

#define XPT2046_MISO()                HAL_GPIO_ReadPin(XPT2046_SPI_MISO_PORT, XPT2046_SPI_MISO_PIN)


/******************************* 除錯輸出宏定義 ***************************/
#define XPT2046_DEBUG_ON              0

#define XPT2046_INFO(fmt, args...)    printf("<<-XPT2046-INFO->> " fmt "\n", ##args)
#define XPT2046_ERROR(fmt, args...)   printf("<<-XPT2046-ERROR->> " fmt "\n", ##args)
#define XPT2046_DEBUG(fmt, args...)   do{\
                                          if(XPT2046_DEBUG_ON)\
                                          printf("<<-XPT2046-DEBUG->> [%d]" fmt "\n", __LINE__, ##args);\
                                      }while(0)


/******************************* XPT2046 命令定義 ***************************/
#define macXPT2046_CHANNEL_X          0x90    // Y通道
#define macXPT2046_CHANNEL_Y          0xD0    // X通道


/******************************** XPT2046 函數聲明 **********************************/
void XPT2046_Init(void);
uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel);

#endif /* __BSP_XPT2046_LCD_H */
