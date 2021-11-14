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

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
    /* 初始化 */
    clk_enable();
    led_init();
    beep_init();

    while(1) {
        led_switch(LED0, 1);
        beep_switch(1);
        delay(500);

        led_switch(LED0, 0);
        beep_switch(0);
        delay(500);
    }

    return  0;
}