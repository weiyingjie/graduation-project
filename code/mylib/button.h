#ifndef __BUTTON_H
#define __BUTTON_H

/*
KEY0	PC9
KEY1	PC8
WK_UP	PA0
*/

#include "stm32f10x_conf.h"
	
extern void button_init(void);

extern int button_status(int nu);

#endif


