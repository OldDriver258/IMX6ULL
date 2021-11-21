/**************************************************************
文件名 : bsp_icm20608.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : icm  六轴传感器 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_ICM20608_H
#define _BSP_ICM20608_H
#include "imx6ul.h"
#include "bsp_gpio.h"
#include "bsp_spi.h"

#define ICM20608_CSN(n)     (n ? gpio_pinwrite(GPIO1, 20, 1) : gpio_pinwrite(GPIO1, 20, 0))

#define ICM20608G_ID        0xAF
#define ICM20608D_ID        0xAE

/*
 * ICM20608结构体
 */
struct icm20608_dev_struc
{
	signed int gyro_x_adc;		/* 陀螺仪X轴原始值 			*/
	signed int gyro_y_adc;		/* 陀螺仪Y轴原始值 			*/
	signed int gyro_z_adc;		/* 陀螺仪Z轴原始值 			*/
	signed int accel_x_adc;		/* 加速度计X轴原始值 			*/
	signed int accel_y_adc;		/* 加速度计Y轴原始值 			*/
	signed int accel_z_adc;		/* 加速度计Z轴原始值 			*/
	signed int temp_adc;		/* 温度原始值 				*/

	/* 下面是计算得到的实际值，扩大100倍 */
	signed int gyro_x_act;		/* 陀螺仪X轴实际值 			*/
	signed int gyro_y_act;		/* 陀螺仪Y轴实际值 			*/
	signed int gyro_z_act;		/* 陀螺仪Z轴实际值 			*/
	signed int accel_x_act;		/* 加速度计X轴实际值 			*/
	signed int accel_y_act;		/* 加速度计Y轴实际值 			*/
	signed int accel_z_act;		/* 加速度计Z轴实际值 			*/
	signed int temp_act;		/* 温度实际值 				*/
};
extern struct icm20608_dev_struc icm20608_dev;	/* icm20608设备 */

unsigned char  icm20608_init (void);
void  icm20608_getdata (void);

#endif