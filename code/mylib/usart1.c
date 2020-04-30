#include "usart1.h"
#include "string.h"

/*
串口USART1
RX	PA10	接收	只需要配置成浮空输入
TX	PA9		发送	需要配置成复用推挽
*/

#define RECVSIZE	128

usart1_handler u1_h;


void usart_1_init(void) // 初始化USART1
{
	GPIO_InitTypeDef Gpio_Value;
	USART_InitTypeDef Usart_Value;
	NVIC_InitTypeDef Nvic_Value;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	// 初始化GPIO
	Gpio_Value.GPIO_Mode = GPIO_Mode_AF_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_9;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	// 初始化USART1
	Usart_Value.USART_BaudRate = 115200; // 设置波特率115200
	Usart_Value.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 关闭硬件流控
	Usart_Value.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 设置串口模式为发送接收
	Usart_Value.USART_Parity = USART_Parity_No; // 没有奇偶校验位
	Usart_Value.USART_StopBits = USART_StopBits_1; // 1个停止位
	Usart_Value.USART_WordLength = USART_WordLength_8b; // 8个数据位
	USART_Init(USART1, &Usart_Value); // 初始化USART1
	
	// 初始化NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Nvic_Value.NVIC_IRQChannel = USART1_IRQn;
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&Nvic_Value);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能USART1的中断为接收触发
	
	USART_Cmd(USART1, ENABLE); // 使能USART1
}

void USART1_IRQHandler(void)
{	
	static int i = 0;
	unsigned char c = 0;
	static char recv_str[RECVSIZE] = {0};
	static int recv_flag = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		//u1_h(USART_ReceiveData(USART1));
		recv_flag = 1; // 开始接收
		c = USART_ReceiveData(USART1);
		if(i <= RECVSIZE) {
			if(c == ';') {
				recv_flag = 0; // 完成接收
				c = '\0';
			}
			recv_str[i++] = c;
		} else {
			recv_flag = 0;
		}
		if(!recv_flag) {
			// 接收完成调用函数
			u1_h(recv_str);
			//memset(recv_str, '\0', RECVSIZE);
			i = 0;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除标志位
	}
}

void usart_1_send_byte(unsigned char c) // 发送数据
{
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART1, USART_FLAG_TC); // 清除标志位
}

unsigned char usart_1_recv_byte(void) // 接收数据
{
	unsigned char ret = 0;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
		ret = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	return ret;
}

void usart_1_send_data(char *buf) // 发送字符串
{
	int i = 0;
	int len = strlen(buf);
	
	for(i = 0; i < len; i++) {
		usart_1_send_byte(buf[i]);
	}
}

void set_usart_1_handler(usart1_handler h)
{
	u1_h = h;
}

