/**************************************************************

文件名 : bsp_gpio.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : GPIO 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#include "bsp_gpio.h"

/*!
 * @brief       : GPIO初始化输入输出
 * @param base  : 对应GPIO组寄存器的基地址
 * @param pin   : 操作的GPIO管脚编号
 * @param config: GPIO管脚配置
 * @return      : 无
 */
void  gpio_init (GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    base->IMR &= ~(1U << pin);                          /*  关闭中断掩码    */

    if (config->direction == kGPIO_DigitalIntup) {      /*  GPIO为输入      */
        base->GDIR &= ~(1 << pin);
    } else {                                            /*  GPIO为输出      */
        base->GDIR |= (1 << pin);
        gpio_pinwrite(base, pin, config->outputLogic);
    }
    gpio_intconfig(base, pin, config->interruptMode);
}

/*!
 * @brief       : 读取对于GPIO电平值
 * @param base  : 对应GPIO组寄存器的基地址
 * @param pin   : 操作的GPIO管脚编号
 * @return      : 无
 */
int  gpio_pinread (GPIO_Type *base, int pin)
{
    return  (((base->DR) >> pin) & 0x1);
}

/*!
 * @brief               : GPIO管脚输出对应的电平
 * @param base          : 对应GPIO组寄存器的基地址
 * @param pin           : 操作的GPIO管脚编号
 * @param default_val   : GPIO输出默认电平
 * @return              : 无
 */
void  gpio_pinwrite (GPIO_Type *base, int pin, int default_val)
{
    if (default_val == 0U) {            /*  输出高电平  */
        base->DR &= ~(1 << pin);
    } else {
        base->DR |= (1 << pin);         /*  输出低电平  */
    }
}

/*!
 * @brief                   : 配置GPIO中断
 * @param   base            : 要配置的GPIO组的基地址
 * @param   pin             : 要配置的GPIO号
 * @param   pin_int_mode    : 中断模式
 * @return                  : 无
 */
void  gpio_intconfig (GPIO_Type  *base, unsigned int  pin,
                      gpio_interrupt_mode_t  pin_int_mode)
{
    __IOM uint32_t *icr;
    uint32_t        icrShift;

    icrShift = pin;

    base->EDGE_SEL &= ~(1U << pin);

    if (pin < 16) {
        icr = &(base->ICR1);
    } else {
        icr = &(base->ICR2);
        icrShift -= 16;
    }

    switch (pin_int_mode)
    {
    case kGPIO_IntLowLevel:
        *icr &= ~(3U << (2 * icrShift));
        break;

    case kGPIO_IntHighLevel:
        *icr = (*icr & (~(3U << (2 * icrShift)))) | (1U << (2 * icrShift));
        break;

    case kGPIO_IntRisingEdge:
        *icr = (*icr & (~(3U << (2 * icrShift)))) | (2U << (2 * icrShift));
        break;

    case kGPIO_IntFallingEdge:
        *icr |= (3U << (2 * icrShift));
        break;

    case kGPIO_IntRisingOrFallingEdge:
        base->EDGE_SEL |= (1U << pin);
        break;

    default:
        break;
    }
}

/*!
 * @brief           : 使能对应中断功能
 * @param   base    : 要配置的GPIO组的基地址
 * @param   pin     : 要配置的GPIO号
 * @return          : 无
 */
void  gpio_enableint (GPIO_Type  *base, unsigned int pin)
{
    base->IMR |= (1 << pin);
}

/*!
 * @brief           : 禁能对应中断功能
 * @param   base    : 要配置的GPIO组的基地址
 * @param   pin     : 要配置的GPIO号
 * @return          : 无
 */
void  gpio_disableint (GPIO_Type  *base, unsigned int pin)
{
    base->IMR &= ~(1 << pin);
}

void  gpio_clearintflags (GPIO_Type  *base, unsigned int pin)
{
    base->ISR |= (1 << pin);
}