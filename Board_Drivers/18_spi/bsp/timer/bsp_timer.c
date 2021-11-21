/**************************************************************
文件名 : bsp_timer.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 硬件 timer 驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#include "bsp_timer.h"
#include "bsp_int.h"
#include "bsp_led.h"

/**
 * @brief 定时器 1 的初始化
 *
 * @param frac  定时器分频值
 * @param value 计数器重装载值
 */
void  epit_init (unsigned int  frac, unsigned int  value)
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
                              (system_irq_handler_t)epit1_irqhandler,
                               NULL);

    //使能定时器
    EPIT1->CR |= 1 << 0;
}

/**
 * @brief 定时器 1 的中断服务函数
 *
 */
void  epit1_irqhandler (void)
{
    static unsigned char state = 0;

    if (EPIT1->SR & (1 << 0)) {
        state = !state;
        led_switch(LED0, state);
    }
    EPIT1->SR |= (1 << 0);
}