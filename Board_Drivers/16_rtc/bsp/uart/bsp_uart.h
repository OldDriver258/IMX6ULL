/**************************************************************
文件名 : bsp_uart.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : uart 驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "imx6ul.h"

void  uart_init (void);
void  putc (unsigned char c);
void  puts (char  *str);
unsigned char  getc (void);

#endif