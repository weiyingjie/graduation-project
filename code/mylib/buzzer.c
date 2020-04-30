#include "buzzer.h"
#include "bitband.h"

void buzzer_init()
{
	GPIO_InitTypeDef Buzzer_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	Buzzer_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Buzzer_Value.GPIO_Pin = GPIO_Pin_7;
	Buzzer_Value.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &Buzzer_Value);
	
	PCOut(7) = 0;
}

void buzzer_on()
{
	PCOut(7) = 1;
}

void buzzer_off()
{
	PCOut(7) = 0;
}

