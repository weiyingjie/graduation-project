#ifndef __LDT_H
#define __LDT_H
/*
HC138_A0		PC11
HC138_A1		PC12
HC138_A2		PD2
HC595_DATA	PB3
HC595_LCLK	PB4
HC595_SCLK	PB5
需要驱动数码管,数码管的公共端连接到了74HC138上
数码管的LED端连接到了74HC595
*/
#include "stm32f10x_conf.h"//包含了该头文件所有的外设文件均已包含

extern void ldt_init(void);//初始化的函数(初始化模块使用到的管脚)

extern void hc138_out_data(uint8_t data);//通过hc138选择数码管的公共端

extern void hc595_send_data(uint8_t data);//通过hc595把串行数据转换成并行数据

extern void digit_show_data(int data);//在数码管中显示数据

#endif









