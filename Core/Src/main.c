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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "gpio.h"
#include "usart.h"
#include "usb.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_led.h"
#include "bsp_sdio_sdcard.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
char SDPath[4]; /* SD卡邏輯設備路徑 */
FATFS fs; /* FatFs檔案系統物件 */
FIL file; /* 檔案物件 */
FRESULT f_res; /* 檔案操作結果 */
UINT fnum; /* 成功讀寫的位元組數 */
BYTE ReadBuffer[1024] = {0}; /* 讀取緩衝區 */
BYTE WriteBuffer[] =
		        "歡迎使用野火STM32開發板，今天是個好日子，新建檔案系統測試檔案\r\n";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void MX_FREERTOS_Init(void);

/* USER CODE BEGIN PFP */
static void printf_fatfs_error(FRESULT fresult);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
* @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick.
	 */
	HAL_Init();

	/* USER CODE BEGIN Init */
        LED_GPIO_Config();
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_USB_PCD_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* Call init function for freertos objects (in cmsis_os2.c) */
	MX_FREERTOS_Init();

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief  列印輸出 FatFs 錯誤資訊
 * @param  fresult：FatFs 返回值
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
			printf("！！物理裝置無法運作。\r\n");
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
			printf("！！邏輯裝置為寫入保護狀態。\r\n");
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
			printf("！！因為函式參數問題導致 f_mkfs 執行失敗。\r\n");
			break;
		case FR_TIMEOUT:
			printf("！！操作逾時。\r\n");
			break;
		case FR_LOCKED:
			printf("！！檔案已被鎖定或保護。\r\n");
			break;
		case FR_NOT_ENOUGH_CORE:
			printf("！！長檔案名稱支援無法分配堆疊空間。\r\n");
			break;
		case FR_TOO_MANY_OPEN_FILES:
			printf("！！開啟太多檔案。\r\n");
			break;
		case FR_INVALID_PARAMETER:
			printf("！！無效的參數。\r\n");
			break;
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
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

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
	                              RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
