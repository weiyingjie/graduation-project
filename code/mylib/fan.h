#ifndef __FAN_H
#define __FAN_H
/*
	排风扇
	PC5管脚控制
*/
#include "stm32f10x_conf.h"	// 包含ST公司提供的头文件
	
extern void fan_init(void);

extern void fan_on(void);

extern void fan_off(void);


#endif


