/**************************************************************

文件名 : bsp_clk.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 系统时钟驱动
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "bsp_clk.h"

/*!
 * @brief       : 使能所有外设的时钟
 * @param       : 无
 * @return      : 无
 */
void  clk_enable (void)
{
    CCM->CCGR0 = 0xffffffff;
    CCM->CCGR1 = 0xffffffff;
    CCM->CCGR3 = 0xffffffff;
    CCM->CCGR4 = 0xffffffff;
    CCM->CCGR5 = 0xffffffff;
    CCM->CCGR6 = 0xffffffff;
}