/**************************************************************

文件名 : bsp_exit.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 外部中断 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#include "bsp_exit.h"
#include "bsp_gpio.h"
#include "bsp_delay.h"
#include "bsp_int.h"
#include "bsp_beep.h"

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
    system_register_irqhandler(GPIO1_Combined_16_31_IRQn,
                              (system_irq_handler_t)gpio1_io18_irqhandle,
                               NULL);
    gpio_enableint(GPIO1, 18);
}

void  gpio1_io18_irqhandle (void)
{
    static unsigned char state = 0;

    delay(10);
    if (gpio_pinread(GPIO1, 18) == 0) {
        state = !state;
        beep_switch(state);
    }

    gpio_clearintflags(GPIO1, 18);
}