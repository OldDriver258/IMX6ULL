/**************************************************************

文件名 : main.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 完成 C 语言的初始化main
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "imx6ul.h"
#include "bsp_clk.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_gpio.h"
#include "bsp_key.h"
#include "bsp_int.h"
#include "bsp_exit.h"
#include "bsp_timer.h"
#include "bsp_keyfilter.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "bsp_lcd.h"
#include "bsp_lcdapi.h"

/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW,
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

};

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
	unsigned char index = 0;
	unsigned char state = 1;

    /* 初始化 */
    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();
    led_init();
    beep_init();
    uart_init();
    lcd_init(); /* 初始化 LCD */

    tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(10,10,400,32,32,(char*)"ZERO-IMX6UL ELCD TEST");  /* 显示字符串 */
	lcd_draw_rectangle(10, 52, 1014, 290);	/* 绘制矩形框  		*/
	lcd_drawline(10, 52,1014, 290);			/* 绘制线条		  	*/
	lcd_drawline(10, 290,1014, 52);			/* 绘制线条 		*/
	lcd_draw_Circle(512, 171, 119);			/* 绘制圆形 		*/

    while(1) {
 		index++;
		if(index == 10)
			index = 0;
		lcd_fill(0, 300, 1023, 599, backcolor[index]);
		lcd_show_string(800,10,240,32,32,(char*)"INDEX=");  /*显示字符串				  */
		lcd_shownum(896,10, index, 2, 32); 					/* 显示数字，叠加显示	*/

		state = !state;
		led_switch(LED0,state);
		delayms(1000);	/* 延时一秒	*/
    }

    return  0;
}