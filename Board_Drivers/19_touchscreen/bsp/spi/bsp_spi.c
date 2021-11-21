/**************************************************************
文件名 : bsp_spi.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : spi 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#include "bsp_spi.h"

/**
 * @brief   初始化 SPI
 * @note
 * @param  *base: 要初始化的 SPI
 * @retval None
 */
void  spi_init (ECSPI_Type  *base)
{
    /* 配置 CONREG 寄存器
    * bit0 : 1 使能 ECSPI
    * bit3 : 1 当向 TXFIFO 写入数据以后立即开启 SPI 突发。
    * bit[7:4]： 0001 SPI 通道 0 主模式，根据实际情况选择，开发板上的
    * ICM-20608 接在 SS0 上，所以设置通道 0 为主模式
    * bit[19:18]:00 选中通道 0(其实不需要，因为片选信号我们我们自己控制)
    * bit[31:20]:0x7 突发长度为 8 个 bit。
    */
    base->CONREG = 0;
    base->CONREG |= (1 << 0) | (1 << 3) | (1 << 4) | (7 << 20);

    /*
    * ECSPI 通道 0 设置,即设置 CONFIGREG 寄存器
    * bit0: 0 通道 0 PHA 为 0
    * bit4: 0 通道 0 SCLK 高电平有效
    * bit8: 0 通道 0 片选信号 当 SMC 为 1 的时候此位无效
    * bit12: 0 通道 0 POL 为 0
    * bit16: 0 通道 0 数据线空闲时高电平
    * bit20: 0 通道 0 时钟线空闲时低电平
    */
    base->CONFIGREG = 0; /* 设置通道寄存器 */

    /*
    * ECSPI 通道 0 设置，设置采样周期
    * bit[14:0] : 0X2000 采样等待周期，比如当 SPI 时钟为 10MHz 的时候
    * 0X2000 就等于 1/10000 * 0X2000 = 0.8192ms，也就是
    * 连续读取数据的时候每次之间间隔 0.8ms
    * bit15 : 0 采样时钟源为 SPI CLK
    * bit[21:16]: 0 片选延时，可设置为 0~63
    */
    base->PERIODREG = 0X2000; /* 设置采样周期寄存器 */

    /*
    * ECSPI 的 SPI 时钟配置，SPI 的时钟源来源于 pll3_sw_clk/8=480/8=60MHz
    * SPI CLK = (SourceCLK / PER_DIVIDER) / (2^POST_DIVEDER)
    * 比如我们现在要设置 SPI 时钟为 6MHz，那么设置如下：
    * PER_DIVIDER = 0X9。
    * POST_DIVIDER = 0X0。
    * SPI CLK = 60000000/(0X9 + 1) = 60000000=6MHz
    */
    base->CONREG &= ~((0XF << 12) | (0XF << 8)); /* 清除以前的设置 */
    base->CONREG |= (0X9 << 12); /* 设置 SPI CLK = 6MHz */
}

/**
 * @brief  SPI 通道 0 发送/接收一个字节的数据
 * @note
 * @param  *base: 要使用的 SPI
 * @param  txdata: 要发送的数据
 * @retval 接收到的数据
 */
unsigned char  spich0_readwrite_byte (ECSPI_Type  *base, unsigned char txdata)
{
    uint32_t    spirxdata = 0;
    uint32_t    spitxdata = txdata;

    base->CONREG &= ~(3 << 18);
    base->CONREG |= (0 << 18);

    while ((base->STATREG & (1 << 0)) == 0){}
    base->TXDATA = spitxdata;

    while((base->STATREG & (1 << 3)) == 0){} /* 等待接收 FIFO 有数据 */
    spirxdata = base->RXDATA;

    return spirxdata;
}