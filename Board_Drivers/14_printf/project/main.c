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

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
    int a, b;
    unsigned char state = 1;

    /* 初始化 */
    int_init();
    imx6u_clkinit();
    clk_enable();
    led_init();
    beep_init();
    uart_init();


    while(1) {
        printf("输入两个整数， 用空格隔开：");
        scanf("%d %d", &a, &b);
        printf("\r\n数据%d + %d = %d\r\n\r\n", a, b, a+b);

        state = !state;
        led_switch(LED0, state);
    }

    return  0;
}