/**************************************************************
文件名 : bsp_backlight.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : pwm 背光调节 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BACKLIGHT_H
#define _BACKLIGHT_H

#include "imx6ul.h"

/* 背光PWM结构体 */
struct backlight_dev_struc
{
	unsigned char pwm_duty;		/* 占空比	*/
};


/* 函数声明 */
void backlight_init(void);
void pwm1_enable(void);
void pwm1_setsample_value(unsigned int value);
void pwm1_setperiod_value(unsigned int value);
void pwm1_setduty(unsigned char duty);
void pwm1_irqhandler(void);

#endif