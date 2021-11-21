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
#include "bsp_rtc.h"
#include "bsp_ap3216c.h"

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
	unsigned short ir, als, ps;
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

	lcd_printf("ALPHA-IMX6UL IIC TEST");
	lcd_printf("AP3216C TEST");

	while (ap3216c_init()) {
		lcd_printf("AP3216C Check Failed!");
		delayms(500);
		lcd_printf("Please Check!	     ");
		delayms(500);
	}

	lcd_printf("AP3216C Ready!");

	while (1) {
		ap3216c_read(&ir, &ps, &als);
		lcd_printf("IR:  %4d, PS:  %4d, ALS: %4d", ir, ps, als);

		state = !state;
		led_switch(LED0,state);
	}

    return  0;
}