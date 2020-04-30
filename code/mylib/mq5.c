#include "mq5.h"

/*
	液化气传感器
	AO	PC4	模拟输入	由原理图可知AD转换通道为14
	DO	PA4	数字输入
*/

void adc_init(void) // 初始化ADC
{
	GPIO_InitTypeDef Gpio_Value;
	ADC_InitTypeDef Adc_Value;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	// 初始化GPIO
	Gpio_Value.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入
	Gpio_Value.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &Gpio_Value);
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
	GPIO_Init(GPIOC, &Gpio_Value);
	
	// 初始化ADC
	Adc_Value.ADC_ContinuousConvMode = DISABLE; // 连续转换模式关闭
	Adc_Value.ADC_DataAlign = ADC_DataAlign_Right; // 数据右对齐
	Adc_Value.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 关闭外部触发
	Adc_Value.ADC_Mode = ADC_Mode_Independent; // 单次转换模式
	Adc_Value.ADC_NbrOfChannel = 14; // 转换通道为14
	Adc_Value.ADC_ScanConvMode = DISABLE; // 扫描模式关闭
	ADC_Init(ADC1, &Adc_Value);
	
	// 配置转换通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
	
	// 使能ADC
	ADC_Cmd(ADC1, ENABLE);
}

void adc_start(void) // 开始转换
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void adc_wait(void)
{
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // 清除标志位
}

int adc_get_value(void)
{
	return ADC_GetConversionValue(ADC1);
}

void mq5_init(void) // 初始化
{
	adc_init();
}

int mq5_get_value(void) // 获取检测的模拟量
{
	adc_start();
	adc_wait();
	return adc_get_value();
}

