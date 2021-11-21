/**************************************************************
文件名 : bsp_i2c.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : i2c 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"

/**
 * @brief  初始化 I2C 和时钟
 * @note
 * @param  *base: I2Cx 基地址
 * @retval None
 */
void  i2c_init (I2C_Type  *base)
{
    //先关闭 I2C
    base->I2CR &= ~(1 << 7);

    /* 设置波特率为 100K
    * I2C 的时钟源来源于 IPG_CLK_ROOT=66Mhz
    * IFDR 设置为 0X15 ，也就是 640 分频，
    * IFDR 设置为 0X0D ，也就是 160 分频，
    * IFDR 设置为 0X0E ，也就是 192 分频，
    * 66000000 / 640 = 103.125  KHz≈100KHz。
    * 66000000 / 160 = 412.5    kHz
    * 66000000 / 192 = 343.75   kHz
    */
    base->IFDR = 0X0D << 0;

    base->I2CR |= (1 << 7);
}

/**
 * @brief  重新发送开始信号
 * @note
 * @param  *base: 使用的 I2C 地址
 * @param  address: 设备地址
 * @param  direction: 传输方向
 * @retval 0 正常， 其他值错误
 */
static unsigned char  i2c_master_repeated_start (I2C_Type           *base,
                                          unsigned char       address,
                                          enum i2c_direction  direction)
{
    /* I2C 忙并且工作在从模式,跳出 */
    if (base->I2SR & (1 << 5) && ((base->I2CR & (1 << 5)) == 0))
        return  1;

    base->I2CR |= (1 << 4) | (1 << 2);

    base->I2DR = ((unsigned int)address << 1) |
                 ((direction == kI2C_Read) ? 1 : 0);

    return  0;
}

/**
 * @brief  发送开始信号
 * @note
 * @param  *base: 使用的 I2C 地址
 * @param  address: 设备地址
 * @param  direction: 传输方向
 * @retval 0 正常， 其他值错误
 */
static unsigned char  i2c_master_start  (I2C_Type           *base,
                                  unsigned char       address,
                                  enum i2c_direction  direction)
{
    if(base->I2SR & (1 << 5)) /* I2C 忙 */
        return  1;

    base->I2CR |= (1 << 5) | (1 << 4);

    base->I2DR = ((unsigned int)address << 1) |
                 ((direction == kI2C_Read) ? 1 : 0);

    return  0;
}

/**
 * @brief  检查并清除错误
 * @note
 * @param  *base: 使用的 I2C 地址
 * @param  status: 状态
 * @retval 状态结果
 */
static unsigned char  i2c_check_and_clear_error  (I2C_Type  *base, unsigned int  status)
{
    if (status & (1 << 4)) {            /* 检查是否发生仲裁丢失错误 */
        base->I2SR &= ~(1<<4);          /* 清除仲裁丢失错误位 */
        base->I2CR &= ~(1 << 7);        /* 先关闭 I2C */
        base->I2CR |= (1 << 7);         /* 重新打开 I2C */
        return I2C_STATUS_ARBITRATIONLOST;
    } else if (status & (1 << 0)) {     /* 没有接收到从机的应答信号 */
        return I2C_STATUS_NAK;          /* 返回 NAK(No acknowledge) */
    }
    return I2C_STATUS_OK;
}

/**
 * @brief   停止信号
 * @note
 * @param  *base: 要使用的 IIC
 * @retval 状态结果
 */
static unsigned char  i2c_master_stop  (I2C_Type  *base)
{
    unsigned short  timeout = 0xFFFF;

    base->I2CR &= ~((1 << 5) | (1 << 4) | (1 << 3));
    while ((base->I2SR & (1 << 5))) {
        timeout--;
        if (timeout == 0) {
            return  I2C_STATUS_TIMEOUT;
        }
    }

    return  I2C_STATUS_OK;
}

/**
 * @brief  发送数据
 * @note
 * @param  *base: 要使用的 IIC
 * @param  *buf: 要发送的数据
 * @param  size: 要发送的数据大小
 * @retval None
 */
static void  i2c_master_write (I2C_Type  *base, volatile unsigned char  *buf, unsigned int  size)
{
    while(!(base->I2SR & (1 << 7))); /* 等待传输完成 */
    base->I2SR &= ~(1 << 1); /* 清除标志位 */
    base->I2CR |= 1 << 4; /* 发送数据 */
    while(size--) {
        base->I2DR = *buf++; /* 将 buf 中的数据写入到 I2DR 寄存器 */
        while(!(base->I2SR & (1 << 1))); /* 等待传输完成 */
        base->I2SR &= ~(1 << 1); /* 清除标志位 */

        /* 检查 ACK */
        if(i2c_check_and_clear_error(base, base->I2SR))
            break;

        base->I2SR &= ~(1 << 1);
        i2c_master_stop(base); /* 发送停止信号 */
    }
}

/**
 * @brief  读取数据
 * @note
 * @param  *base: 要使用的 IIC
 * @param  *buf: 读取到数据
 * @param  size:  要读取的数据大小
 * @retval None
 */
static void  i2c_master_read (I2C_Type  *base, volatile unsigned char  *buf, unsigned int  size)
{
    volatile uint8_t dummy = 0;

    while(!(base->I2SR & (1 << 7)));        /* 等待传输完成 */
    base->I2SR &= ~(1 << 1);                /* 清除中断挂起位 */
    base->I2CR &= ~((1 << 4) | (1 << 3));   /* 接收数据 */
    if (size == 1)
        base->I2CR |= (1 << 3);

    dummy++;
    dummy = base->I2DR;

    while (size--) {
        while (!(base->I2SR & (1 << 1)));

        base->I2SR &= ~(1 << 1); /* 清除标志位 */

        if(size == 0)
            i2c_master_stop(base); /* 发送停止信号 */
        if(size == 1)
            base->I2CR |= (1 << 3);

        *(buf++) = base->I2DR;
    }
}

unsigned char  i2c_master_transfer (I2C_Type  *base, struct i2c_transfer  *xfer)
{
    unsigned char ret = 0;
    enum i2c_direction direction = xfer->direction;

    base->I2SR &= ~((1 << 1) | (1 << 4));
    while(!((base->I2SR >> 7) & 0X1)){}; /* 等待传输完成 */

    if ((xfer->subaddressSize > 0) && (xfer->direction == kI2C_Read)) {
        direction = kI2C_Write;
    }

    ret = i2c_master_start(base, xfer->slaveAddress, direction);
    if (ret) {
        return  ret;
    }
    while(!(base->I2SR & (1 << 1))){};      /* 等待传输完成 */

    ret = i2c_check_and_clear_error(base, base->I2SR);
    if (ret) {
        i2c_master_stop(base);
        return  ret;
    }

    if (xfer->subaddressSize) {
        do {
            base->I2SR &= ~(1 << 1);        /* 清除标志位 */
            xfer->subaddressSize--;
            base->I2DR = ((xfer->subaddress) >> (8 * xfer->subaddressSize));
            while (!(base->I2SR & (1 << 1)));

            ret = i2c_check_and_clear_error(base, base->I2SR);
            if (ret) {
                i2c_master_stop(base);
                return  ret;
            }
        } while ((xfer->subaddressSize > 0) && (ret == I2C_STATUS_OK));

        if (xfer->direction == kI2C_Read) {
            base->I2SR &= ~(1 << 1);
            i2c_master_repeated_start(base, xfer->slaveAddress, kI2C_Read);
            while(!(base->I2SR & (1 << 1))){};

            ret = i2c_check_and_clear_error(base, base->I2SR);
            if (ret) {
                ret = I2C_STATUS_ADDRNAK;
                i2c_master_stop(base);
                return  ret;
            }
        }
    }

    if ((xfer->direction == kI2C_Write) && (xfer->dataSize > 0))
        i2c_master_write(base, xfer->data, xfer->dataSize);

    if ((xfer->direction == kI2C_Read) && (xfer->dataSize > 0))
        i2c_master_read(base, xfer->data, xfer->dataSize);

    return  0;
}