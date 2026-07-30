#include <time.h>       // time_t, struct tm, time(), localtime(), mktime()
#include <sys/time.h>   // struct timeval, settimeofday()
#include "bsp_time.h"

Calendar_t calendar;

void rtc_get_time(void)
{
    struct tm *datetime;
    time_t second;

    /* 返回自(1970.1.1 00:00:00 UTC)经过的时间(秒) */
    time(&second);
    datetime = localtime(&second);

    calendar.hour = datetime->tm_hour;   /* 时 */
    calendar.min  = datetime->tm_min;    /* 分 */
    calendar.sec  = datetime->tm_sec;    /* 秒 */

    /* 公历年月日周 */
    calendar.year  = datetime->tm_year + 1900;  /* 年 */
    calendar.month = datetime->tm_mon + 1;      /* 月 */
    calendar.date  = datetime->tm_mday;         /* 日 */

    /* 周 */
    calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date);
}


void rtc_set_time(int year, int mon, int mday, int hour, int min, int sec)
{
    struct tm datetime;

    /* 设置时间 */
    datetime.tm_year  = year - 1900;
    datetime.tm_mon   = mon - 1;
    datetime.tm_mday  = mday;
    datetime.tm_hour  = hour;
    datetime.tm_min   = min;
    datetime.tm_sec   = sec;
    datetime.tm_isdst = -1;

    /* 获取 1970.1.1 以来的总秒数 */
    time_t second = mktime(&datetime);
    struct timeval val = { .tv_sec = second, .tv_usec = 0 };

    /* 设置当前时间 */
    settimeofday(&val, NULL);
}

/*
 * Sakamoto 算法计算星期几
 * 返回值: 0=星期日, 1=星期一, ..., 6=星期六
 */
int rtc_get_week(int year, int month, int day)
{
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}
