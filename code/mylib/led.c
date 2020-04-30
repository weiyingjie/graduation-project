#include "led.h"
#include "bitband.h"

void led_init(void)	// 初始化
{
	GPIO_InitTypeDef Led_Value;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// PC1~3设置推挽输出
	
	Led_Value.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	Led_Value.GPIO_Speed = GPIO_Speed_50MHz;// 速度50Mhz
	Led_Value.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	
	GPIO_Init(GPIOC, &Led_Value);
	
	PCOut(1) = 0;
	PCOut(2) = 0;
	PCOut(3) = 0;
}

void led_on(int nu)	// 根据nu参数亮灯
{
	switch (nu)
	{
		case 0 : PCOut(1) = 1; break;
		case 1 : PCOut(2) = 1; break;
		case 2 : PCOut(3) = 1; break;
		default:
			break;
	}
}

void led_off(int nu)// 根据nu参数灭灯
{
	switch (nu)
	{
		case 0 : PCOut(1) = 0; break;
		case 1 : PCOut(2) = 0; break;
		case 2 : PCOut(3) = 0; break;
		default:
			break;
	}
}


