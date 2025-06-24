#include "bsp_xpt2046_lcd.h"
#include "main.h"  // 確保 GPIO 定義來自 CubeMX

static void XPT2046_DelayUS(uint32_t ulCount);
static void XPT2046_WriteCMD(uint8_t ucCmd);
static uint16_t XPT2046_ReadCMD(void);

void XPT2046_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // CLK
    GPIO_InitStruct.Pin = XPT2046_SPI_CLK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(XPT2046_SPI_CLK_PORT, &GPIO_InitStruct);

    // MOSI
    GPIO_InitStruct.Pin = XPT2046_SPI_MOSI_PIN;
    HAL_GPIO_Init(XPT2046_SPI_MOSI_PORT, &GPIO_InitStruct);

    // MISO
    GPIO_InitStruct.Pin = XPT2046_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_SPI_MISO_PORT, &GPIO_InitStruct);

    // CS
    GPIO_InitStruct.Pin = XPT2046_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(XPT2046_SPI_CS_PORT, &GPIO_InitStruct);

    XPT2046_CS_DISABLE();

    // PENIRQ
    GPIO_InitStruct.Pin = XPT2046_PENIRQ_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(XPT2046_PENIRQ_GPIO_PORT, &GPIO_InitStruct);
}

static void XPT2046_DelayUS(uint32_t ulCount) {
    uint32_t i;
    for (i = 0; i < ulCount; i++) {
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    }
}

static void XPT2046_WriteCMD(uint8_t ucCmd) {
    uint8_t i;

    XPT2046_MOSI_0();
    XPT2046_CLK_LOW();

    for (i = 0; i < 8; i++) {
        if (ucCmd & 0x80)
            XPT2046_MOSI_1();
        else
            XPT2046_MOSI_0();

        ucCmd <<= 1;
        XPT2046_DelayUS(5);
        XPT2046_CLK_HIGH();
        XPT2046_DelayUS(5);
        XPT2046_CLK_LOW();
    }
}

static uint16_t XPT2046_ReadCMD(void) {
    uint8_t i;
    uint16_t usBuf = 0;

    XPT2046_MOSI_0();
    XPT2046_CLK_HIGH();

    for (i = 0; i < 12; i++) {
        XPT2046_CLK_LOW();
        usBuf <<= 1;
        if (XPT2046_MISO())
            usBuf |= 0x01;
        XPT2046_CLK_HIGH();
    }
    return usBuf;
}

#define SAMP_CNT 4
#define SAMP_CNT_DIV2 2

uint16_t XPT2046_ReadAdc_Fliter(uint8_t channel) {
    uint8_t i, j, min;
    uint16_t temp, tempXY[SAMP_CNT];
    static uint16_t adc_x = 0, adc_y = 0;

    if (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) {
        for (i = 0; i < SAMP_CNT; i++) {
            XPT2046_WriteCMD(channel);
            tempXY[i] = XPT2046_ReadCMD();
        }

        // 排序
        for (i = 0; i < SAMP_CNT - 1; i++) {
            min = i;
            for (j = i + 1; j < SAMP_CNT; j++) {
                if (tempXY[min] > tempXY[j]) min = j;
            }
            temp = tempXY[i];
            tempXY[i] = tempXY[min];
            tempXY[min] = temp;
        }

        // 取中間兩值均值，判斷差異
        if ((tempXY[SAMP_CNT_DIV2] - tempXY[SAMP_CNT_DIV2 - 1]) > 5) {
            return (channel == macXPT2046_CHANNEL_Y) ? adc_x : adc_y;
        }

        if (channel == macXPT2046_CHANNEL_Y) {
            adc_x = (tempXY[SAMP_CNT_DIV2] + tempXY[SAMP_CNT_DIV2 - 1]) / 2;
            return adc_x;
        } else {
            adc_y = (tempXY[SAMP_CNT_DIV2] + tempXY[SAMP_CNT_DIV2 - 1]) / 2;
            return adc_y;
        }
    } else {
        return 0;
    }
}
