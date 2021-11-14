/**************************************************************
Copyright © zuozhongkai Co., Ltd. 1998-2019. All rights reserved.
文件名 : led.s
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : 裸机实验 1 汇编点灯
	使用汇编来点亮开发板上的 LED 灯，学习和掌握如何用汇编语言来
	完成对 I.MX6U 处理器的 GPIO 初始化和控制。
日志 : 初版 V1.0 2021/10/31 张宇辰创建
**************************************************************/

.global  _start /* 全局标号 */

_start:
    /* 
     * 使能外设时钟
     */
    ldr r0, =0x020c4068 @CCGR0
    ldr r1, =0xffffffff @要向CCGR0写入的数据
    str r1, [r0]        @将0xffffffff写入到CCGR0

    @ ldr r0, =0X020C406C /* 寄存器 CCGR1 */
    @ str r1, [r0]

    @ ldr r0, =0X020C4070 /* 寄存器 CCGR2 */
    @ str r1, [r0]

    @ ldr r0, =0X020C4074 /* 寄存器 CCGR3 */
    @ str r1, [r0]

    @ ldr r0, =0X020C4078 /* 寄存器 CCGR4 */
    @ str r1, [r0]

    @ ldr r0, =0X020C407C /* 寄存器 CCGR5 */
    @ str r1, [r0]

    @ ldr r0, =0X020C4080 /* 寄存器 CCGR6 */
    @ str r1, [r0]

    add r0, r0, #0x4    @CCGR1
    str r1, [r0]

    add r0, r0 , #0x4  	@CCGR2
    str r1, [r0]

    add r0, r0 , #0x4  	@CCGR3
    str r1, [r0]

    add r0, r0 , #0x4  	@CCGR4
    str r1, [r0]

    add r0, r0 , #0x4  	@CCGR5
    str r1, [r0]

    add r0, r0 , #0x4  	@CCGR6
    str r1, [r0]

    /*
     * 使能引脚复用
     */
    ldr r0, =0x020e0068 @IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03
    ldr r1, =0x5        @要写入的数据
    str r1, [r0]

    /*
     * 设置电气属性
     */
    ldr r0, =0x020e02f4 @IOMUXC_SW_MUX_CTL_PAD_GPIO1_IO03
    ldr r1, =0x10b0
    str r1, [r0]

    /*
     * GPIO 设置
     */
    ldr r0, =0x0209c004 @GPIO1_GDIR
    ldr r1, =0x8
    str r1, [r0]

    /*
     * 打开 LED
     */
    ldr r0, =0x0209c000 @GPIO1_DR
    ldr r1, =0x0        @输出低电平
    str r1, [r0]

loop:
    b loop


