/**************************************************************
文件名 : bsp_icm20608.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : icm  六轴传感器 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#include "bsp_icm20608.h"
#include "bsp_spi.h"
#include "bsp_delay.h"
#include "bsp_lcdapi.h"

/* ICM20608寄存器
 *复位后所有寄存器地址都为0，除了
 *Register 107(0X6B) Power Management 1 	= 0x40
 *Register 117(0X75) WHO_AM_I 				= 0xAF或0xAE
 */
/* 陀螺仪和加速度自测(出产时设置，用于与用户的自检输出值比较） */
#define	ICM20_SELF_TEST_X_GYRO		0x00
#define	ICM20_SELF_TEST_Y_GYRO		0x01
#define	ICM20_SELF_TEST_Z_GYRO		0x02
#define	ICM20_SELF_TEST_X_ACCEL		0x0D
#define	ICM20_SELF_TEST_Y_ACCEL		0x0E
#define	ICM20_SELF_TEST_Z_ACCEL		0x0F

/* 陀螺仪静态偏移 */
#define	ICM20_XG_OFFS_USRH			0x13
#define	ICM20_XG_OFFS_USRL			0x14
#define	ICM20_YG_OFFS_USRH			0x15
#define	ICM20_YG_OFFS_USRL			0x16
#define	ICM20_ZG_OFFS_USRH			0x17
#define	ICM20_ZG_OFFS_USRL			0x18

#define	ICM20_SMPLRT_DIV			0x19
#define	ICM20_CONFIG				0x1A
#define	ICM20_GYRO_CONFIG			0x1B
#define	ICM20_ACCEL_CONFIG			0x1C
#define	ICM20_ACCEL_CONFIG2			0x1D
#define	ICM20_LP_MODE_CFG			0x1E
#define	ICM20_ACCEL_WOM_THR			0x1F
#define	ICM20_FIFO_EN				0x23
#define	ICM20_FSYNC_INT				0x36
#define	ICM20_INT_PIN_CFG			0x37
#define	ICM20_INT_ENABLE			0x38
#define	ICM20_INT_STATUS			0x3A

/* 加速度输出 */
#define	ICM20_ACCEL_XOUT_H			0x3B
#define	ICM20_ACCEL_XOUT_L			0x3C
#define	ICM20_ACCEL_YOUT_H			0x3D
#define	ICM20_ACCEL_YOUT_L			0x3E
#define	ICM20_ACCEL_ZOUT_H			0x3F
#define	ICM20_ACCEL_ZOUT_L			0x40

/* 温度输出 */
#define	ICM20_TEMP_OUT_H			0x41
#define	ICM20_TEMP_OUT_L			0x42

/* 陀螺仪输出 */
#define	ICM20_GYRO_XOUT_H			0x43
#define	ICM20_GYRO_XOUT_L			0x44
#define	ICM20_GYRO_YOUT_H			0x45
#define	ICM20_GYRO_YOUT_L			0x46
#define	ICM20_GYRO_ZOUT_H			0x47
#define	ICM20_GYRO_ZOUT_L			0x48

#define	ICM20_SIGNAL_PATH_RESET		0x68
#define	ICM20_ACCEL_INTEL_CTRL 		0x69
#define	ICM20_USER_CTRL				0x6A
#define	ICM20_PWR_MGMT_1			0x6B
#define	ICM20_PWR_MGMT_2			0x6C
#define	ICM20_FIFO_COUNTH			0x72
#define	ICM20_FIFO_COUNTL			0x73
#define	ICM20_FIFO_R_W				0x74
#define	ICM20_WHO_AM_I 				0x75

/* 加速度静态偏移 */
#define	ICM20_XA_OFFSET_H			0x77
#define	ICM20_XA_OFFSET_L			0x78
#define	ICM20_YA_OFFSET_H			0x7A
#define	ICM20_YA_OFFSET_L			0x7B
#define	ICM20_ZA_OFFSET_H			0x7D
#define	ICM20_ZA_OFFSET_L 			0x7E

/**
 * @brief  写 ICM20608 指定寄存器
 * @note
 * @param  reg: 要读取的寄存器地址
 * @param  value: 要写入的值
 * @retval None
 */
static void  icm20608_write_reg (unsigned char  reg, unsigned char  value)
{
    reg &= ~0x80;

    ICM20608_CSN(0);
    spich0_readwrite_byte(ECSPI3, reg);
    spich0_readwrite_byte(ECSPI3, value);
    ICM20608_CSN(1);
}

/**
 * @brief   读取 ICM20608 寄存器值
 * @note
 * @param  reg: 要读取的寄存器地址
 * @retval 读取到的寄存器值
 */
static unsigned char  icm20608_read_reg (unsigned char  reg)
{
    unsigned char  reg_val;

    reg |= 0x80;

    ICM20608_CSN(0);
    spich0_readwrite_byte(ECSPI3, reg);
    reg_val = spich0_readwrite_byte(ECSPI3, 0xFF);
    ICM20608_CSN(1);

    return  reg_val;
}

unsigned char  icm20608_init (void)
{
    unsigned char       regvalue;
    gpio_pin_config_t   cs_config;

    IOMUXC_SetPinMux(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0);
    IOMUXC_SetPinMux(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RX_DATA_ECSPI3_SCLK, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_CTS_B_ECSPI3_MOSI, 0x10B1);
    IOMUXC_SetPinConfig(IOMUXC_UART2_RTS_B_ECSPI3_MISO, 0x10B1);

    /* 初始化片选引脚 */
    IOMUXC_SetPinMux(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART2_TX_DATA_GPIO1_IO20, 0X10B0);
    cs_config.direction = kGPIO_DigitalOutput;
    cs_config.outputLogic = 0;
    gpio_init(GPIO1, 20, &cs_config);

    spi_init(ECSPI3);

    icm20608_write_reg(ICM20_PWR_MGMT_1, 0x80);
    delayms(50);
    icm20608_write_reg(ICM20_PWR_MGMT_1, 0x01);
    delayms(50);

    regvalue = icm20608_read_reg(ICM20_WHO_AM_I);
    lcd_printf("icm20608 id = %#X\r\n", regvalue);
    if ((regvalue != ICM20608G_ID) && (regvalue != ICM20608D_ID))
        return  1;

    icm20608_write_reg(ICM20_SMPLRT_DIV, 0x00);/* 输出速率设置 */
    icm20608_write_reg(ICM20_GYRO_CONFIG, 0x18); /* 陀螺仪±2000dps */
    icm20608_write_reg(ICM20_ACCEL_CONFIG, 0x18); /* 加速度计±16G */
    icm20608_write_reg(ICM20_CONFIG, 0x04); /* 陀螺 BW=20Hz */
    icm20608_write_reg(ICM20_ACCEL_CONFIG2, 0x04);
    icm20608_write_reg(ICM20_PWR_MGMT_2, 0x00); /* 打开所有轴 */
    icm20608_write_reg(ICM20_LP_MODE_CFG, 0x00); /* 关闭低功耗 */
    icm20608_write_reg(ICM20_FIFO_EN, 0x00); /* 关闭 FIFO */

    return 0;
}

/**
 * @brief  读取 ICM20608 连续多个寄存器
 * @note
 * @param  reg: 要读取的寄存器地址
 * @param  *buf:  读取到的寄存器值
 * @param  len: 读取数据长度
 * @retval None
 */
static void  icm20608_read_len (u8 reg, u8 *buf, u8 len)
{
    u8  i;

    reg |= 0x80;

    ICM20608_CSN(0);
    spich0_readwrite_byte(ECSPI3, reg);
    for (i = 0; i < len; i++) {
        buf[i] = spich0_readwrite_byte(ECSPI3, 0xFF);
    }
    ICM20608_CSN(1);
}

/**
 * @brief  获取陀螺仪的分辨率
 * @note
 * @retval 获取到的分辨率
 */
static float  icm20608_gyro_scaleget (void)
{
    u8      data;
    float   gyroscale;

    data = (icm20608_read_reg(ICM20_GYRO_CONFIG) >> 3) & 0x3;
    switch (data) {
    case 0:
        gyroscale = 131;
        break;

    case 1:
        gyroscale = 65.5;
        break;

    case 2:
        gyroscale = 32.8;
        break;

    case 3:
        gyroscale = 16.4;
        break;

    default:
        break;
    }

    return  gyroscale;
}

/**
 * @brief  获取加速度计的分辨率
 * @note
 * @retval 获取到的分辨率
 */
static unsigned short  icm20608_accel_scaleget (void) {
    unsigned char data;
    unsigned short accelscale;

    data = (icm20608_read_reg(ICM20_ACCEL_CONFIG) >> 3) & 0X3;
    switch (data) {
    case 0:
        accelscale = 16384;
        break;
    case 1:
        accelscale = 8192;
        break;
    case 2:
        accelscale = 4096;
        break;
    case 3:
        accelscale = 2048;
        break;
    }

    return accelscale;
}

struct icm20608_dev_struc icm20608_dev;	/* icm20608设备 */
/**
 * @brief  获得 icm20608 传感器数据
 * @note
 * @retval None
 */
void  icm20608_getdata (void)
{
    float           gyroscale;
    unsigned short  accescale;
    unsigned char   data[14];

    icm20608_read_len(ICM20_ACCEL_XOUT_H, data, 14);

    gyroscale = icm20608_gyro_scaleget();
    accescale = icm20608_accel_scaleget();

    //原始数据
    icm20608_dev.accel_x_adc = (s16)((data[0] << 8) | (data[1]));
    icm20608_dev.accel_y_adc = (s16)((data[2] << 8) | (data[3]));
    icm20608_dev.accel_z_adc = (s16)((data[4] << 8) | (data[5]));

    icm20608_dev.temp_adc    = (s16)((data[6] << 8) | (data[7]));

    icm20608_dev.gyro_x_adc  = (s16)((data[8] << 8) | (data[9]));
    icm20608_dev.gyro_y_adc  = (s16)((data[10] << 8) | (data[11]));
    icm20608_dev.gyro_z_adc  = (s16)((data[12] << 8) | (data[13]));

    //实际数据
    icm20608_dev.accel_x_act = ((float)(icm20608_dev.accel_x_adc) / accescale) * 100;
    icm20608_dev.accel_y_act = ((float)(icm20608_dev.accel_y_adc) / accescale) * 100;
    icm20608_dev.accel_z_act = ((float)(icm20608_dev.accel_z_adc) / accescale) * 100;

    icm20608_dev.temp_act   = (((float)(icm20608_dev.temp_adc) - 25) / 326.8 + 25) * 100;

    icm20608_dev.gyro_x_act = ((float)(icm20608_dev.gyro_x_adc) / gyroscale)  * 100;
    icm20608_dev.gyro_y_act = ((float)(icm20608_dev.gyro_y_adc) / gyroscale)  * 100;
    icm20608_dev.gyro_z_act = ((float)(icm20608_dev.gyro_z_adc) / gyroscale)  * 100;
}