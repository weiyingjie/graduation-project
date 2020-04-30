#include "ldt.h"
#include "bitband.h"
#include "delay.h"

#define HC138_A0		PCOut(11)
#define HC138_A1		PCOut(12)
#define HC138_A2		PDOut(2)
#define HC595_SLCK		PBOut(5)
#define HC595_LCLK		PBOut(4)
#define HC595_DATA		PBOut(3)

void ldt_init(void)//初始化的函数(初始化模块使用到的管脚)
{
	GPIO_InitTypeDef Gpio_Value;//定义了GPIO固件的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
	RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//通过APB2总线使能GPIOC组 GPIOB组 GPIOD组 和AFIO的时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//改变指定管脚的映射
	//JTAG-DP 关闭功能 + SW-DP 使能
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//选择 11 12 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择50MHz的输出速度
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_2;//选择 2 号管脚
	GPIO_Init(GPIOD, &Gpio_Value);//按照上述配置初始化GPIOD组的管脚
	
	Gpio_Value.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;//选择3 4 5号管脚
	GPIO_Init(GPIOB, &Gpio_Value);//按照上述配置初始化GPIOB组的管脚
	
	HC138_A0 = 0;
	HC138_A1 = 0;
	HC138_A2 = 0;
	HC595_SLCK = 0;
	HC595_LCLK = 0;
	HC595_DATA = 0;
}

void hc138_out_data(uint8_t data)//通过hc138选择数码管的公共端
{//按照74HC138的官方手册上的真值表配置函数
	switch(data) {
		case 0 : HC138_A0 = 0; HC138_A1 = 0; HC138_A2 = 0; break;
		case 1 : HC138_A0 = 1; HC138_A1 = 0; HC138_A2 = 0; break;
		case 2 : HC138_A0 = 0; HC138_A1 = 1; HC138_A2 = 0; break;
		case 3 : HC138_A0 = 1; HC138_A1 = 1; HC138_A2 = 0; break;
	}
}

void hc595_send_data(uint8_t data)//通过hc595把串行数据转换成并行数据
{//按照74HC595的官方手册上的流程配置函数
	uint8_t i = 0; // 循环变量
	
	for(i = 0; i < 8; i++) {
		HC595_DATA = ((data >> 7) & 0x01); // 将最低位数据交给DS管脚
		HC595_SLCK = 1; // SH_CP管脚产生上升沿
		data <<= 1;
		HC595_SLCK = 0;
	}
	HC595_LCLK = 1; // ST_CP管脚产生上升沿
	//delay_ms(1);
	HC595_LCLK = 0;
}

void digit_show_data(int data)//在数码管中显示数据
{//使用的是4位共阴极的数码管,自己去找编码 0x3F 0x06 ... (0 ~ 9 A ~ F)
	uint8_t i = 0;
	int data_[4] = {0};
	uint8_t table[] = {
		0x3f,0x06,0x5b,0x4f, // 0-3
		0x66,0x6d,0x7d,0x07, // 4-7
		0x7f,0x6f,0x77,0x7c, // 6-b
		0x39,0x5e,0x79,0x71	 // c-f
	};//存放数码管的编码
	
	data_[0] = data / 1000;
	data_[1] = (data / 100) % 10;
	data_[2] = (data / 10) % 10;
	data_[3] = data % 10;
	
	for(i = 0; i < 4; i++) {
		hc138_out_data(i);//选择公共端(选择让哪个数码管显示数据)
		hc595_send_data(table[data_[i]]);//把编码通过函数转换成并行数据
		delay_ms(1);
		hc595_send_data(0);//手动清空74HC595模块中8位位移寄存器中的数据
	}

}










