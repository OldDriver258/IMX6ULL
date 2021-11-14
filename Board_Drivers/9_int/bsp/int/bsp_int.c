/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : bsp_int.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 中断 函数
日志 : 初版 V1.0 2021/11/06 张宇辰创建
**************************************************************/
#include    "bsp_int.h"

/*  中断嵌套计数器  */
static unsigned int irqNesting;

/*  中断服务函数表  */
static sys_irq_handle_t  irqTable[NUMBER_OF_INT_VECTORS];

/*!
 * @brief       : 中断初始化函数
 * @param       : 无
 * @return      : 无
 */
void  int_init (void)
{
    GIC_Init();                         /*  初始化GIC          */
    system_irqtable_init();             /*  初始化中断表        */
    __set_VBAR((uint32_t)0x87800000);   /*  设置中断向量偏移    */
}

/*!
 * @brief       : 初始化中断服务函数
 * @param       : 无
 * @return      : 无
 */
void  system_irqtable_init (void)
{
    unsigned int    i = 0;
    irqNesting = 0;

    /*  先将所有的中断服务函数设置为默认值  */
    for (i = 0; i < NUMBER_OF_INT_VECTORS; i++) {
        system_register_irqhandler((IRQn_Type)i,
                                   default_irqhandler,
                                   NULL);
    }
}

/*!
 * @brief               : 给指定中断向量安装服务函数
 * @param   irq         : 要注册函数的中断号
 * @param   handler     : 要注册的中断处理函数
 * @param   userPrarm   : 中断服务函数的传递参数
 * @return              : 无
 */
void  system_register_irqhandler (IRQn_Type  irq,
                                  system_irq_handler_t handler,
                                  void  *userPrarm)
{
    irqTable[irq].irqHandler = handler;
    irqTable[irq].userParam  = userPrarm;
}

void  system_irqhandler (unsigned int giccIar)
{
    uint32_t    intNum = giccIar & 0x3FFUL;

    /*  检查中断号是否符合要求  */
    if ((intNum == 1020) || (intNum >= NUMBER_OF_INT_VECTORS)) {

        return;
    }

    irqNesting++;       /*  中断嵌套层数增加    */

    /*  调用中断服务函数    */
    irqTable[intNum].irqHandler(intNum, irqTable[intNum].userParam);

    irqNesting--;       /*  中断嵌套层数减少    */
}

/*!
 * @brief               : 默认中断服务函数
 * @param   giccIar     : 中断号
 * @param   userPrarm   : 中断服务函数参数
 * @return              : 无
 */
void  default_irqhandler (unsigned int  giccIar, void  *userPrarm)
{
    while(1) {}
}