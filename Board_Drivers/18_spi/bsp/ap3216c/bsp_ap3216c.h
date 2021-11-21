/**************************************************************
文件名 : bsp_ap3216c.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : ap3216c 环境传感器 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_AP3216C_H
#define _BSP_AP3216C_H
#include "imx6ul.h"

#define AP3216C_ADDR 0X1E       /* AP3216C 器件地址 */

void  ap3216c_read (unsigned short  *ir, unsigned short  *ps, unsigned short  *als);
unsigned char  ap3216c_init (void);

#endif