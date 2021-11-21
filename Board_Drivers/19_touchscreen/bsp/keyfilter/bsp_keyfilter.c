/**************************************************************
文件名 : bsp_keyfilter.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 硬件 timer 延时消抖按键驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#include "bsp_keyfilter.h"
#include "bsp_int.h"
#include "bsp_gpio.h"
#include "bsp_beep.h"

/**
 * @brief  定时器启动
 * @note
 * @retval None
 */
static void  filtertimer_start (void)
{
    //使能定时器
    EPIT1->CR |= 1 << 0;
}

/**
 * @brief  定时器关闭
 * @note
 * @retval None
 */
static void  filtertimer_stop (void)
{
    //使能定时器
    EPIT1->CR &= ~(1 << 0);
    EPIT1->CNR = EPIT1->LR;
}

/**
 * @brief  GPIO 中断服务函数
 * @note
 * @retval None
 */
static void  gpio1_16_31_irqhandler (void)
{
    filtertimer_start();
    gpio_clearintflags(GPIO1, 18);
}

/**
 * @brief  定时器中断服务函数
 * @note
 * @retval None
 */
static void  filtertimer_irqhandler (void)
{
    static unsigned char state = 1;
    static unsigned char cnt = 0;

    if (EPIT1->SR & (1 << 0)) {
        cnt++;
        if (cnt == 10) {
            cnt = 0;
            filtertimer_stop();
            if (gpio_pinread(GPIO1, 18) == 0) {
                state = !state;
                beep_switch(state);
            }
        }
    }

    EPIT1->SR |= (1 << 0);
}

/**
 * @brief  消抖用定时器初始化
 * @note
 * @param  frac: 定时器分频值
 * @param  value: 定时器重装载值
 * @retval None
 */
static void  filtertimer_init (unsigned int  frac, unsigned int  value)
{
    if (frac > 0xFFF)
    frac = 0xFFF;

    EPIT1->CR = 0;

    EPIT1->CR = (1<<24 | (frac - 1) << 4 | 1<<3 | 1<<2 | 1<<1);
    EPIT1->LR = value - 1; /* 加载寄存器值 */
    EPIT1->CMPR = 0; /* 比较寄存器值 */

    //使能对应 GIC 中断
    GIC_EnableIRQ(EPIT1_IRQn);

    //注册中断服务函数
    system_register_irqhandler(EPIT1_IRQn,
                              (system_irq_handler_t)filtertimer_irqhandler,
                               NULL);
}

/**
 * @brief  按键和定时器消抖初始化
 * @note
 * @retval None
 */
void  filterkey_init (void)
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
                              (system_irq_handler_t)gpio1_16_31_irqhandler,
                               NULL);
    gpio_enableint(GPIO1, 18);
    //66000000/3300/20 = 1000
    filtertimer_init(3300, 20);
}
