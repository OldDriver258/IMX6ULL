/**************************************************************

文件名 : bsp_key.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 按键 函数
日志 : 初版 V1.0 2021/11/04 张宇辰创建
**************************************************************/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "imx6ul.h"

enum keyValue {
    KEY_NONE    = 0,
    KEY0_VALUE,
};

void  key_init (void);
int  key_getvalue (void);

#endif // !_BSP_KEY_H
