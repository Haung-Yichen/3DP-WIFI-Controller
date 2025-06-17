#include "esp32.h"
#include <string.h>
#include <stdbool.h>

void ESP32_Init(void) {
    currentState = ESP32_INIT;
    memset(rx_Buf, 0, sizeof(rx_Buf));
}

void ESP32_LoadGcode(void) {
    // 實作 G-code 載入流程（根據你的系統需求）
}

ESP32_STATE_TypeDef ESP32_GetState(void) {
    return currentState;
}

void ESP32_SetState(ESP32_STATE_TypeDef state) {
    currentState = state;
}

void ESP32_RxHandler(void) {
    // 範例：處理收到的資料
    if (strstr((char*)rx_Buf, ESP32_READY)) {
        ESP32_SetState(ESP32_IDLE);
    } else if (strstr((char*)rx_Buf, ESP32_DISCONNECTED)) {
        ESP32_SetState(ESP32_INIT);
    }

    // 重啟接收（必要）
    HAL_UART_Receive_IT(&huart1, rx_Buf, sizeof(rx_Buf));
}

void ESP32_UploadToPrinter(void) {
    ESP32_SetState(ESP32_BUSY);
    // 傳送資料邏輯
}

void ESP32_StartTransmissionCmdHandler(const char* args) {
    // args 可用來解析指令參數
    ESP32_UploadToPrinter();
}

void ESP32_TransmissionOverCmdHandler(const char* args) {
    (void)args;
    ESP32_SetState(ESP32_IDLE);
}

// 串口中斷回調函式（放在中斷源文件中，例如 usart.c）
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart == &huart1) {
        ESP32_RxHandler();
    }
}
