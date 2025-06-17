#ifndef _ESP32_H_
#define _ESP32_H_

#include "usart.h"
#include "cmdList.h"
#include <stdint.h>

#define ESP32_READY        "ok"
#define ESP32_DISCONNECTED "wifi disconnected"

typedef enum {
    ESP32_INIT = 0,
    ESP32_IDLE,
    ESP32_BUSY
} ESP32_STATE_TypeDef;

// 內部狀態與緩衝區
static uint8_t rx_Buf[15];
static ESP32_STATE_TypeDef currentState = ESP32_INIT;

/**
 * @brief 初始化 ESP32 模組（如建立初始狀態）
 */
void ESP32_Init(void);

/**
 * @brief 載入 G-code
 */
void ESP32_LoadGcode(void);

/**
 * @brief 傳回目前 ESP32 狀態
 */
ESP32_STATE_TypeDef ESP32_GetState(void);

/**
 * @brief 設定 ESP32 狀態
 */
void ESP32_SetState(ESP32_STATE_TypeDef state);

/**
 * @brief 處理 UART 接收資料（每次中斷進入）
 */
void ESP32_RxHandler(void);

/**
 * @brief 上傳資料到 3D 印表機
 */
void ESP32_UploadToPrinter(void);

/**
 * @brief 命令：開始傳輸（command 觸發器）
 */
void ESP32_StartTransmissionCmdHandler(const char* args);

/**
 * @brief 命令：傳輸結束（command 觸發器）
 */
void ESP32_TransmissionOverCmdHandler(const char* args);

#endif /* _ESP32_H_ */
