/**
  ******************************************************************************
  * @file    bsp_ili9341_lcd.c
  * @version V1.0
  * @date    2023-xx-xx
  * @brief   ILI9341 液晶驅動程式 (HAL庫版本)
  ******************************************************************************
  * @attention
  *
  * 實驗平台: 野火 F103-指南者 STM32 開發板
  * 論壇    : http://www.firebbs.cn
  * 淘寶    : https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "bsp_ili9341_lcd.h"

#include "main.h"

// 根據液晶掃描方向而變化的XY像素寬度
// 調用ILI9341_GramScan函數設置方向時會自動更改
uint16_t LCD_X_LENGTH = ILI9341_LESS_PIXEL;
uint16_t LCD_Y_LENGTH = ILI9341_MORE_PIXEL;

// 液晶屏掃描模式，參數可選值為0-7
// 調用ILI9341_GramScan函數設置方向時會自動更改
// LCD在初始完成後會使用此默認值
uint8_t LCD_SCAN_MODE = 5;

/**
  * @brief  向ILI9341寫入命令
  * @param  usCmd : 要寫入的命令（寄存器地址）
  * @retval 無
  */
void ILI9341_Write_Cmd(uint16_t usCmd) {
	*(__IO uint16_t *) (FSMC_Addr_ILI9341_CMD) = usCmd;
}

/**
  * @brief  向ILI9341寫入數據
  * @param  usData : 要寫入的數據
  * @retval 無
  */
void ILI9341_Write_Data(uint16_t usData) {
	*(__IO uint16_t *) (FSMC_Addr_ILI9341_DATA) = usData;
}

/**
  * @brief  從ILI9341讀取數據
  * @param  無
  * @retval 讀取到的數據
  */
uint16_t ILI9341_Read_Data(void) {
	return (*(__IO uint16_t *) (FSMC_Addr_ILI9341_DATA));
}

/**
  * @brief  ILI9341 簡單延時函數
  * @param  nCount 延時計數值
  * @retval 無
  */
static void ILI9341_Delay(__IO uint32_t nCount) {
	for (; nCount != 0; nCount--);
}

/**
  * @brief  初始化ILI9341的GPIO
  * @param  無
  * @retval 無
  */
static void ILI9341_GPIO_Config(void) {
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* 初始化FSMC相關控制信號線 */
	/* RD (讀使能) */
	GPIO_InitStruct.Pin = ILI9341_RD_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ILI9341_RD_PORT, &GPIO_InitStruct);

	/* WR (寫使能) */
	GPIO_InitStruct.Pin = ILI9341_WR_PIN;
	HAL_GPIO_Init(ILI9341_WR_PORT, &GPIO_InitStruct);

	/* DC (數據/命令選擇) */
	GPIO_InitStruct.Pin = ILI9341_DC_PIN;
	HAL_GPIO_Init(ILI9341_DC_PORT, &GPIO_InitStruct);

	/* CS (片選) */
	GPIO_InitStruct.Pin = ILI9341_CS_PIN;
	HAL_GPIO_Init(ILI9341_CS_PORT, &GPIO_InitStruct);

	/* 初始化數據線 D0-D15 */
	GPIO_InitStruct.Pin = ILI9341_D0_PIN | ILI9341_D1_PIN | ILI9341_D2_PIN | ILI9341_D3_PIN |
	                      ILI9341_D4_PIN | ILI9341_D5_PIN | ILI9341_D6_PIN | ILI9341_D7_PIN |
	                      ILI9341_D8_PIN | ILI9341_D9_PIN | ILI9341_D10_PIN | ILI9341_D11_PIN |
	                      ILI9341_D12_PIN | ILI9341_D13_PIN | ILI9341_D14_PIN | ILI9341_D15_PIN;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); // 假設大部分數據線在GPIOD
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); // 部分數據線可能在GPIOE

	/* 初始化復位信號線 */
	GPIO_InitStruct.Pin = ILI9341_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(ILI9341_RST_PORT, &GPIO_InitStruct);

	/* 初始化背光控制線 */
	GPIO_InitStruct.Pin = ILI9341_BK_PIN;
	HAL_GPIO_Init(ILI9341_BK_PORT, &GPIO_InitStruct);
}

/**
  * @brief  LCD FSMC模式配置 (STM32F1專用版本)
  * @param  無
  * @retval 無
  */
static void ILI9341_FSMC_Config ( void )
{
	SRAM_HandleTypeDef hsram;
	FSMC_NORSRAM_TimingTypeDef  readWriteTiming = {0};

	/* 使能FSMC时钟*/
	__HAL_RCC_FSMC_CLK_ENABLE();

	//地址建立时间（ADDSET）为1个HCLK 2/72M=28ns
	readWriteTiming.AddressSetupTime      = 0x01;    //地址建立时间
	//数据保持时间（DATAST）+ 1个HCLK = 5/72M=70ns
	readWriteTiming.DataSetupTime         = 0x04;    //数据建立时间
	//选择控制的模式
	//模式B,异步NOR FLASH模式，与ILI9341的8080时序匹配
	readWriteTiming.AccessMode            = FSMC_ACCESS_MODE_B;

	/*以下配置与模式B无关*/
	//地址保持时间（ADDHLD）模式A未用到
	readWriteTiming.AddressHoldTime       = 0x00;    //地址保持时间
	//设置总线转换周期，仅用于复用模式的NOR操作
	readWriteTiming.BusTurnAroundDuration = 0x00;
	//设置时钟分频，仅用于同步类型的存储器
	readWriteTiming.CLKDivision           = 0x00;
	//数据保持时间，仅用于同步型的NOR
	readWriteTiming.DataLatency           = 0x00;

	/* 配置FSMC参数 */
	hsram.Instance = FSMC_NORSRAM_DEVICE;
	hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	hsram.Init.NSBank = FSMC_NORSRAM_BANK1;
	hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryType = FSMC_MEMORY_TYPE_NOR;
	hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
}

/**
  * @brief  初始化ILI9341寄存器
  * @param  無
  * @retval 無
  */
static void ILI9341_REG_Config(void) {
	/* Power control B (CFh) */
	DEBUG_DELAY();
	ILI9341_Write_Cmd(0xCF);
	ILI9341_Write_Data(0x00);
	ILI9341_Write_Data(0x81);
	ILI9341_Write_Data(0x30);

	/* 其他寄存器配置保持不變... */
	/* 這裡省略了中間的寄存器配置代碼，實際使用時需要保留 */

	/* Sleep Out (11h) */
	ILI9341_Write_Cmd(0x11);
	ILI9341_Delay(0xAFFf << 2);
	DEBUG_DELAY();

	/* Display ON (29h) */
	ILI9341_Write_Cmd(0x29);
}

/**
  * @brief  ILI9341初始化函數，使用LCD前必須先調用此函數
  * @param  無
  * @retval 無
  */
void ILI9341_Init(void) {
	ILI9341_GPIO_Config(); // 1. GPIO初始化
	ILI9341_FSMC_Config(); // 2. FSMC控制器配置

	ILI9341_Rst(); // 3. 硬件复位
	ILI9341_REG_Config(); // 4. 寄存器配置

	ILI9341_GramScan(LCD_SCAN_MODE); // 5. 设置扫描方向
	ILI9341_BackLed_Control(ENABLE); // 6. 最后开启背光
}

/**
  * @brief  ILI9341背光控制
  * @param  enumState 控制是否使能背光LED
  *   @arg ENABLE : 使能背光LED
  *   @arg DISABLE : 禁用背光LED
  * @retval 無
  */
void ILI9341_BackLed_Control(FunctionalState enumState) {
	if (enumState)
		HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(ILI9341_BK_PORT, ILI9341_BK_PIN, GPIO_PIN_SET);
}

/**
  * @brief  ILI9341硬件復位
  * @param  無
  * @retval 無
  */
void ILI9341_Rst(void) {
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET); // 低電平復位
	ILI9341_Delay(0xAFF);
	HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET); // 釋放復位
	ILI9341_Delay(0xAFF);
}

/**
  * @brief  設置ILI9341的GRAM掃描方向
  * @param  ucOption 可選GRAM掃描方向
  *     @arg 0-7 : 參數可選值為0-7，具體見下方說明
  *
  * 推薦模式0、3、5、6適合橫屏顯示文字；
  * 模式0、2、4、6的X最大寬度為240，Y最大寬度為320
  * 模式1、3、5、7的X最大寬度為320，Y最大寬度為240
  *
  * 模式6為大部分液晶屏的默認顯示方向
  * 模式3為豎屏方向
  * 模式0為BMP圖片顯示常用的方向
  *
  * @retval 無
  */
void ILI9341_GramScan(uint8_t ucOption) {
	// 參數檢查，只允許0-7
	if (ucOption > 7)
		return;

	// 更新模式值到LCD_SCAN_MODE，用於後續程序選擇方向
	LCD_SCAN_MODE = ucOption;

	// 根據模式設置XY方向的像素寬度
	if (ucOption % 2 == 0) {
		// 0 2 4 6模式，X方向像素寬度為240，Y方向為320
		LCD_X_LENGTH = ILI9341_LESS_PIXEL;
		LCD_Y_LENGTH = ILI9341_MORE_PIXEL;
	} else {
		// 1 3 5 7模式，X方向像素寬度為320，Y方向為240
		LCD_X_LENGTH = ILI9341_MORE_PIXEL;
		LCD_Y_LENGTH = ILI9341_LESS_PIXEL;
	}

	// 0x36命令的高3位控制GRAM掃描方向
	ILI9341_Write_Cmd(0x36);
	ILI9341_Write_Data(0x08 | (ucOption << 5)); // 設置ucOption的值到LCD控制器(0-7模式)

	// 設置X坐標範圍
	ILI9341_Write_Cmd(CMD_SetCoordinateX);
	ILI9341_Write_Data(0x00); // x起始坐標高8位
	ILI9341_Write_Data(0x00); // x起始坐標低8位
	ILI9341_Write_Data(((LCD_X_LENGTH - 1) >> 8) & 0xFF); // x結束坐標高8位
	ILI9341_Write_Data((LCD_X_LENGTH - 1) & 0xFF); // x結束坐標低8位

	// 設置Y坐標範圍
	ILI9341_Write_Cmd(CMD_SetCoordinateY);
	ILI9341_Write_Data(0x00); // y起始坐標高8位
	ILI9341_Write_Data(0x00); // y起始坐標低8位
	ILI9341_Write_Data(((LCD_Y_LENGTH - 1) >> 8) & 0xFF); // y結束坐標高8位
	ILI9341_Write_Data((LCD_Y_LENGTH - 1) & 0xFF); // y結束坐標低8位

	/* 開始寫GRAM */
	ILI9341_Write_Cmd(CMD_SetPixel);
}

/*********************檔案結束*************************/
