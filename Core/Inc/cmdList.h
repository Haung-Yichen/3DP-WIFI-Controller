#ifndef INC_COMMAND_LIST_H_
#define INC_COMMAND_LIST_H_

#define CMD_Start_Transmisson   (const char*)"cStartTransmisson"  //開始傳送(檔案)
#define CMD_Transmisson_Over    (const char*)"cTransmissonOver"   //傳送完畢(檔案)
#define CMD_Start_To_Print      (const char*)"cStartToPrint"      //開始列印
#define CMD_Pause_Printing      (const char*)"cPausePrinting"     //暫停列印S
#define CMD_Stop_printing       (const char*)"cStopPrinting"      //停止列印
#define CMD_Go_Home             (const char*)"cGoHome"            //回原點
#define CMD_Get_Remainning_time (const char*)"cReqRemainningTime" //請求剩餘列印時間
#define CMD_Get_Progress        (const char*)"cReqProgress"       //請求進度(百分比)
#define CMD_Get_Nozzle_Temp     (const char*)"cReqNozzleTemp"     //請求噴頭溫賭
#define CMD_Get_Bed_Temp        (const char*)"cReqBedTemp"        //請求熱床溫度
#define CMD_Set_Nozzle_Temp     (const char*)"cSetNozzleTemp"     //設置噴嘴溫度
#define CMD_Set_Bed_Temp        (const char*)"cSetBedTemp"        //設置熱床溫度
#define CMD_Enable_Camera       (const char*)"cEnableCamera"      //啟用錄影
#define CMD_Disable_Camera      (const char*)"cDisableCamera"     //進用錄影
#define CMD_GetFilament_Weight  (const char*)"cReqFilamentWeight" //請求耗材重量
#define CMD_Emergency_Stop      (const char*)"cEmergencyStop"     //緊急停止

#endif /* INC_COMMAND_LIST_H_ */
