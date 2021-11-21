/**************************************************************

文件名 : bsp_clk.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 系统时钟驱动
日志 : 初版  V1.0 2021/10/31 张宇辰创建
**************************************************************/
#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#include "imx6ul.h"

void  clk_enable (void);
void imx6u_clkinit(void);

#endif // !__BSP_CLK_H