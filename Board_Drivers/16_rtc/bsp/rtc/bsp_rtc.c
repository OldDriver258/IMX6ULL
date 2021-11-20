/**************************************************************
文件名 : bsp_rtc.c
作者 : 张宇辰
邮箱 : 1430024269@qq.com
版本 : V1.0
描述 : rtc 驱动
日志 : 初版 V1.0 2021/11/18 张宇辰创建
**************************************************************/
#include "bsp_rtc.h"
#include "stdio.h"

/**
 * @brief  使能 RTC 时钟
 * @note
 * @retval None
 */
static void  rtc_enable (void)
{
    SNVS->LPCR |= 1 << 0;
    while(!(SNVS->LPCR & 0x01));
}

/**
 * @brief  禁能 RTC 时钟
 * @note
 * @retval None
 */
static void  rtc_disable (void)
{
    SNVS->LPCR &= ~(1 << 0);
    while((SNVS->LPCR & 0x01));
}

/**
 * @brief  检查年份是否是闰年
 * @note
 * @param  year: 要检查的年份
 * @retval 是否是闰年 1：是闰年 2：不是闰年
 */
static unsigned char  rtc_isleapyear (unsigned short  year)
{
    unsigned char  value = 0;

    if (year % 400 == 0) {
        value = 1;
    } else {
        if ((year % 4 == 0) && (year % 100 != 0)) {
            value = 1;
        } else {
            value = 0;
        }
    }

    return  value;
}

/**
 * @brief  把日期转换为秒数
 * @note
 * @param  *datetime: 日期结构体
 * @retval 转换出的秒数
 */
static unsigned int  rtc_convert_date_to_seconds(struct rtc_datetime  *datetime)
{
    unsigned short  i       = 0;
    unsigned int    seconds = 0;
    unsigned int    days    = 0;
    unsigned short  monthdays[] = {
        0U, 0U, 31U, 59U, 90U, 120U, 151U,
        181U, 212U, 243U, 273U, 304U, 334U
    };

    for (i = 1970; i < datetime->year; i++) {
        days += DAYS_IN_A_YEAR;                 /* 平年，每年 365 天 */
        days += rtc_isleapyear(datetime->year); /* 闰年多加一天 */
    }

    days += monthdays[datetime->month];
    if(rtc_isleapyear(i) && (datetime->month >= 3))
        days += 1;

    days += datetime->day - 1;

    seconds = days             * SECONDS_IN_A_DAY    +
              datetime->hour   * SECONDS_IN_A_HOUR   +
              datetime->minute * SECONDS_IN_A_MINUTE +
              datetime->second;

    return  seconds;
}

/**
 * @brief  获得 RTC 的秒数
 * @note
 * @retval RTC 的秒数
 */
static unsigned int  rtc_getSeconds (void)
{
    unsigned int  seconds;

    seconds = (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);

    return  seconds;
}

/**
 * @brief 把秒数转换为日期
 * @note
 * @param  seconds:秒数
 * @param  *datetime:转换的日期
 * @retval None
 */
static void  rtc_convert_seconds_to_datetime(unsigned int          seconds,
                                             struct rtc_datetime  *datetime)
{
    unsigned int    x;
    unsigned int    secondsRemaining, days;
    unsigned short  daysInYear;

    unsigned char daysPerMonth[] = {
        0U, 31U, 28U, 31U, 30U, 31U,
        30U, 31U, 31U, 30U, 31U, 30U, 31U
    };

    secondsRemaining = seconds;
    days = secondsRemaining / SECONDS_IN_A_DAY + 1;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY;

    datetime->hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;

    datetime->minute = secondsRemaining / SECONDS_IN_A_MINUTE;
    datetime->second = secondsRemaining % SECONDS_IN_A_MINUTE;

    daysInYear = DAYS_IN_A_YEAR;
    datetime->year = YEAR_RANGE_START;
    while (days > daysInYear) {
        days -= daysInYear;
        datetime->year++;

        if (!(rtc_isleapyear(datetime->year)))
            daysInYear = DAYS_IN_A_YEAR;
        else
            daysInYear = DAYS_IN_A_YEAR + 1;
    }

    if (rtc_isleapyear(datetime->year))
        daysPerMonth[2] = 29;

    for (x = 1; x <= 12; x++) {
        if (days <= daysPerMonth[x]) {
            datetime->month = x;
            break;
        } else {
            days -= daysPerMonth[x];
        }
    }

    datetime->day = days;
}

/**
 * @brief  配置 RTC 时间
 * @note
 * @param  *datetime:时间结构体
 * @retval None
 */
void  rtc_setDatetime (struct rtc_datetime  *datetime)
{
    unsigned int seconds = 0;
    unsigned int tmp     = SNVS->LPCR;

    rtc_disable();  /* 设置寄存器 HPRTCMR 和 HPRTCLR 前要先关闭 RTC */

    seconds = rtc_convert_date_to_seconds(datetime);
    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17);
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15);

    /* 如果此前 RTC 是打开的在设置完 RTC 时间以后需要重新打开 RTC */
    if (tmp & 0x1) {
        rtc_enable();
    }
}

/**
 * @brief  获得 RTC 时间
 * @note
 * @param  *datetime: 时间结构体
 * @retval None
 */
void  rtc_getDatetime (struct rtc_datetime  *datetime)
{
    unsigned int seconds = 0;

    seconds = rtc_getSeconds();
    rtc_convert_seconds_to_datetime(seconds, datetime);
}

void  rtc_init (void)
{
    /*
     * 允许设置 SNVS 寄存器，先要置 1
     */
    SNVS->HPCOMR |= (1 << 31);

#if 0
    struct rtc_datetime rtcdate;

    rtcdate.year    = 2021;
    rtcdate.month   = 11;
    rtcdate.day     = 18;
    rtcdate.hour    = 23;
    rtcdate.minute  = 08;
    rtcdate.second  = 18;
    rtc_setDatetime(&rtcdate);
#endif

    rtc_enable();
}