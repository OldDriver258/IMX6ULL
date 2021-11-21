/**************************************************************
文件名 : bsp_i2c.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : i2c 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_I2C_H
#define _BSP_I2C_H
#include "imx6ul.h"

#define I2C_STATUS_OK (0)
#define I2C_STATUS_BUSY (1)
#define I2C_STATUS_IDLE (2)
#define I2C_STATUS_NAK (3)
#define I2C_STATUS_ARBITRATIONLOST (4)
#define I2C_STATUS_TIMEOUT (5)
#define I2C_STATUS_ADDRNAK (6)

enum i2c_direction {
    kI2C_Write = 0x0,
    kI2C_Read  = 0x1,
};

struct i2c_transfer {
    unsigned char           slaveAddress;   //从机地址
    enum i2c_direction      direction;      //传输方向
    unsigned int            subaddress;     //寄存器地址
    unsigned char           subaddressSize; //寄存器长度
    volatile unsigned char *data;           //数据缓冲区
    volatile unsigned int   dataSize;       //数据缓冲区长度
};

void            i2c_init (I2C_Type  *base);
unsigned char   i2c_master_transfer (I2C_Type  *base, struct i2c_transfer  *xfer);

#endif