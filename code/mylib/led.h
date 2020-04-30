#ifndef __LED_H
#define __LED_H
/*
LED0	PC1
LED1	PC2
LED2	PC3
从原理图中已知
GPIO管脚输出高电平	灯亮
GPIO管脚输出低电平	灯灭
*/

#include "stm32f10x_conf.h"	// 包含ST公司提供的头文件

extern void led_init(void);	// 初始化

extern void led_on(int nu);	// 根据nu参数亮灯

extern void led_off(int nu);// 根据nu参数灭灯

#endif


