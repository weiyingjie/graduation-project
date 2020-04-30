#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x_conf.h" 

#define SYSCLK 72				//系统时钟的基数(注:是72而不是72M)

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;
	
void delay_init(void);	//延时函数的初始化
void delay_ms(u16 nms);	//毫秒级的延时函数(参数为毫秒数)
void delay_us(u32 nus);	//微妙级的延时函数(参数为微妙数)

#endif





























