/**************************************************************
文件名 : bsp_led.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : led 驱动
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "imx6ul.h"

#define LED0    0

void led_init(void);
void led_switch(int led, int status);

#endif