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
#include "bsp_icm20608.h"
#include "bsp_ft5xx6.h"
#include "bsp_backlight.h"

/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW,
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

};

void  imx6ul_hardfpu_enable (void)
{
	uint32_t	cpacr;
	uint32_t	fpexc;

	cpacr = __get_CPACR();
	cpacr = (cpacr & ~(CPACR_ASEDIS_Msk | CPACR_D32DIS_Msk)) | (3UL << CPACR_cp10_Pos) | (3UL << CPACR_cp11_Pos);
	__set_CPACR(cpacr);

	fpexc = __get_FPEXC();
	fpexc |= 0x40000000UL;
	__set_FPEXC(fpexc);
}

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
	unsigned char keyvalue = 0;
	unsigned char state = 1;
	unsigned char duty = 0;

    /* 初始化 */
	imx6ul_hardfpu_enable();
    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();
    led_init();
    beep_init();
	key_init();
    uart_init();
    lcd_init(); /* 初始化 LCD */
	backlight_init();

    tftlcd_dev.forecolor = LCD_RED;

	lcd_printf("ALPHA-IMX6UL BACKLIGHT PWM TEST");
	lcd_printf("BACKLIGHT PWM TEST");

	tftlcd_dev.forecolor = LCD_BLUE;

	duty = 10;
	lcd_printf("PWM Duty: %d %%", duty);
	pwm1_setduty(duty);

	while (1) {
		keyvalue = key_getvalue();
		if (keyvalue == KEY0_VALUE) {
			duty += 10; /* 占空比加 10% */
			if(duty > 100) /* 如果占空比超过 100%，重新从 10%开始 */
				duty = 10;
			lcd_printf("PWM Duty: %d %%", duty);
			pwm1_setduty(duty);
		}

		state = !state;
		led_switch(LED0,state);
	}

    return  0;
}