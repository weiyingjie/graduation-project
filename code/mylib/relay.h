#ifndef __RELAY_H
#define __RELAY_H

/*
	继电器
	PC5
*/

#include "stm32f10x_conf.h"	// 包含ST公司提供的头文件
	
extern void relay_init(void);

extern void relay_on(void);

extern void relay_off(void);


#endif


