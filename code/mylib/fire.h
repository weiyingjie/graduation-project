#ifndef __FIRE_H
#define __FIRE_H
/*
	火焰传感器
	PA7	浮空输入
*/
#include "stm32f10x_conf.h"

extern void fire_init(void); // 初始化

extern u8 fire_get_state(void); // 获取有无火焰

#endif


