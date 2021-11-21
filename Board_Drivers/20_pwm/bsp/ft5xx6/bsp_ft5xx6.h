/**************************************************************
文件名 : bsp_ft5xx6.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : ft5xx6 电容触摸传感器 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_FT5XX6_H
#define _BSP_FT5XX6_H
#include "imx6ul.h"

struct ft5426_dev_struct {
    unsigned char   initflag;
    unsigned char   intflag;
    unsigned short  point_num;
    unsigned short  x[5];
    unsigned short  y[5];
};

extern struct ft5426_dev_struct ft5426_dev;

void ft5426_read_tpnum(void);
void ft5426_read_tpcoord(void);
void  ft5426_init (void);

#endif
