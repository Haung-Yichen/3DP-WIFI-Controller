#ifndef INC_COMMAND_LIST_H_
#define INC_COMMAND_LIST_H_

#define CMD_Start_Transmisson   "cStartTransmisson"  //開始傳送(檔案)
#define CMD_Transmisson_Over    "cTransmissonOver"   //傳送完畢(檔案)
#define CMD_Start_To_Print      "cStartToPrint"      //開始列印
#define CMD_Pause_Printing      "cPausePrinting"     //暫停列印S
#define CMD_Stop_printing       "cStopPrinting"      //停止列印
#define CMD_Go_Home             "cGoHome"            //回原點
#define CMD_Get_Remainning_time "cReqRemainningTime" //請求剩餘列印時間
#define CMD_Get_Progress        "cReqProgress"       //請求進度(百分比)
#define CMD_Get_Nozzle_Temp     "cReqNozzleTemp"     //請求噴頭溫賭
#define CMD_Get_Bed_Temp        "cReqBedTemp"        //請求熱床溫度
#define CMD_Set_Nozzle_Temp     "cSetNozzleTemp"     //設置噴嘴溫度
#define CMD_Set_Bed_Temp        "cSetBedTemp"        //設置熱床溫度
#define CMD_Enable_Camera       "cEnableCamera"      //啟用錄影
#define CMD_Disable_Camera      "cDisableCamera"     //進用錄影
#define CMD_GetFilament_Weight  "cReqFilamentWeight" //請求耗材重量
#define CMD_Emergency_Stop      "cEmergencyStop"     //緊急停止

#endif /* INC_COMMAND_LIST_H_ */
