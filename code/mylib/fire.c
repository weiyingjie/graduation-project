#include "fire.h"
#include "bitband.h"

void fire_init(void) // 初始化
{
	GPIO_InitTypeDef Gpio_Value;
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIO
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &Gpio_Value);
	
}

u8 fire_get_state(void) // 获取有无火焰
{
	// 无火焰返回1
	return PAIn(7);
}

