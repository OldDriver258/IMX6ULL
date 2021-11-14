/**************************************************************

文件名 : bsp_beep.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 蜂鸣器驱动
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "imx6ul.h"

void  beep_init (void);
void  beep_switch (int  status);

#endif // !__BSP_BEEP_H