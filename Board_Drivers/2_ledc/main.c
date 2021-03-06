/**************************************************************

文件名 : main.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 完成 C 语言的初始化main
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "main.h"

/*
 * @description : 使能所有外设的时钟
 * @prarm       : 无
 * @return      : 无
 */
void  clock_enable (void)
{
    int i;
    for(i = 0; i <= 6; i++)
    CCM_CCGR(i) = 0xffffffff;
}

/*
 * @description : 使能GPIO
 * @prarm       : 无
 * @return      : 无
 */
void  led_enable (void)
{
    SW_MUX_GPIO1_IO03 = 0x5;
    SW_PAD_GPIO1_IO03 = 0x10b0;
    GPIO1_GDIR        = 0x8;
    GPIO1_DR          = 0x0;
}

/*
 * @description : 打开 LED
 * @prarm       : 无
 * @return      : 无
 */
void  led_on (void)
{
    GPIO1_DR &= ~(1 << 3);
}

/*
 * @description : 关闭 LED
 * @prarm       : 无
 * @return      : 无
 */
void  led_off (void)
{
    GPIO1_DR |= (1 << 3);
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