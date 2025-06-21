/*********************************************************************
 * @file   printer_controller.h
 * @brief  印表機控制器
 * 為本項目核心模組，負責與印表機通訊，接收來自電腦端與esp32的命令，
 * 以及emWin GUI。每隔一段時間會主動獲取傳感器狀態，並提供接口讓外部訪問。
 *
 * @author Lin, YiChen
 * @date   2024.12.08
 *********************************************************************/

#ifndef _PRINTER_CONTROLLER_H_
#define _PRINTER_CONTROLLER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief 開始列印命令的處理函式
 */
void StartToPrintCmdHandler(const char* args);

/**
 * @brief 暫停列印命令的處理函式
 */
void PausePrintingCmdHandler(const char* args);

/**
 * @brief 停止列印命令的處理函式
 */
void StopPrintingCmdHandler(const char* args);

/**
 * @brief 回到原點命令的處理函式
 */
void GoHomeCmdHandler(const char* args);

#ifdef __cplusplus
}
#endif

#endif /* _PRINTER_CONTROLLER_H_ */
