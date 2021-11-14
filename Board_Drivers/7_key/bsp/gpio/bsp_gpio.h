/**************************************************************

文件名 : bsp_gpio.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : GPIO 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#ifndef _BSP_GPIO_H
#define _BSP_GPIO_H
#include "imx6ul.h"

typedef enum _gpio_pin_direction {
    kGPIO_DigitalIntup  = 0U,   /*  方向为输入  */
    kGPIO_DigitalOutput = 1U,   /*  方向为输出  */
} gpio_pin_direction_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t    direction;  /*  GPIO方向    */
    uint8_t                 outputLogic;/*  输出默认电平    */
} gpio_pin_config_t;

void gpio_pinwrite(GPIO_Type *base, int pin, int default_val);
void gpio_init(GPIO_Type *base, int pin, gpio_pin_config_t *config);
int gpio_pinread(GPIO_Type *base, int pin);

#endif // !_BSP_GPIO_H
