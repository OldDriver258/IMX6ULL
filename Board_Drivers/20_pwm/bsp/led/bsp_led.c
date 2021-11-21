/**************************************************************

文件名 : bsp_led.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : led 驱动
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/

#include "bsp_led.h"

/*!
 * @brief       : 初始化 LED 及对应 GPIO
 * @param       : 无
 * @return      : 无
 */
void led_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0X10B0);
    GPIO1->GDIR                 = 0x8;
    GPIO1->DR                   = 0x0;
    GPIO1->DR                  |= (1 << 3);
}

/*!
 * @brief       : 控制 LED 打开关闭
 * @param led   : 控制 LED 编号
 * @param tatus : 打开关闭状态
 * @return      : 无
 */
void led_switch(int led, int status)
{
#define ON  1
#define OFF 0
    switch (led) {
    case LED0:
        if (status == ON)
            GPIO1->DR &= ~(1 << 3);
        else if (status == OFF)
            GPIO1->DR |= (1 << 3);
        break;
    default:
        break;
    }
}