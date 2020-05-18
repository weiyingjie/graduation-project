#include "usart2.h"
#include "esp8266.h"

usart_2_handler usart2_handler;

void usart_2_init(void)
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
	
	Usart_Value.USART_BaudRate = 115200;
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	Usart_Value.USART_Parity = USART_Parity_No;
	Usart_Value.USART_StopBits = USART_StopBits_1;
	Usart_Value.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &Usart_Value);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART2_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		
	USART_Cmd(USART2, ENABLE);
}

void usart_2_send_byte(char c)
{
	USART_SendData(USART2, (uint16_t)c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void usart_2_send_data(char *buf)
{
	while(*buf)
	{
		usart_2_send_byte(*buf);
		buf++;
	}
}

void USART2_IRQHandler(void)
{
	uint16_t data;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		data = USART_ReceiveData(USART2);
		dataHandler(data);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

void set_usart_2_handler(usart_2_handler h)
{
	usart2_handler = h;
}

unsigned char usart_2_recv_byte(void)
{
	unsigned char data;
	
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET)
	{	
		data = USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	return data;
}






