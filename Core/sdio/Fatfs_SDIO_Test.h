#ifndef FATFS_SDIO_TEST_H
#define FATFS_SDIO_TEST_H

#include <stdio.h>
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"
#include "bsp_sdio_sdcard.h"
#include "usart.h"
#include "ff_print_err.h"

//用FATFS測試讀寫SD卡
void SDIO_FatFs_RW_Test();

#endif //FATFS_SDIO_TEST_H
