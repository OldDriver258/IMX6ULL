/**************************************************************
文件名 : bsp_lcdapi.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : LCD API 函数接口
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#ifndef _BSP_LCDAPI_H
#define _BSP_LCDAPI_H
#include "imx6ul.h"

/* 函数声明 */
void lcd_drawline(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void lcd_draw_Circle(unsigned short x0,unsigned short y0,unsigned char r);
void lcd_showchar(unsigned     short x,unsigned short y,unsigned char num,unsigned char size, unsigned char mode);
unsigned int lcd_pow(unsigned char m,unsigned char n);
void lcd_shownum(unsigned short x, unsigned short y, unsigned int num, unsigned char len,unsigned char size);
void lcd_showxnum(unsigned short x, unsigned short y, unsigned int num, unsigned char len, unsigned char size, unsigned char mode);
void lcd_show_string(unsigned short x,unsigned short y,
unsigned short width, unsigned short height, unsigned char size,char *p);

#endif