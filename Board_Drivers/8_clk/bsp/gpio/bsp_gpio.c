/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : bsp_gpio.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : GPIO 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#include "bsp_gpio.h"

/*!
 * @brief       : GPIO管脚输出对应的电平
 * @param base  : 对应GPIO组寄存器的基地址
 * @param pin   : 操作的GPIO管脚编号
 * @param default_val   : GPIO输出默认电平
 * @return      : 无
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
 * @brief       : GPIO初始化输入输出
 * @param base  : 对应GPIO组寄存器的基地址
 * @param pin   : 操作的GPIO管脚编号
 * @param config: GPIO管脚配置
 * @return      : 无
 */
void  gpio_init (GPIO_Type *base, int pin, gpio_pin_config_t *config)
{
    if (config->direction == kGPIO_DigitalIntup) {      /*  GPIO为输入  */
        base->GDIR &= ~(1 << pin);
    } else {                                            /*  GPIO为输出  */
        base->GDIR |= (1 << pin);
        gpio_pinwrite(base, pin, config->outputLogic);
    }
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