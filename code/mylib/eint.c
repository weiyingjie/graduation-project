#include "eint.h"
#include "delay.h"
#include "button.h"

static eint_handler eint_h0, eint_h1, eint_h2;

void eint_init(void) // 初始化GPIO/NVIC/EXTI
{
	GPIO_InitTypeDef Gpio_Value;
	NVIC_InitTypeDef Nvic_Value;
	EXTI_InitTypeDef Exti_Value;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	// 初始化GPIO(PC9 PC8 IPU	PA0 IPD)
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPU;
	Gpio_Value.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_Init(GPIOC, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IPD;
	Gpio_Value.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	
	// 初始化NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置NVIC优先级
	
	Nvic_Value.NVIC_IRQChannel = EXTI9_5_IRQn; // 设置中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	Nvic_Value.NVIC_IRQChannel = EXTI0_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	// 初始化EXTI
	Exti_Value.EXTI_Line = EXTI_Line9 | EXTI_Line8;
	Exti_Value.EXTI_LineCmd = ENABLE;
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt; // 设置为中断模式
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	EXTI_Init(&Exti_Value);
	Exti_Value.EXTI_Line = EXTI_Line0;
	Exti_Value.EXTI_LineCmd = ENABLE;
	Exti_Value.EXTI_Mode = EXTI_Mode_Interrupt;
	Exti_Value.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
	EXTI_Init(&Exti_Value);
	delay_init();
}


void EXTI9_5_IRQHandler(void) // 实现中断处理的回调函数
{
	// 判断是由哪个外部中断触发的
	if(EXTI_GetITStatus(EXTI_Line9) == SET) {
		delay_ms(30);
		if(button_status(0)) {
			while(button_status(0))
				delay_ms(30);
			eint_h0();
			// 清除标志位
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
	}
	
	if(EXTI_GetITStatus(EXTI_Line8) == SET) {
		delay_ms(30);
		if(button_status(1)) {
			while(button_status(1))
				delay_ms(30);
			eint_h1();
			// 清除标志位
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
	}
}

void EXTI0_IRQHandler(void)
{
	delay_ms(30);
	if(button_status(2)) {
		while(button_status(2))
			delay_ms(30);
		eint_h2();
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

void set_eint_handler(eint_handler h0, eint_handler h1, eint_handler h2) // 设置中断处理函数
{
	eint_h0 = h0;
	eint_h1 = h1;
	eint_h2 = h2;
}

