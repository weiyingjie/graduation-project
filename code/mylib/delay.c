#include "delay.h"

//利用系统滴答定时，编写的延时函数

static u8  fac_us=0; //us延时倍乘数			   
static u16 fac_ms=0; //ms延时倍乘数

/****************************************************************************
* 名    称: delay_init()
* 功    能：延时函数初始化
* 入口参数：无
* 返回参数：无
* 说    明：
****************************************************************************/
void delay_init(void)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	//为系统定时器选择时钟为	HCLK 8分频	72MHz / 8 = 9MHz
	fac_us = SYSCLK / 8;
	//微妙的倍乘数 = 72 / 8 = 9
	//X * fac_us就相当于有X个微妙
	fac_ms = (u16)fac_us * 1000; 
	//每个ms需要的systick时钟数(1ms = 1000us),所以fac_us*1000
	//X * fac_ms就相当于有X个毫妙
}								    

/****************************************************************************
* 名    称: void delay_us(u32 nus)
* 功    能：延时nus
* 入口参数：要延时的微妙数
* 返回参数：无
* 说    明：nus的值,不要大于1864135us
****************************************************************************/
/*
通过相关手册我们了解到,备份值寄存器和当前值寄存器的值最大为16M - 1
我们的微妙级的延时函数的倍乘数为9所以16M - 1 / 9 = 1864135
*/
void delay_us(u32 nus)
{
	u32 midtime;																			//保存寄存器的状态
	SysTick->LOAD = nus * fac_us;											//时间加载(相当于有nus个微妙)
	SysTick->VAL = 0x00;															//清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;					//使能系统定时器计数
	do
	{
		midtime = SysTick->CTRL;
	}
	while((midtime & 0x01) && !(midtime & (1 << 16)));//等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;				//关闭计数器
	SysTick->VAL = 0X00;															//清空计数器	 
}
/****************************************************************************
* 名    称: void delay_xms(u16 nms)
* 功    能：延时nms
* 入口参数：要延时的毫妙数
* 返回参数：无
* 说    明：SysTick->LOAD为24位寄存器,所以,最大延时为: nms<=0xffffff*8*1000/SYSCLK
            对9M条件下,nms<=1864ms 
****************************************************************************/
void delay_xms(u16 nms)
{	 		  	  
	u32 midtime;		   
	SysTick->LOAD = (u32)nms*fac_ms;									//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;															//清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;					//开始倒数
	do
	{
		midtime = SysTick->CTRL;
	}
	while((midtime & 0x01) && !(midtime & (1 << 16)));//等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;				//关闭计数器
	SysTick->VAL = 0X00;															//清空计数器
} 
/****************************************************************************
* 名    称: void delay_ms(u16 nms)
* 功    能：延时nms
* 入口参数：要延时的毫妙数
* 返回参数：无
* 说    明：nms:0~65535
****************************************************************************/
void delay_ms(u16 nms)
{	 	 
	u8 repeat = nms / 540;	//记录有多少个整的540ms
	u16 remain = nms % 540;	//记录有多少个不够540ms
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 

			 
