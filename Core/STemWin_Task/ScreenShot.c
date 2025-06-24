/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 ����ģ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F103 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
/*******************************************************************************
 * ������ͷ�ļ�
 ******************************************************************************/
#include "ScreenShot.h"
#include "usart.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
 * ȫ�ֱ���
 ******************************************************************************/ 
extern FATFS   fs;			/* FatFs�ļ�ϵͳ���� */
extern FIL     file;		/* file objects */
extern UINT    bw;     /* File R/W count */
extern FRESULT result; 
extern FILINFO fno;
extern DIR dir;

uint16_t  file_num = 0;

/**
  * @brief FS_Init
  * @note �ļ�ϵͳ��ʼ��
  * @param ��
  * @retval ��
  */
void FS_Init(void)
{
	const TCHAR *ScreenShotPATH  = "0:/ScreenShot";
	
	/* �����ļ�ϵͳ������ʱ���SD����ʼ�� */
  result = f_mount(&fs,"0:",1);
	if(result != FR_OK)
	{
		printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
		while(1);
	}
	/*  ���·�������ڣ��򴴽�·��  */
	result = f_opendir(&dir, ScreenShotPATH);
	while(result != FR_OK)
	{
		f_mkdir(ScreenShotPATH);
		result = f_opendir(&dir, ScreenShotPATH);
	}
	/* ɨ���Ѵ��ڵĽ�ͼ�ļ����� */
	while(f_readdir(&dir, &fno) == FR_OK)
	{
		if(fno.fname[0] == 0)
			break;
		if(fno.fattrib == AM_ARC)
			file_num++;
		vTaskDelay(1);
	}
}

/**
  * @brief ScreenShot
  * @note ��ȡLCD��Ļ��ǰ����ʾ����
  * @param ��
  * @retval ��
  */
void ScreenShot(void)
{
	char filename[35] = {0};
	
	sprintf(filename, "0:/ScreenShot/ScreenShot_%03d.bmp", file_num);
	result = f_open(&file, (const char*)filename, FA_WRITE | FA_CREATE_ALWAYS);
	if(result == FR_OK)
	{
		printf("��ʼ���������ļ�\r\n");
		
		GUI_BMP_Serialize(_WriteByte2File, &file);
	
		result = f_close(&file);
		
		printf("�����ļ������ɹ�\r\n");
		file_num++;
	}
	else
	{
		printf("�����ļ�����ʧ��\r\n");
	}
}

/**
  * @brief _WriteByte2File
  * @note д�����ݵ�ָ���洢����
  * @param ��
  * @retval ��
  */
void _WriteByte2File(U8 Data, void *p)
{
	f_write(p, &Data, 1, &bw);
}
