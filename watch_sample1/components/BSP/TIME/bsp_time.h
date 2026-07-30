#ifndef _BSP_TIME_H_
#define _BSP_TIME_H_

/* 日历结构体 */
typedef struct {
    int hour, min, sec;
    int year, month, date;
    int week;
} Calendar_t;

extern Calendar_t calendar;

void rtc_get_time(void);
void rtc_set_time(int year, int mon, int mday, int hour, int min, int sec);
int  rtc_get_week(int year, int month, int day);

#endif /* _BSP_TIME_H_ */
