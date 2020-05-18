#ifndef __RTC_H
#define __RTC_H
/*
该文件用于驱动STM32F103RBT6芯片中RTC的外设
时钟频率使用的是LSE加预分频 = 1Hz
计数值是用来存储秒数的
我们表示的时间范围为
1970/1/1 00:00:00 ----- 2099/12/31 23:59:59
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设头文件均已包含

struct time_st//用于存储日期和时间的结构体
{
	int Y;//年份
	int M;//月份
	int D;//日期
	int W;//星期
	int h;//小时
	int m;//分钟
	int s;//秒数
};

extern void rtc_init(struct time_st *t);//初始化RTC

extern void set_time(struct time_st *t);//设置时间

extern void conv_time(struct time_st *t);//转换时间

extern void get_time(struct time_st *t);//获取时间

#endif







