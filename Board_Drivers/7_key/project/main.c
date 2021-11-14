/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
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

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
    int         keyvalue = 0;
    uint8_t     led_state, beep_state;

    /* 初始化 */
    clk_enable();
    led_init();
    beep_init();
    key_init();

    led_state = 0;
    beep_state = 0;

    while(1) {
        keyvalue = key_getvalue();
        if (keyvalue) {         /*  按键按下    */
            switch (keyvalue)
            {
            case KEY0_VALUE:
                led_state = !led_state;
                beep_state = !beep_state;
                break;
            
            default:
                break;
            }
        }
    led_switch(LED0, led_state);
    beep_switch(beep_state);
    }

    return  0;
}