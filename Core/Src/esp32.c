#include "esp32.h"

#include "cmsis_os2.h"

SemaphoreHandle_t triOverSemaphore = NULL;
SemaphoreHandle_t staPriSemaphore = NULL;
SemaphoreHandle_t rxSemaphore = NULL;

uint8_t *ResBuf = 0;
uint8_t CRxBuf[MAX_CMD_LEN + 4] = {0};
uint8_t FRxBuf[100] = {0};

volatile uint16_t CRxLen = 0;

void ESP32_Init(void) {
	ESP32_SetState(ESP32_INIT);
	ESP32_RegCallback();

	rxSemaphore = xSemaphoreCreateBinary();
	triOverSemaphore = xSemaphoreCreateBinary();
	staPriSemaphore = xSemaphoreCreateBinary();

	HAL_UART_Receive_DMA(&ESP32_USART_PORT, CRxBuf, sizeof(CRxBuf));
	__HAL_UART_ENABLE_IT(&ESP32_USART_PORT, UART_IT_IDLE);

	ESP32_SetState(ESP32_IDLE);
	printf("esp32 init successed!\r\n");
}

void ESP32_RegCallback(void) {
	register_command(CMD_Start_Transmisson, StartTransmissionCmdHandler);
	register_command(CMD_Transmisson_Over, TransmissionOverCmdHandler);
	register_command(CMD_Start_To_Print, StartToPrintCmdHandler);
	register_command(CMD_Pause_Printing, PausePrintingCmdHandler);
	register_command(CMD_Stop_printing, StopPrintingCmdHandler);
	register_command(CMD_Go_Home, GoHomeCmdHandler);
	register_command(CMD_Get_Remainning_time, GetRemainingTimeCmdHandler);
	register_command(CMD_Get_Progress, GetProgressCmdHandler);
	register_command(CMD_Get_Nozzle_Temp, GetNozzleTempCmdHandler);
	register_command(CMD_Get_Bed_Temp, GetBedTempCmdHandler);
	register_command(CMD_Set_Nozzle_Temp, SetNozzleTempCmdHandler);
	register_command(CMD_Set_Bed_Temp, SetBedTempCmdHandler);
	register_command(CMD_GetFilament_Weight, GetFilamentWeightCmdHandler);
	register_command(CMD_Emergency_Stop, EmergencyStopCmdHandler);
}

ESP32_STATE_TypeDef ESP32_GetState(void) {
	return currentState;
}

void ESP32_SetState(ESP32_STATE_TypeDef state) {
	currentState = state;
}

void ESP32_RxHandler_Task(void *argument) {
	while (1) {
		if (xSemaphoreTake(rxSemaphore, HAL_MAX_DELAY) == pdTRUE) {
			//UART中斷觸發
			void *res = isReqCmd(CRxBuf) ? ResBuf : NULL; //判斷是否需要返回值
			execute_command(CRxBuf, res);
			memset(CRxBuf, 0, sizeof(CRxBuf));
		}
	}
}

void ESP32_ReturnClbkRes(void) {
	HAL_UART_Transmit_DMA(&ESP32_USART_PORT, ResBuf, sizeof(ResBuf));
}

void StartTransmissionCmdHandler(const char *args, void *res) {
#ifdef DEBUG
	printf("Start Transmitting...\r\n");
#endif
	//實作接收邏輯以及把資料存入SD卡
	ESP32_SetState(ESP32_BUSY);

	ESP32_SetState(ESP32_IDLE);
}

void TransmissionOverCmdHandler(const char *args, void *res) {
	//通知UI線程傳送完畢
	xSemaphoreGive(triOverSemaphore);
}

void StartToPrintCmdHandler(const char *args, void *res) {
#ifdef DEBUG
	printf("Start Printing...\r\n");
#endif
	//通知印表機控制器開始列印
	xSemaphoreGive(staPriSemaphore);
}

void PausePrintingCmdHandler(const char *args, void *res) {
	//通知印表機控制器暫停發送gcode
}

void StopPrintingCmdHandler(const char *args, void *res) {
	//清空列印計數器，印表機回原點
}

void GoHomeCmdHandler(const char *args, void *res) {
	// TODO: 實作回到原點邏輯
}

void GetRemainingTimeCmdHandler(const char *args, void *res) {
	// TODO: 實作回傳剩餘列印時間邏輯
}

void GetProgressCmdHandler(const char *args, void *res) {
	// TODO: 實作回傳列印進度邏輯
}

void GetNozzleTempCmdHandler(const char *args, void *res) {
	// TODO: 實作回傳噴嘴溫度邏輯
}

void GetBedTempCmdHandler(const char *args, void *res) {
	// TODO: 實作回傳熱床溫度邏輯
}

void SetNozzleTempCmdHandler(const char *args, void *res) {
	// TODO: 實作設置噴嘴溫度邏輯
}

void SetBedTempCmdHandler(const char *args, void *res) {
	// TODO: 實作設置熱床溫度邏輯
}

void GetFilamentWeightCmdHandler(const char *args, void *res) {
	// TODO: 實作回傳耗材重量邏輯
}

void EmergencyStopCmdHandler(const char *args, void *res) {
	// TODO: 實作緊急停止邏輯
}
