/**************************************************************
文件名 : bsp_ap3216c.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : ap3216c 环境传感器 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#include "bsp_ap3216c.h"
#include "bsp_i2c.h"
#include "bsp_delay.h"
#include "stdio.h"

/* AP3316C 寄存器 */
#define AP3216C_SYSTEMCONG  0x00 /* 配置寄存器 */
#define AP3216C_INTSTATUS   0X01 /* 中断状态寄存器 */
#define AP3216C_INTCLEAR    0X02 /* 中断清除寄存器 */
#define AP3216C_IRDATALOW   0x0A /* IR 数据低字节 */
#define AP3216C_IRDATAHIGH  0x0B /* IR 数据高字节 */
#define AP3216C_ALSDATALOW  0x0C /* ALS 数据低字节 */
#define AP3216C_ALSDATAHIGH 0X0D /* ALS 数据高字节 */
#define AP3216C_PSDATALOW   0X0E /* PS 数据低字节 */
#define AP3216C_PSDATAHIGH  0X0F /* PS 数据高字节 */

/**
 * @brief  向 AP3216C 写入数据
 * @note
 * @param  addr: 设备地址
 * @param  reg: 要写入的寄存器
 * @param  data: 要写入的数据
 * @retval 操作结果
 */
static unsigned char  ap3216c_writeonebyte (unsigned char  addr,
                                            unsigned char  reg,
                                            unsigned char  data)
{
    unsigned char status;
    unsigned char writedata = data;
    struct i2c_transfer masterXfer;

    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Write;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = &writedata;
    masterXfer.dataSize         = 1;

    if (i2c_master_transfer(I2C1, &masterXfer))
        status = 1;

    return  status;
}

/**
 * @brief  从 AP3216C 读取一个字节的数据
 * @note
 * @param  addr: 设备地址
 * @param  reg: 要读取的寄存器
 * @retval 读取到的数据
 */
static unsigned char ap3216c_readonebyte (unsigned char  addr,
                                          unsigned char  reg)
{
    unsigned char val = 0;

    struct i2c_transfer masterXfer;
    masterXfer.slaveAddress     = addr;
    masterXfer.direction        = kI2C_Read;
    masterXfer.subaddress       = reg;
    masterXfer.subaddressSize   = 1;
    masterXfer.data             = &val;
    masterXfer.dataSize         = 1;
    i2c_master_transfer(I2C1, &masterXfer);

    return  val;
}

/**
 * @brief  读取 AP3216C 的原始数据，包括 ALS,PS 和 IR, 注意！如果同时打开 ALS,IR+PS 两次数据读取的时间间隔要大于 112.5ms
 * @note
 * @param  *ir: ir 数据
 * @param  *ps: ps 数据
 * @param  *als: als 数据
 * @retval None
 */
void  ap3216c_read (unsigned short  *ir, unsigned short  *ps, unsigned short  *als)
{
    unsigned char buf[6];
    unsigned char i;

    for (i = 0; i < 6; i++) {
        buf[i] = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_IRDATALOW + i);
    }

    if (buf[0] & 0x80)
        *ir = 0;
    else
        *ir = ((unsigned short)buf[1] << 2) | (buf[0] & 0x03);

    *als = ((unsigned short)buf[3] << 8) | buf[2];

    if (buf[4] & 0x40)
        *ps = 0;
    else
        *ps = ((unsigned short)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F);
}

/**
 * @brief   初始化 AP3216C
 * @note
 * @retval 0 成功，其他值 错误代码
 */
unsigned char  ap3216c_init (void)
{
    unsigned char data;

    IOMUXC_SetPinMux(IOMUXC_UART4_TX_DATA_I2C1_SCL, 1);
    IOMUXC_SetPinMux(IOMUXC_UART4_RX_DATA_I2C1_SDA, 1);
    IOMUXC_SetPinConfig(IOMUXC_UART4_TX_DATA_I2C1_SCL, 0x70B0);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RX_DATA_I2C1_SDA, 0X70B0);

    i2c_init(I2C1);

    /*  复位    */
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0x04);
    delayms(50); /* AP33216C 复位至少 10ms */

    /* 开启 ALS、PS+IR */
    ap3216c_writeonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG, 0X03);

    /* 读取刚刚写进去的 0X03 */
    data = ap3216c_readonebyte(AP3216C_ADDR, AP3216C_SYSTEMCONG);
    if (data == 0x03) {
        return  0;
    } else {
        return  1;
    }
}