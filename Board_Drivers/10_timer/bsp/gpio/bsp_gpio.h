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

typedef enum _gpio_interrupt_mode {
    kGPIO_NotIntMode                = 0U,       /*  无中断      */
    kGPIO_IntLowLevel               = 1U,       /*  低电平触发  */
    kGPIO_IntHighLevel              = 2U,       /*  高电平触发  */
    kGPIO_IntRisingEdge             = 3U,       /*  上升沿触发  */
    kGPIO_IntFallingEdge            = 4U,       /*  下降沿触发  */
    kGPIO_IntRisingOrFallingEdge    = 5U,       /*  上升沿和下降沿都触发    */
} gpio_interrupt_mode_t;

typedef struct _gpio_pin_config
{
    gpio_pin_direction_t    direction;      /*  GPIO方向        */
    uint8_t                 outputLogic;    /*  输出默认电平    */
    gpio_interrupt_mode_t   interruptMode;  /*  中断方式        */
} gpio_pin_config_t;

void  gpio_pinwrite (GPIO_Type  *base, int  pin, int  default_val);
void  gpio_init (GPIO_Type  *base, int  pin, gpio_pin_config_t  *config);
int  gpio_pinread (GPIO_Type  *base, int  pin);
void  gpio_intconfig (GPIO_Type  *base, unsigned int  pin, gpio_interrupt_mode_t  pin_int_mode);
void  gpio_enableint (GPIO_Type  *base, unsigned int pin);
void  gpio_disableint (GPIO_Type  *base, unsigned int pin);
void  gpio_clearintflags (GPIO_Type  *base, unsigned int pin);

#endif // !_BSP_GPIO_H
