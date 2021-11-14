/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : bsp_exit.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 外部中断 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#ifndef _BSP_EXIT_H
#define _BSP_EXIT_H
#include "imx6ul.h"

void  exit_init (void);
void  gpio1_io18_irqhandle (void);

#endif // !_BSP_EXIT_H