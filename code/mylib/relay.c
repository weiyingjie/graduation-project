#include "relay.h"
#include "bitband.h"

void relay_init()
{
	GPIO_InitTypeDef Relay_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Relay_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Relay_Value.GPIO_Pin = GPIO_Pin_5;
	Relay_Value.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &Relay_Value);
	
	PCOut(5) = 0;
}

void relay_on()
{
	PCOut(5) = 1;
}

void relay_off()
{
	PCOut(5) = 0;
}



