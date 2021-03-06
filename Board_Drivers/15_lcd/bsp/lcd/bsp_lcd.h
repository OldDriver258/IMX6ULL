/**************************************************************
文件名 : bsp_lcd.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : LCD 底层驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#ifndef _BSP_LCD_H
#define _BSP_LCD_H
#include "imx6ul.h"

/* 颜色 */
#define LCD_BLUE		  0x000000FF
#define LCD_GREEN		  0x0000FF00
#define LCD_RED 		  0x00FF0000
#define LCD_CYAN		  0x0000FFFF
#define LCD_MAGENTA 	  0x00FF00FF
#define LCD_YELLOW		  0x00FFFF00
#define LCD_LIGHTBLUE	  0x008080FF
#define LCD_LIGHTGREEN	  0x0080FF80
#define LCD_LIGHTRED	  0x00FF8080
#define LCD_LIGHTCYAN	  0x0080FFFF
#define LCD_LIGHTMAGENTA  0x00FF80FF
#define LCD_LIGHTYELLOW   0x00FFFF80
#define LCD_DARKBLUE	  0x00000080
#define LCD_DARKGREEN	  0x00008000
#define LCD_DARKRED 	  0x00800000
#define LCD_DARKCYAN	  0x00008080
#define LCD_DARKMAGENTA   0x00800080
#define LCD_DARKYELLOW	  0x00808000
#define LCD_WHITE		  0x00FFFFFF
#define LCD_LIGHTGRAY	  0x00D3D3D3
#define LCD_GRAY		  0x00808080
#define LCD_DARKGRAY	  0x00404040
#define LCD_BLACK		  0x00000000
#define LCD_BROWN		  0x00A52A2A
#define LCD_ORANGE		  0x00FFA500
#define LCD_TRANSPARENT   0x00000000

/* LCD显存地址 */
#define LCD_FRAMEBUF_ADDR	(0x89000000)

//LCD 结构体
typedef struct tftlcd_typedef{
    unsigned short  height;     //LCD 屏幕高度
    unsigned short  width;      //LCD 屏幕宽度
    unsigned char   pixsize;    //LCD 每个像素占用字节数
    unsigned short  vspw;       //VSYNC 数据宽度
    unsigned short  vbpd;       /* 帧同步信号后肩 */
    unsigned short  vfpd;       /* 帧同步信号前肩 */
    unsigned short  hspw;       /* HSYNC 信号宽度 */
    unsigned short  hbpd;       /* 水平同步信号后见肩 */
    unsigned short  hfpd;       /* 水平同步信号前肩 */
    unsigned int    framebuffer;    /* LCD 显存首地址 */
    unsigned int    forecolor;      /* 前景色 */
    unsigned int    backcolor;      /* 背景色 */
}tftlcd_typedef;

//屏幕参数
extern tftlcd_typedef tftlcd_dev;

void  lcd_init (void);
void  lcd_clear (unsigned int color);
inline void lcd_drawpoint(unsigned short x,unsigned short y,unsigned int color);
inline unsigned int lcd_readpoint(unsigned short x,unsigned short y);
void  lcd_fill (unsigned    short x0, unsigned short y0,
                unsigned short x1, unsigned short y1, unsigned int color);

#endif