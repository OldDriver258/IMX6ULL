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

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
    /* 初始化 */
    int_init();
    imx6u_clkinit();
    clk_enable();
    led_init();
    beep_init();
    key_init();
    // exit_init();
    //定时器触发频率 66000000/3300/10000 = 2, 周期 500ms
    // epit_init(3300, 10000);
    filterkey_init();

    while(1) {
        delay(500);
    }

    return  0;
}