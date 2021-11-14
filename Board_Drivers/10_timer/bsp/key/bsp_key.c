/**************************************************************

文件名 : bsp_key.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 按键 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"

/*!
 * @brief       : 初始化按键
 * @param       : 无
 * @return      : 无
 */
void  key_init (void)
{
    gpio_pin_config_t   key_config;

    /*  设置管脚复用    */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);

    /*  设置电气属性    */
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    key_config.direction = kGPIO_DigitalIntup;
    gpio_init(GPIO1, 18, &key_config);
}

/*!
 * @brief       : 初始化按键
 * @param       : 无
 * @return      : 按键是否触发
 */
int  key_getvalue (void)
{
    int     ret = 0;
    static uint8_t release = 1;

    if((release == 1) && (gpio_pinread(GPIO1, 18)) == 0) {
        delay(10);      /*  按键消抖    */
        release = 0;
        if ((gpio_pinread(GPIO1, 18)) == 0) {
            ret = KEY0_VALUE;
        }
    } else if ((gpio_pinread(GPIO1, 18)) == 1) {
        ret = 0;
        release = 1;
    }

    return  ret;
}