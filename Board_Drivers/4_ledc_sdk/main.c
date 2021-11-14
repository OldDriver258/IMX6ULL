/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : main.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 完成 C 语言的初始化main
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "MCIMX6Y2.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"

/*
 * @description : 使能所有外设的时钟
 * @prarm       : 无
 * @return      : 无
 */
void  clock_enable (void)
{
    int i;
    for(i = 0; i <= 6; i++)
    CCM->CCGR0 = 0xffffffff;
    CCM->CCGR1 = 0xffffffff;
    CCM->CCGR3 = 0xffffffff;
    CCM->CCGR4 = 0xffffffff;
    CCM->CCGR5 = 0xffffffff;
    CCM->CCGR6 = 0xffffffff;
}

/*
 * @description : 使能GPIO
 * @prarm       : 无
 * @return      : 无
 */
void  led_enable (void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0X10B0);
    GPIO1->GDIR                 = 0x8;
    GPIO1->DR                   = 0x0;
}

/*
 * @description : 打开 LED
 * @prarm       : 无
 * @return      : 无
 */
void  led_on (void)
{
    GPIO1->DR &= ~(1 << 3);
}

/*
 * @description : 关闭 LED
 * @prarm       : 无
 * @return      : 无
 */
void  led_off (void)
{
    GPIO1->DR |= (1 << 3);
}

/*
 * @description : 短时间循环
 * @prarm - n   : 空循环次数
 * @return      : 无
 */
void  delay_short (volatile unsigned int n)
{
    while(n--){}
}

/*
 * @description : 延时函数
 * @prarm - ms  : 延时的 ms 数
 * @return      : 无
 */
void  delay (volatile unsigned int n)
{
    while (n--) {
        delay_short(0x7FF);
    }
}

/*
 * @description : main 函数
 * @prarm       : 无
 * @return      : 无
 */
int  main (void)
{
    /* 初始化 */
    clock_enable();
    led_enable();

    while(1) {
        led_off();
        delay(500);

        led_on();
        delay(500);
    }

    return  0;
}