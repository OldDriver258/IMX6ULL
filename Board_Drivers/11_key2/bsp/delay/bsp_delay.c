/**************************************************************

文件名 : bsp_delay.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 延迟函数
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "bsp_delay.h"

/*!
 * @brief       : 短时间延时函数
 * @param n     : 循环次数
 * @return      : 无
 */
void  delay_short (volatile unsigned int n)
{
    while(n--){}
}

/*!
 * @brief       : 延时函数
 * @param ms    : 延时的 ms 数
 * @return      : 无
 */
void  delay (volatile unsigned int ms)
{
    while (ms--) {
        delay_short(0x7FF);
    }
}