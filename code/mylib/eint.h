#ifndef __EINT_H
#define __EINT_H
/*
使用中断的方式来获取按键
开发板设备	所连接的管脚	使用的外部中断号
KEY0		PC9				EXTI9
KEY1		PC8				EXTI8
KEY2		PA0				EXTI0
从原理图可知
KEY0和KEY1在按下时	检测到	低电平
		没有按下时	检测到	高电平
KEY2在按下时		检测到	高电平
		没有按下时	检测到	低电平
*/

#include "stm32f10x_conf.h"

typedef void (*eint_handler)(void);

extern void eint_init(void); // 初始化GPIO/NVIC/EXTI

extern void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2); // 设置中断处理函数

#endif

