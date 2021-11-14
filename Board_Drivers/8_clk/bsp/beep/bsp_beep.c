/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : bsp_beep.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 蜂鸣器驱动
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/
#include "bsp_beep.h"

/*!
 * @brief       : 初始化蜂鸣器
 * @param       : 无
 * @return      : 无
 */
void  beep_init (void)
{
    /*1.初始化 IO 复用*/
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0);
    /*2.配置 IO 属性*/
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01,0X10B0);
    /*3.GPIO 方向为输出*/
    GPIO5->GDIR |= (1 << 1);
    /*4.输出高电平*/
    GPIO5->DR |= (1 << 1);
}

/*!
 * @brief       : 蜂鸣器控制函数，控制蜂鸣器打开还是关闭
 * @param       : 0，关闭蜂鸣器，1 打开蜂鸣器
 * @return      : 无
 */
void  beep_switch (int  status)
{ 
#define ON      1
#define OFF     0
    if(status == ON)
        GPIO5->DR &= ~(1 << 1); /* 打开蜂鸣器 */
    else if(status == OFF)
        GPIO5->DR |= (1 << 1); /* 关闭蜂鸣器 */
}