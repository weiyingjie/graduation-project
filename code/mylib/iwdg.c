#include "iwdg.h"

void iwdg_init(int ms) // 初始化
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // 开启写访问
	IWDG_SetPrescaler(IWDG_Prescaler_256); // 选择64分频		40MHz / 64 = 625Hz
	IWDG_SetReload(ms); // 设置重载值
	IWDG_ReloadCounter(); // 喂狗
	IWDG_Enable(); // 启动
}

void iwdg_feed_dog(void) // 喂狗
{
	IWDG_ReloadCounter();
}

