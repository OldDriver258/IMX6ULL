/**************************************************************
文件名 : bsp_spi.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : spi 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_SPI_H
#define _BSP_SPI_H
#include "imx6ul.h"

void  spi_init (ECSPI_Type  *base);
unsigned char  spich0_readwrite_byte (ECSPI_Type  *base, unsigned char txdata);

#endif