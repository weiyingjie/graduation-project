#include "button.h"
#include "bitband.h"

void button_init(void)
{
	GPIO_InitTypeDef Button_Value1;
	GPIO_InitTypeDef Button_Value2;
	
	// 使能A组和C组时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	// KEY0,1 上拉输入1111// WK_UP 下拉
	Button_Value1.GPIO_Mode = GPIO_Mode_IPU;
	Button_Value1.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	Button_Value2.GPIO_Mode = GPIO_Mode_IPD;
	Button_Value2.GPIO_Pin = GPIO_Pin_0;
	
	GPIO_Init(GPIOC, &Button_Value1);
	GPIO_Init(GPIOA, &Button_Value2);

	//GPIO_SetBits(GPIOC, GPIO_Pin_8 | GPIO_Pin_9);
	//GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

int button_status(int nu)
{
	int ret = 0;
	switch(nu) {
		case 0 : ret = PCIn(9); break;
		case 1 : ret = PCIn(8); break;
		case 2 : ret = PAIn(0); ret = !ret; break;
	}
	return !ret;
}

