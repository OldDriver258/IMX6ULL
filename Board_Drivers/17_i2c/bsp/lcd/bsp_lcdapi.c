/**************************************************************
文件名 : bsp_lcdapi.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : LCD API 函数接口
日志 : 初版 V1.0 2021/11/14 张宇辰创建
**************************************************************/
#include "bsp_lcdapi.h"
#include "bsp_lcd.h"
#include "bsp_delay.h"
#include "font.h"
#include "vsprintf.h"
#include "stdio.h"

/**
 * @brief  画线函数
 * @note
 * @param  x1: 线起始点坐标X轴
 * @param  y1: 线起始点坐标Y轴
 * @param  x2: 线终止点坐标X轴
 * @param  y2: 线终止点坐标Y轴
 * @retval None
 */
void lcd_drawline(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	u16 t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; 					/* 计算坐标增量 */
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if(delta_x > 0) 					/* 设置单步方向 */
		incx = 1;
	else if(delta_x==0)					/* 垂直线 */
		incx = 0;
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if(delta_y>0)
		incy=1;
	else if(delta_y == 0)				/* 水平线 */
		incy=0;
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if( delta_x > delta_y)				/*选取基本增量坐标轴  */
		distance = delta_x;
	else
		distance = delta_y;
	for(t = 0; t <= distance+1; t++ )	/* 画线输出 */
	{

		lcd_drawpoint(uRow, uCol, tftlcd_dev.forecolor);/* 画点 */
		xerr += delta_x ;
		yerr += delta_y ;
		if(xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

/**
 * @brief  画矩形函数
 * @note
 * @param  x1: 矩形坐上角坐标X轴
 * @param  y1: 矩形坐上角坐标Y轴
 * @param  x2: 矩形右下角坐标X轴
 * @param  y2: 矩形右下角坐标Y轴
 * @retval None
 */
void lcd_draw_rectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
	lcd_drawline(x1, y1, x2, y1);
	lcd_drawline(x1, y1, x1, y2);
	lcd_drawline(x1, y2, x2, y2);
	lcd_drawline(x2, y1, x2, y2);
}

/**
 * @brief  在指定位置画一个指定大小的圆
 * @note
 * @param  x0: 圆心坐标X轴
 * @param  y0: 圆心坐标Y轴
 * @param  r: 圆形半径
 * @retval None
 */
void lcd_draw_Circle(unsigned short x0,unsigned short y0,unsigned char r)
{
    int mx = x0, my = y0;
    int x = 0, y = r;

    int d = 1 - r;
    while(y > x)    	/* y>x即第一象限的第1区八分圆 */
    {
        lcd_drawpoint(x  + mx, y  + my, tftlcd_dev.forecolor);
        lcd_drawpoint(y  + mx, x  + my, tftlcd_dev.forecolor);
        lcd_drawpoint(-x + mx, y  + my, tftlcd_dev.forecolor);
        lcd_drawpoint(-y + mx, x  + my, tftlcd_dev.forecolor);

        lcd_drawpoint(-x + mx, -y + my, tftlcd_dev.forecolor);
        lcd_drawpoint(-y + mx, -x + my, tftlcd_dev.forecolor);
        lcd_drawpoint(x  + mx, -y + my, tftlcd_dev.forecolor);
        lcd_drawpoint(y  + mx, -x + my, tftlcd_dev.forecolor);
        if( d < 0)
        {
            d = d + 2 * x + 3;
        }
        else
        {
            d= d + 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

/**
 * @brief  在指定位置显示一个字符
 * @note
 * @param  x: 起始坐标X轴
 * @param  y: 起始坐标Y轴
 * @param  num: 显示字符
 * @param  size: 字体大小, 可选12/16/24/32
 * @param  mode: 叠加方式(1)还是非叠加方式(0)
 * @retval None
 */
void lcd_showchar(unsigned     short x, unsigned short y,
				      unsigned char num, unsigned char size,
				      unsigned char mode)
{
    unsigned char  temp, t1, t;
	unsigned short y0 = y;
	unsigned char csize = (size / 8+ ((size % 8) ? 1 : 0)) * (size / 2);	/* 得到字体一个字符对应点阵集所占的字节数	 */
 	num = num - ' ';  	/*得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）  */
	for(t = 0; t < csize; t++)
	{
		if(size == 12) temp = asc2_1206[num][t]; 		/* 调用1206字体 */
		else if(size == 16)temp = asc2_1608[num][t];	/* 调用1608字体 */
		else if(size == 24)temp = asc2_2412[num][t];	/* 调用2412字体 */
		else if(size == 32)temp = asc2_3216[num][t];	/* 调用3216字体 */
		else return;									/* 没有的字库 		*/
		for(t1 = 0; t1 < 8; t1++)
		{
			if(temp & 0x80)lcd_drawpoint(x, y, tftlcd_dev.forecolor);
			else if(mode==0)lcd_drawpoint(x, y, tftlcd_dev.backcolor);
			temp <<= 1;
			y++;
			if(y >= tftlcd_dev.height) return;			/* 超区域了 */
			if((y - y0) == size)
			{
				y = y0;
				x++;
				if(x >= tftlcd_dev.width) return;		/* 超区域了 */
				break;
			}
		}
	}
}

/**
 * @brief  计算m的n次方
 * @note
 * @param  m: 要计算的值
 * @param  n: n次方
 * @retval m^n次方.
 */
unsigned int lcd_pow(unsigned char m,unsigned char n)
{
	unsigned int result = 1;
	while(n--) result *= m;
	return result;
}

/**
 * @brief  显示指定的数字，高位为0的话不显示
 * @note
 * @param  x: 起始坐标点X轴。
 * @param  y: 起始坐标点Y轴。
 * @param  num: 数值(0~999999999)。
 * @param  len: 数字位数。
 * @param  size: 字体大小
 * @retval None
 */
void lcd_shownum(unsigned     short x,
					 unsigned short y,
					 unsigned int num,
					 unsigned char len,
					 unsigned char size)
{
	unsigned char  t, temp;
	unsigned char  enshow = 0;
	for(t = 0; t < len; t++)
	{
		temp = (num / lcd_pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				lcd_showchar(x + (size / 2) * t, y, ' ', size, 0);
				continue;
			}else enshow = 1;
		}
	 	lcd_showchar(x + (size / 2) * t, y, temp + '0', size, 0);
	}
}

/**
 * @brief  显示指定的数字，高位为0,还是显示
 * @note
 * @param  x: 起始坐标点X轴。
 * @param  y: 起始坐标点Y轴。
 * @param  num: 数值(0~999999999)。
 * @param  len: 数字位数。
 * @param  size: 字体大小
 * @param  mode: [7]:0,不填充;1,填充0.
 * 					  [6:1]:保留
 *					  [0]:0,非叠加显示;1,叠加显示.
 * @retval None
 */
void lcd_showxnum(unsigned     short x, unsigned short y,
					  unsigned int num, unsigned char len,
					  unsigned char size, unsigned char mode)
{
	unsigned char t, temp;
	unsigned char enshow = 0;
	for(t = 0; t < len; t++)
	{
		temp = (num / lcd_pow(10, len - t- 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				if(mode & 0X80) lcd_showchar(x + (size / 2) * t, y, '0', size, mode & 0X01);
				else  lcd_showchar(x + (size / 2) * t, y , ' ', size, mode & 0X01);
 				continue;
			}else enshow=1;

		}
	 	lcd_showchar( x + (size / 2) * t, y, temp + '0' , size , mode & 0X01);
	}
}

/**
 * @brief  显示一串字符串
 * @note
 * @param  x: 起始坐标点X轴。
 * @param  y: 起始坐标点Y轴。
 * @param  width: 字符串显示区域长度
 * @param  height: 字符串显示区域高度
 * @param  size: 字体大小
 * @param  *p: 要显示的字符串首地址
 * @retval None
 */
void lcd_show_string(unsigned short x,unsigned short y,
					 unsigned short width,unsigned short height,
					 unsigned char size,char *p)
{
	unsigned char x0 = x;
	width += x;
	height += y;
    while((*p <= '~') &&(*p >= ' '))		/* 判断是不是非法字符! */
    {
        if(x >= width) {x = x0; y += size;}
        if(y >= height) break;				/* 退出 */
        lcd_showchar(x, y, *p , size, 0);
        x += size / 2;
        p++;
    }
}

#define CHAR_SIZE	24
static char	lcd_buf[100][50];
static char white_buf[100];
static char flag = 0;
/**
 * @brief  lcd 格式化打印
 * @note
 * @param  *fmt:格式化格式
 * @retval None
 */
void  lcd_printf (const char *fmt, ...)
{
	char buf_tmp[100];
	va_list args;
	int i, last_line;
	unsigned char len0, len1;

	if (flag == 0) {
		for (i = 0; i < 100; i++)
			white_buf[i] = ' ';
		flag++;
	}

	last_line = tftlcd_dev.height / CHAR_SIZE;
#ifdef LCD_CMD_LINE
	last_line = LCD_CMD_LINE;
#endif

	/*	格式化字符串	*/
	va_start(args, fmt);
	i=vsprintf(buf_tmp, fmt, args);
	va_end(args);
	printf("len = %d\r\n", strlen(buf_tmp));

	/*	调整字符串顺序	*/
	for (i = 0; i < last_line - 1; i++) {
		len0 = strlen(lcd_buf[i]);
		len1 = strlen(lcd_buf[i + 1]);

		if (len0 - len1 > 0) {
			//需要清除多余字符的显示
			strcpy(lcd_buf[i], lcd_buf[i + 1]);
			memset(lcd_buf[i] + len1, ' ', len0 - len1);
		} else {
			strcpy(lcd_buf[i], lcd_buf[i + 1]);
		}

		//更新屏幕
		lcd_show_string(0, i * CHAR_SIZE,
					    tftlcd_dev.width, CHAR_SIZE, CHAR_SIZE,
						lcd_buf[i]);
	}

	sprintf((lcd_buf[last_line - 1]), "@zyc >> %s", buf_tmp);
	lcd_show_string(0, (last_line - 1) * CHAR_SIZE,
					    tftlcd_dev.width, CHAR_SIZE, CHAR_SIZE,
						(lcd_buf[last_line - 1]));
}