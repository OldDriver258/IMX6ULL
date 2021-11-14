/**************************************************************
文件名 : bsp_uart.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : uart 驱动
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#include "bsp_uart.h"

/**
 * @brief  串口的 IO 初始化
 * @note
 * @retval None
 */
static void  uart_io_init (void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
}

/**
 * @brief  串口使能
 * @note
 * @param  *base: 对应串口的寄存器基地址
 * @retval None
 */
static void  uart_disable (UART_Type  *base)
{
    base->UCR1 &= ~(1 << 0);
}

/**
 * @brief  串口禁能
 * @note
 * @param  *base: 对应串口的寄存器基地址
 * @retval None
 */
static void  uart_enable (UART_Type  *base)
{
    base->UCR1 |= (1 << 0);
}

/**
 * @brief  串口软件复位
 * @note
 * @param  *base: 对应串口的寄存器基地址
 * @retval None
 */
static void  uart_softreset (UART_Type  *base)
{
    base->UCR2 &= ~(1<<0);
    while((base->UCR2 & 0x1) == 0);
}

/**
 * @brief  串口初始化
 * @note
 * @retval None
 */
void  uart_init (void)
{
    uart_io_init();

    uart_disable(UART1);
    uart_softreset(UART1);

    UART1->UCR1 = 0;

    //设置数据格式， 发送接收使能
    UART1->UCR1 &= ~(1 << 14);
    UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1);
    UART1->UCR3 |= 1<<2;

    //设置波特率
    /*
     * 因此波特率= 80000000/(16 * (3124+1)/(71+1))
     * = 80000000/(16 * 3125/72)
     * = (80000000*72) / (16*3125)
     * = 115200
     */
    UART1->UFCR = 5<<7; /* ref freq 等于 ipg_clk/1=80Mhz */
    UART1->UBIR = 71;
    UART1->UBMR = 3124;

    uart_enable(UART1);
}

/**
 * @brief  串口发送一个字符
 * @note
 * @param  c: 发送的字符
 * @retval None
 */
void  putc (unsigned char c)
{
    while (((UART1->USR2 >> 3) & 0x01) == 0);
    UART1->UTXD = c & 0XFF;
}

/**
 * @brief  串口发送字符串
 * @note
 * @param  *str: 发送的字符串
 * @retval None
 */
void  puts (char  *str)
{
    char *p = str;

    while (*p) {
        putc(*p++);
    }
}

/**
 * @brief  串口获得字符
 * @note
 * @retval 串口得到的字符
 */
unsigned char  getc (void)
{
    while((UART1->USR2 & 0x1) == 0);
    return UART1->URXD;
}