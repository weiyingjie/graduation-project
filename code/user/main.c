#include "led.h"
#include "buzzer.h"
#include "fan.h"
#include "button.h"
#include "eint.h"
#include "iwdg.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "string.h"
#include "eeprom.h"
#include "sht.h"
#include "oled.h"
#include "oledfont.h"
#include "fire.h"
#include "ldt.h"
#include "mq5.h"

int main(void)
{
	char sht_buf[25] = {0};	// 存储SHT30的温湿度字符串
	double sht_data[2] = {0};	// 存储sht30的数据
	unsigned char sht_flag = 0, fire_flag = 0;
	int i = 500; // 数码管循环变量
	int mq5_value = 0; // 存储mq5的数据
	
	// 初始化
	led_init();		// 初始化LED
	buzzer_init();	// 初始化蜂鸣器
	fan_init();		// 初始化排风扇
	button_init();	// 初始化按键
	eint_init();	// 初始化外部中断
	delay_init();	// 初始化延时
	usart_1_init();	// 初始化串口1
	eeprom_init();	// 初始化eeprom
	sht_init();		// 初始化sht30
	OLED_Init();	// 初始化OLED
	fire_init();	// 初始化火焰传感器
	ldt_init();		// 初始化数码管
	mq5_init();		// 初始化液化气传感器
	
	
	// oled
	OLED_Clear();
	
	sht_write_mode(); // 配置sht30采样频率
	
	while(1) {
		if(!(sht_flag || fire_flag)) {
			// sht30
			sht_write_read_cmd();
			sht_read_data(sht_data);
			sprintf(sht_buf, "Temp:%0.2f Hum:%0.2f %%", sht_data[0], sht_data[1]);
			OLED_ShowString(0, 0, (u8 *)sht_buf, 14);
		}
		
		if (sht_data[1] > 80) {
			sht_flag = 1;
			
			OLED_Clear();
			OLED_ShowTEmp(46, 3); // 显示温度
			
			buzzer_on();
		}
		
		if (sht_flag) {
			// sht30
			sht_write_read_cmd();
			sht_read_data(sht_data);
			if (sht_data[1] <= 80) {
				sht_flag = 0;
				buzzer_off();
			}
		}
		
		// fire
		if(!fire_get_state()) {
			fire_flag = 1;
			
			OLED_Clear();
			OLED_ShowFIre(46, 3); // 显示火焰
			
			buzzer_on();
		}
		
		
		if(fire_flag) {
			if(fire_get_state()) {
				fire_flag = 0;
				buzzer_off();
			}
		}
		
		// 获取mq5的值
		mq5_value = mq5_get_value();
		
		// 数码管显示
		while(i--)
			digit_show_data(mq5_value);
		i = 500;
			
		led_on(0);
		delay_ms(500);
		led_off(0);
		delay_ms(500);
	}
}


