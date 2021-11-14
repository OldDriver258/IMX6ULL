/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : bsp_exit.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 外部中断 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#include "bsp_exit.h"
#include "bsp_gpio.h"
#include "bsp_int.h"

void  exit_init (void)
{
    gpio_pin_config_t   key_config;

    /*  设置 IO 复用    */
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    /*  初始化GPIO为中断模式    */
    key_config.direction        = kGPIO_DigitalIntup;
    key_config.interruptMode    = kGPIO_IntFallingEdge;
    key_config.outputLogic      = 1;
    gpio_init(GPIO1, 18, &key_config);

    /*  使能GIC中断，注册中断服务函数，使能GPIO中断 */
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);

}