/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef RTC_CALENDAR_H
#define RTC_CALENDAR_H

#include "stm32f10x.h"

typedef struct{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
}rtc_time;
    
void GregorianDay(rtc_time * tm);
uint32_t mktimev(rtc_time *tm);
void to_tm(uint32_t tim, rtc_time * tm);



#endif
