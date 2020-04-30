#ifndef __MQ5_H
#define __MQ5_H
/*
	液化气传感器
	AO	PC4	模拟输入
	DO	PA4	数字输入
*/

#include "stm32f10x_conf.h"

extern void mq5_init(void); // 初始化

extern int mq5_get_value(void); // 获取检测的模拟量

#endif