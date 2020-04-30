#include "dht11.h"
#include "delay.h"
#include "bitband.h"

void dht11_init(void) // 初始化dht11
{
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	delay_init();
}

void set_dht_gpio_output(void)
{
	GPIO_InitTypeDef Gpio_Value;
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &Gpio_Value);
}

void set_dht_gpio_input(void)
{
	GPIO_InitTypeDef Gpio_Value;
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Gpio_Value.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &Gpio_Value);
}

void dht_output_status(int state)
{
	set_dht_gpio_output();
	if(state == 1)
		PCOut(10) = 1;
	else
		PCOut(10) = 0;
}

int dht_input_status(void)
{
	set_dht_gpio_input();
	return PCIn(10);
}

void dht11_get_value(char *buf) // 获取值
{
	int i = 0;
	char flag = 0;
	char time = 0;
	char tmp = 0;
	
	// 发送开始信号
	dht_output_status(1);
	dht_output_status(0);
	delay_ms(18);
	dht_output_status(1);
	// 超时处理（40us）
	do
	{
		flag = dht_input_status();
		time++;
		delay_us(2);
	}while(flag == 1 && time <= 20);
	if(time > 20) // dht11没有响应
		return;
	// 响应了
	while(!dht_input_status());
	for(i = 0; i < 40; i++) {
		while(dht_input_status());
		while(!dht_input_status());
		delay_us(30);
		// 开始传输数据
		tmp <<= 1;
		if(dht_input_status()) // 数据为1
			tmp |= 1;
		if(i % 8 == 7) {
			buf[i / 8] = tmp;
			tmp = 0;
		}
	}
	dht_output_status(1);
}

