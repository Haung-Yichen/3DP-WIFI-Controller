/**
 * @file    esp32.h
 * @author  HAUNG YI CHEN
 * @date    2025/06/24
 * @brief   該模組負責處理解析ESP32傳來的資料
 *		    並實作所有命令的回調函數，回調函數
 *		    再去呼叫真正執行該邏輯的函數。
 */

#ifndef _ESP32_H_
#define _ESP32_H_

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "usart.h"
#include "cmdList.h"
#include "cmdHandler.h"
#include "printerController.h"

#define ESP32_USART_PORT   huart2
#define ESP32_READY        "ok"
#define ESP32_DISCONNECTED "wifi disconnected"
#define CRXBUF_SIZE (20 + 4)

typedef enum {
	ESP32_INIT = 0,
	ESP32_IDLE,
	ESP32_BUSY
} ESP32_STATE_TypeDef;

extern SemaphoreHandle_t traOverSemaphore; //傳檔完成信號量
extern SemaphoreHandle_t staPriSemaphore;  //開始列印信號量
extern SemaphoreHandle_t rxSemaphore;      //UART啟動解析信號量

// 內部狀態與緩衝區
extern volatile uint16_t CRxLen;
extern uint8_t *ResBuf; //回調函數傳回結果緩衝區
extern uint8_t CRxBuf[CRXBUF_SIZE]; //命令接收緩衝區
extern uint8_t FRxBuf[100]; //gcode接收緩衝區 大小依傳輸方式為定
static ESP32_STATE_TypeDef currentState = ESP32_INIT;

/**
 * @brief 初始化 ESP32 模組，註冊回調函數，並等待直到esp32回報初始化完成
 */
void ESP32_Init(void);

/**
 * @brief 註冊回調函數
 */
void ESP32_RegCallback(void);

/**
 * @brief 傳回目前 ESP32 狀態
 */
ESP32_STATE_TypeDef ESP32_GetState(void);

/**
 * @brief 設定 ESP32 狀態
 */
void ESP32_SetState(ESP32_STATE_TypeDef state);

/**
 * @brief 解析UART資料，DMA中斷觸發
 */
void ESP32_RxHandler_Task(void *argument);

/**
 * @brief 回傳查詢的資料給ESP32
 */
void ESP32_ReturnClbkRes(void);


/************************************************
 *                 定義回調函數                  *
 ************************************************/

/**
 * @brief 命令 : 準備接收dcode
 */
void StartTransmissionCmdHandler(const char *args, void *res);

/**
 * @brief 命令：傳輸結束（command 觸發器）
 */
void TransmissionOverCmdHandler(const char *args, void *res);

/**
 * @brief 開始列印命令的處理函式
 */
void StartToPrintCmdHandler(const char *args, void *res);

/**
 * @brief 暫停列印命令的處理函式
 */
void PausePrintingCmdHandler(const char *args, void *res);

/**
 * @brief 停止列印命令的處理函式
 */
void StopPrintingCmdHandler(const char *args, void *res);

/**
 * @brief 回到原點命令的處理函式
 */
void GoHomeCmdHandler(const char *args, void *res);

/**
 * @brief 請求剩餘列印時間命令的處理函式
 */
void GetRemainingTimeCmdHandler(const char *args, void *res);

/**
 * @brief 請求列印進度命令的處理函式
 */
void GetProgressCmdHandler(const char *args, void *res);

/**
 * @brief 請求噴嘴溫度命令的處理函式
 */
void GetNozzleTempCmdHandler(const char *args, void *res);

/**
 * @brief 請求熱床溫度命令的處理函式
 */
void GetBedTempCmdHandler(const char *args, void *res);

/**
 * @brief 設置噴嘴溫度命令的處理函式
 */
void SetNozzleTempCmdHandler(const char *args, void *res);

/**
 * @brief 設置熱床溫度命令的處理函式
 */
void SetBedTempCmdHandler(const char *args, void *res);

/**
 * @brief 請求耗材重量命令的處理函式
 */
void GetFilamentWeightCmdHandler(const char *args, void *res);

/**
 * @brief 緊急停止命令的處理函式
 */
void EmergencyStopCmdHandler(const char *args, void *res);


#endif /* _ESP32_H_ */
