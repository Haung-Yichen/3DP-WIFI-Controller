/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

#include <stdio.h>
#include <string.h>

#include "bsp_led.h"
#include "bsp_sdio_sdcard.h"

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "sdio_test.h"

char SDPath[4];					/* SD卡邏輯裝置路徑 */
FATFS fs;						/* FatFs檔案系統物件 */
FIL file;						/* 檔案物件 */
FRESULT f_res;					/* 檔案操作結果 */
UINT fnum;						/* 成功讀寫的位元組數 */
BYTE ReadBuffer[1024] = {0};	/* 讀取緩衝區 */
BYTE WriteBuffer[] = "歡迎使用野火STM32開發板 今天是個好日子，新建檔案系統測試檔案\r\n";

void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
static void printf_fatfs_error(FRESULT fresult);

/**
  * @brief  主程式進入點
  * @retval int
  */
int main(void) {
	HAL_Init();
	LED_GPIO_Config();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_PCD_Init();

	/* USER CODE BEGIN 2 */
	printf("\r\r\n****** 這是一個SD卡 檔案系統實驗 ******\r\r\n");

	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0) {
		f_res = f_mount(&fs, (TCHAR const *) SDPath, 1);
		printf_fatfs_error(f_res);

		if (f_res == FR_NO_FILESYSTEM) {
			printf("》SD卡還沒有檔案系統，即將進行格式化...\r\n");
			f_res = f_mkfs((TCHAR const *) SDPath, 0, 0);
			if (f_res == FR_OK) {
				printf("》SD卡已成功格式化檔案系統。\r\n");
				f_mount(NULL, (TCHAR const *) SDPath, 1);
				f_mount(&fs, (TCHAR const *) SDPath, 1);
			} else {
				printf("《《格式化失敗。》》\r\n");
				while (1);
			}
		} else if (f_res != FR_OK) {
			printf("！！SD卡掛載檔案系統失敗。(%d)\r\n", f_res);
			printf_fatfs_error(f_res);
			while (1);
		} else {
			printf("》檔案系統掛載成功，可以進行讀寫測試\r\n");
		}

		printf("****** 即將進行檔案寫入測試... ******\r\n");
		f_res = f_open(&file, "FatFs讀寫測試檔案.txt", FA_CREATE_ALWAYS | FA_WRITE);
		if (f_res == FR_OK) {
			printf("》開啟/創建 FatFs讀寫測試檔案.txt 檔案成功，向檔案寫入資料。\r\n");
			f_res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
			if (f_res == FR_OK) {
				printf("》檔案寫入成功，寫入位元組數據：%d\r\n", fnum);
				printf("》寫入的資料為：\r\n%s\r\n", WriteBuffer);
			} else {
				printf("！！檔案寫入失敗：(%d)\r\n", f_res);
			}
			f_close(&file);
		} else {
			printf("！！開啟/創建檔案失敗。\r\n");
		}

		printf("****** 即將進行檔案讀取測試... ******\r\n");
		f_res = f_open(&file, "FatFs test file.txt", FA_OPEN_EXISTING | FA_READ);
		if (f_res == FR_OK) {
			printf("》開啟檔案成功。\r\n");
			f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
			if (f_res == FR_OK) {
				printf("》檔案讀取成功，讀取位元組數據：%d\r\n", fnum);
				printf("》讀取的檔案內容為：\r\n%s \r\n", ReadBuffer);
			} else {
				printf("！！檔案讀取失敗：(%d)\r\n", f_res);
			}
		} else {
			printf("！！開啟檔案失敗。\r\n");
		}
		f_close(&file);

		f_mount(NULL, (TCHAR const *) SDPath, 1);
	}
	FATFS_UnLinkDriver(SDPath);

	/* USER CODE END 2 */

	osKernelInitialize();
	MX_FREERTOS_Init();
	osKernelStart();

	while (1) {
	}
}

/**
  * @brief 系統時鐘設定
  * @retval 無
  */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
/**
  * @brief  顯示 FatFS 錯誤代碼對應訊息
  * @param  fresult: 錯誤代碼
  * @retval 無
  */
static void printf_fatfs_error(FRESULT fresult) {
	switch (fresult) {
		case FR_OK:
			printf("》操作成功。\r\n");
			break;
		case FR_DISK_ERR:
			printf("！！硬體輸入輸出驅動錯誤。\r\n");
			break;
		case FR_INT_ERR:
			printf("！！斷言錯誤。\r\n");
			break;
		case FR_NOT_READY:
			printf("！！實體設備無法運作。\r\n");
			break;
		case FR_NO_FILE:
			printf("！！找不到檔案。\r\n");
			break;
		case FR_NO_PATH:
			printf("！！找不到路徑。\r\n");
			break;
		case FR_INVALID_NAME:
			printf("！！無效的路徑名稱。\r\n");
			break;
		case FR_DENIED:
		case FR_EXIST:
			printf("！！拒絕存取。\r\n");
			break;
		case FR_INVALID_OBJECT:
			printf("！！無效的檔案或路徑。\r\n");
			break;
		case FR_WRITE_PROTECTED:
			printf("！！裝置為寫入保護狀態。\r\n");
			break;
		case FR_INVALID_DRIVE:
			printf("！！無效的邏輯裝置。\r\n");
			break;
		case FR_NOT_ENABLED:
			printf("！！無效的工作區域。\r\n");
			break;
		case FR_NO_FILESYSTEM:
			printf("！！無效的檔案系統。\r\n");
			break;
		case FR_MKFS_ABORTED:
			printf("！！格式化操作因參數錯誤中止。\r\n");
			break;
		case FR_TIMEOUT:
			printf("！！操作逾時。\r\n");
			break;
		case FR_LOCKED:
			printf("！！檔案被鎖定。\r\n");
			break;
		case FR_NOT_ENOUGH_CORE:
			printf("！！無法取得足夠堆空間支援長檔名。\r\n");
			break;
		case FR_TOO_MANY_OPEN_FILES:
			printf("！！開啟檔案數量過多。\r\n");
			break;
		case FR_INVALID_PARAMETER:
			printf("！！無效的參數。\r\n");
			break;
	}
}

/* USER CODE END 4 */

/**
  * @brief 錯誤處理函式
  * @retval 無
  */
void Error_Handler(void) {
	__disable_irq();
	printf("\r\n<UNK>\r\n");
	while (1) {
	}
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
	/* 使用者可自行顯示錯誤的檔名與行號 */
}
#endif /* USE_FULL_ASSERT */
