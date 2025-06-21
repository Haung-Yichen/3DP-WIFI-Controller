#include "printerController.h"
#include <string.h>
#include <stdio.h>

void StartToPrintCmdHandler(const char* args) {
    (void)args;
    // 實作開始列印的邏輯
    printf("Start printing...\n");
}

void PausePrintingCmdHandler(const char* args) {
    (void)args;
    // 實作暫停列印的邏輯
    printf("Pause printing...\n");
}

void StopPrintingCmdHandler(const char* args) {
    (void)args;
    // 實作停止列印的邏輯
    printf("Stop printing...\n");
}

void GoHomeCmdHandler(const char* args) {
    (void)args;
    // 實作回到原點的邏輯
    printf("Go to home...\n");
}
