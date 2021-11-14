/**************************************************************

文件名 : bsp_delay.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 延迟函数
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "bsp_delay.h"

void  delay_init (void)
{
    GPT1->CR = 0;
    GPT1->CR = 1 << 15;
    while((GPT1->CR >> 15) & 0x01);

    GPT1->CR = (1 << 6);

    GPT1->PR = 65;  //66 分频 66000000/66=1000000

    GPT1->OCR[0] = 0xFFFFFFFF;
}

void  delayus (unsigned int  usdelay)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;

    oldcnt = GPT1->CNT;
    while (1) {
        newcnt = GPT1->CNT;
        if (newcnt != oldcnt) {
            if (newcnt > oldcnt) {
                tcntvalue += newcnt - oldcnt;
            } else {
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;
            }
            oldcnt = newcnt;
            //延时时间到了退出
            if (tcntvalue >= usdelay)
                break;
        }
    }
}

void  delayms (unsigned int  msdelay)
{
    int i = 0;
    for (i = 0; i < msdelay; i++) {
        delayus(1000);
    }
}

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