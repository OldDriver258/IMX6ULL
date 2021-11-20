/**************************************************************

文件名 : bsp_int.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 中断 函数
日志 : 初版 V1.0 2021/11/06 张宇辰创建
**************************************************************/
#ifndef _BSP_INT_H
#define _BSP_INT_H
#include "imx6ul.h"

typedef void  (*system_irq_handler_t) (unsigned int giccIar, void *param);

typedef struct _sys_irq_handle {
    system_irq_handler_t    irqHandler;     /*  中断处理函数        */
    void                    *userParam;     /*  中断处理函数参数    */
} sys_irq_handle_t;

void  int_init (void);
void  system_irqtable_init (void);
void  system_register_irqhandler (IRQn_Type  irq,
                                  system_irq_handler_t handler,
                                  void  *userPrarm);
void  system_irqhandler (unsigned int giccIar);
void  default_irqhandler (unsigned int  giccIar, void  *userPrarm);

#endif