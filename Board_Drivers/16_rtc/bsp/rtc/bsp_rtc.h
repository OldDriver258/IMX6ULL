/**************************************************************
文件名 : bsp_rtc.h
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : rtc 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#ifndef _BSP_RTC_H
#define _BSP_RTC_H
#include "imx6ul.h"

#define DAYS_IN_A_YEAR      (365)       /* 一年 365 天 */
#define SECONDS_IN_A_DAY    (86400)     /* 一天 86400 秒 */
#define SECONDS_IN_A_HOUR   (3600)      /* 一个小时 3600 秒 */
#define SECONDS_IN_A_MINUTE (60)        /* 一分钟 60 秒 */
#define YEAR_RANGE_START    (1970)      /* 开始年份 1970 年 */
#define YEAR_RANGE_END      (2099)      /* 结束年份 2099 年 */

struct rtc_datetime {
    unsigned short  year;       /* 范围为:1970 ~ 2099 */
    unsigned char   month;      /* 范围为:1 ~ 12 */
    unsigned char   day;        /* 范围为:1 ~ 31 (不同的月，天数不同).*/
    unsigned char   hour;       /* 范围为:0 ~ 23 */
    unsigned char   minute;     /* 范围为:0 ~ 59 */
    unsigned char   second;     /* 范围为:0 ~ 59 */
};

void  rtc_init (void);
void  rtc_getDatetime (struct rtc_datetime  *datetime);
void  rtc_setDatetime (struct rtc_datetime  *datetime);

#endif