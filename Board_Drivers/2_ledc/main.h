/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : main.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 完成 C 语言的初始化main
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/

#ifndef     __MAIN_H
#define     __MAIN_H

#define CCM_CCGR(x)         *((volatile unsigned long*)(0x020c4068 + x * 4))
#define SW_MUX_GPIO1_IO03   *((volatile unsigned long*)0x020e0068)
#define SW_PAD_GPIO1_IO03   *((volatile unsigned long*)0x020e02f4)
#define GPIO1_GDIR          *((volatile unsigned long*)0x0209c004)
#define GPIO1_DR            *((volatile unsigned long*)0x0209c000)

#endif