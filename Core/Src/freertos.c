/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ledTask.h"
#include "esp32.h"
#include "UITask.h"
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
/* USER CODE BEGIN Variables */

//UI主線程
osThreadId_t UITaskHandle;
const osThreadAttr_t UI_Task_attributes = {
	.name = "UI_Task",
	.stack_size = 128* 16,
	.priority = (osPriority)osPriorityNormal7,
};

//觸控檢測線程
osThreadId_t TouchTaskHandle;
const osThreadAttr_t Touch_Task_attributes = {
	.name = "Touch_Task",
	.stack_size = 128* 8,
	.priority = (osPriority)osPriorityNormal7,
};

//esp32 uart解析線程
osThreadId_t esp32RxHandlerTaskHandle;
const osThreadAttr_t esp32RxHandlerTask_attributes = {
	.name = "esp32RxHandlerTask",
	.stack_size = 128 * 4,
	.priority = (osPriority) osPriorityHigh,
};

//調試用
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
	.name = "ledTask",
	.stack_size = 128 * 2,
	.priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
	.name = "defaultTask",
	.stack_size = 128 * 1,
	.priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	UITaskHandle = osThreadNew(GUI_Task, NULL, &UI_Task_attributes);
	TouchTaskHandle = osThreadNew(touchTask, NULL, &Touch_Task_attributes);
	esp32RxHandlerTaskHandle = osThreadNew(ESP32_RxHandler_Task, NULL, &esp32RxHandlerTask_attributes);
	ledTaskHandle = osThreadNew(ledBlinkTask, NULL, &ledTask_attributes);

	//任務建立結果檢查
#ifdef DEBUG
	if (defaultTaskHandle == NULL) {
		printf("defaultTaskHandle created failed!!\r\n");
	}
	if (UITaskHandle == NULL) {
		printf("UITaskHandle created failed!!\r\n");
	}
	if (TouchTaskHandle == NULL) {
		printf("TouchTaskHandle created failed!!\r\n");
	}
	if (esp32RxHandlerTaskHandle == NULL) {
		printf("esp32RxHandlerTaskHandle created failed!!\r\n");
	}
	if (ledTaskHandle == NULL) {
		printf("ledTaskHandle created failed!!\r\n");
	}
#endif
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
