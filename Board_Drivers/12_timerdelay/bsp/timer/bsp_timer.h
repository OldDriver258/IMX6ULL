/**************************************************************
文件名 : bsp_timer.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 硬件 timer 驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H
#include "imx6ul.h"

void  epit_init (unsigned int  frac, unsigned int  value);
void  epit1_irqhandler (void);

#endif