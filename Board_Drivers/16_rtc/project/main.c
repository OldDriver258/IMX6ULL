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
	unsigned char key = 0;
	int 	i = 3; /* 倒计时 3S */
	int 	t = 0;
	int		state = 1;
	char 	buf[160];
	struct rtc_datetime rtcdate;

    /* 初始化 */
    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();
    led_init();
    beep_init();
	key_init();
    uart_init();
    lcd_init(); /* 初始化 LCD */
	rtc_init();

    tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, /* 显示字符串 */
				    (char*)"ALPHA-IMX6UL RTC TEST");

	tftlcd_dev.forecolor = LCD_BLUE;
	memset(buf, 0, sizeof(buf));

    while(1) {
		if (t == 100) {
			t = 0;
			printf("will be running %d s......\r", i);

			lcd_fill(50, 40,370, 70, tftlcd_dev.backcolor); /* 清屏 */
			sprintf(buf, "will be running %ds......", i);
			lcd_show_string(50, 40, 300, 24, 24, buf);
		}

		key = key_getvalue();
		if (key == KEY0_VALUE)
		{
			rtcdate.year 	= 2021;
			rtcdate.month 	= 11;
			rtcdate.day 	= 19;
			rtcdate.hour	= 0;
			rtcdate.minute	= 50;
			rtcdate.second	= 50;

			rtc_setDatetime(&rtcdate);
			printf("\r\n RTC Init finish\r\n");
			break;
		}

		delayms(10);
		t++;
    }

	tftlcd_dev.forecolor = LCD_RED;
	lcd_fill(50, 40,370, 70, tftlcd_dev.backcolor); /* 清屏 */
	lcd_show_string(50, 40, 200, 24, 24, (char*)"Current Time:");
	tftlcd_dev.forecolor = LCD_BLUE;

	while (1) {
		rtc_getDatetime(&rtcdate);
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtcdate.year,
								 		rtcdate.month,
 										rtcdate.day,
										rtcdate.hour,
										rtcdate.minute,
										rtcdate.second);
		lcd_fill(50,70, 300,94, tftlcd_dev.backcolor);
		lcd_show_string(50,70,250,24,24,(char*)buf); /* 显示字符串 */

		state = !state;
		led_switch(LED0,state);
		delayms(1000); /* 延时一秒 */
	}

    return  0;
}