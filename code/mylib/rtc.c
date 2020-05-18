#include "rtc.h"
#include "string.h"

const uint8_t monthTable[12] = {31,28,31,30,31,30,31,31,30,31,30,31};//存储每个月的天数(闰年2月份需要多加1天)
static struct time_st T;//定义了日期时间结构体的全局变量

static uint8_t isLeapYear(uint16_t y)//判断是否是闰年
{
	return (((!(y % 4)) && (y % 100)) || (!(y % 400)));
}

void RTC_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) == SET)//判断是否是由RTC的秒中断触发的中断
	{
		conv_time(&T);//转换RTC中的时间并保存到全局变量T中
		RTC_ClearITPendingBit(RTC_IT_SEC);//清除RTC秒中断的中断标志
	}
}

void NVIC_Configuration(void)//配置中断控制器
{
	NVIC_InitTypeDef Nvic_Value;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置组优先级和子优先级的所占比例
	Nvic_Value.NVIC_IRQChannel = RTC_IRQn;//选择RTC的中断号
	Nvic_Value.NVIC_IRQChannelCmd = ENABLE;//选择使能该中断
	Nvic_Value.NVIC_IRQChannelPreemptionPriority = 2;//选择组优先级的级别是2
	Nvic_Value.NVIC_IRQChannelSubPriority = 2;//选择子优先级的级别是2
	NVIC_Init(&Nvic_Value);//按照上述配置初始化NVIC
}

void RTC_Configuration(void)//配置RTC
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);
	//通过APB1总线使能BKP和PWR的时钟
	PWR_BackupAccessCmd(ENABLE);//通过PWR使能后备区域的写访问(关闭了写保护)
	BKP_DeInit();//复位BKP
	RCC_LSEConfig(RCC_LSE_ON);//通过RCC开启LSE时钟源
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);//死等的方式等待LSE稳定
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//通过RCC为RTC选择由LSE提供时钟源
	RCC_RTCCLKCmd(ENABLE);//通过RCC使能RTC的时钟
	RTC_WaitForSynchro();//等待RTC时钟同步
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能RTC的秒中断
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_EnterConfigMode();//进入RTC的配置模式
	RTC_SetPrescaler(32767);//设置RTC的预分频
	RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	RTC_ExitConfigMode();//退出RTC的配置模式
}

void rtc_init(struct time_st *t)//初始化RTC
{
	NVIC_Configuration();//配置了RTC的中断
	if(BKP_ReadBackupRegister(BKP_DR10) != 0x9527)//通过读备份寄存器中的值是否是特定的值判断是否是第一次操作RTC
	{
		RTC_Configuration();//配置RTC
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();//进入RTC的配置模式
		set_time(t);//设置时间
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_ExitConfigMode();//退出RTC的配置模式
		BKP_WriteBackupRegister(BKP_DR10, 0x9527);//为BKP的寄存器中写入特定的值
	}
	else//不是第一次操作RTC
	{
		RTC_WaitForSynchro();//等待RTC的时钟同步
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);//使能RTC的秒中断
		RTC_WaitForLastTask();//等待最后一次对RTC寄存器的写操作完成
	}
}

void set_time(struct time_st* t)//设置时间
{
	uint32_t secCount = 0;
	uint16_t i;

	if(t->Y < 1970 || t->Y > 2099)
		return ;
	
	for(i = 1970; i < t->Y; i++)
	{
		if(isLeapYear(i))
			secCount += 31622400;
		else
			secCount += 31536000;
	}

	for (i = 0; i < t->M - 1; i++)
	{
		secCount += ((uint32_t)monthTable[i] * 86400);
		if (isLeapYear(t->Y) && i == 1)
			secCount += 86400;
	}
	secCount += (uint32_t)(t->D - 1) * 86400;    
	secCount += (uint32_t)(t->h) * 3600;
	secCount += (uint32_t)(t->m) * 60;
	secCount += (uint32_t)(t->s);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	RTC_SetCounter(secCount);
	RTC_WaitForLastTask();
}

void conv_time(struct time_st* t)//转换时间
{
	uint32_t timeCount = 0;
	uint32_t Count = 0;
	uint16_t tmp = 0;
	
	timeCount = RTC_GetCounter();
	Count = timeCount / 86400;
	t->W = (4 + Count) % 7;
	if(Count != 0)
	{
		tmp = 1970;
		while(Count >= 365)
		{
			if(isLeapYear(tmp))
			{
				if(Count>= 366)
				{
					Count -= 366;
				}
				else
				{
					tmp++;
					break;
				}
			}
			else
			{
					Count -= 365;
			}
			tmp++;
		}
		t->Y = tmp;
		tmp = 0;
		while(Count >= 28)
		{
			if(isLeapYear(t->Y) && tmp == 1)
			{
				if(Count >= 29) 
						Count -= 29;
				else    
						break;
			}
			else
			{
				if(Count >= monthTable[tmp]) 
					Count -= monthTable[tmp];
				else
					break;
			}
			tmp++;
		}
		t->M = tmp + 1;
		t->D = Count + 1;
    }
    Count = timeCount % 86400;
    t->h = Count / 3600;
    t->m = (Count % 3600) / 60;
    t->s = (Count % 3600) % 60;
}

void get_time(struct time_st *t)//获取时间
{
	memcpy(t, &T, sizeof(T));//把全局变量T中保存的数据另存到形参t中
}





