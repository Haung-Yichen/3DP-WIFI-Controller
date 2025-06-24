#include "UITask.h"

#include <stdio.h>

void touchTask(void *argument) {
	while (1) {
		GUI_TOUCH_Exec();
		vTaskDelay(10);
	}
}

void GUI_Task(void *argument) {
	GUI_Init();
	while (1) {
		MainTask();
	}
}