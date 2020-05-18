#include "led.h"
#include "buzzer.h"
#include "fan.h"
#include "button.h"
#include "iwdg.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "sht.h"
#include "oled.h"
#include "oledfont.h"
#include "fire.h"
#include "ldt.h"
#include "mq5.h"
#include "esp8266.h"
#include "iot.h"
#include "rtc.h"
#include "bitband.h"

#define FAN_VALUE		PCOut(5)
#define BUZZER_VALUE	PCOut(7)
#define FAN_ID			"23"
#define BUZZER_ID		"22"
#define SERVER_IP		"148.70.179.241"

static int buzzer_cmd = 0; // 云平台的指令

void recv_handler(char *buf, int len)
{
	if(strstr(buf, FAN_ID)) {
		if(strstr(buf, "true")) {
			fan_on();
		} else if (strstr(buf, "false")) {
			fan_off();
		}
	} else if (strstr(buf, BUZZER_ID)) {
		if(strstr(buf, "true")) {
			buzzer_cmd = 1;
			buzzer_on();
		} else if (strstr(buf, "false")) {
			buzzer_cmd = 0;
			buzzer_off();
		}
	}
}

int main(void)
{
	struct time_st oldtime = {2020, 5, 18, 1, 16, 29, 0};//定义当前时间结构体变量
	struct time_st newtime;	
	char Date[13] = {0}; // 日期
	char Time[9] = {0}; // 时间
	double sht_data[2] = {0};	// 存储sht30的数据
	char Sht_Temp[9] = {0}; // sht30温度转换成字符串
	char Sht_Hum[11] = {0}; // sht30湿度转换成字符串
	unsigned char sht_flag = 0, fire_flag = 0, mq5_flag = 0;
	int i = 500; // 数码管循环变量
	int mq5_value = 0; // 存储mq5的数据
	char status[1] = {0}; // 存储开关型传感器的状态
	
	// 初始化
	led_init();		// 初始化LED
	buzzer_init();	// 初始化蜂鸣器
	fan_init();		// 初始化排风扇
	delay_init();	// 初始化延时
	sht_init();		// 初始化sht30
	OLED_Init();	// 初始化OLED
	fire_init();	// 初始化火焰传感器
	esp8266_init(); // 初始化esp8266
	ldt_init();		// 初始化数码管
	mq5_init();		// 初始化液化气传感器
	iwdg_init(156 * 10);// iwdg 10秒喂狗时间
	rtc_init(&oldtime); // 初始化rtc

	sht_write_mode(); // 配置sht30采样频率
	
	OLED_Clear();
	OLED_ShowString(25, 1, (u8 *)"Connecting", 16);
	OLED_ShowString(25, 4, (u8 *)"to WiFi...", 16);
	
	// esp8266
	led_on(0);
	esp8266_link_wifi("TP-LINK_2203", "123456789");
	delay_ms(200);
	led_off(0);
	
	OLED_Clear();
	OLED_ShowString(25, 3, (u8 *)"Success!!", 16);
	delay_ms(1000);
	OLED_Clear();
	
	set_wifi_recv_handler(recv_handler); // ESP8266接收的回调函数
	
	while(1) {
		
		// 喂狗
		iwdg_feed_dog();
		
		if(!(sht_flag || fire_flag)) {
			// sht30
			sht_write_read_cmd();
			sht_read_data(sht_data);// 获取SHT30数据			
			get_time(&newtime);//获取时间
			sprintf(Sht_Temp, "Tmp:%.2f", sht_data[0]);//SHT30温度字符串
			sprintf(Sht_Hum, "Hum:%.2f%%", sht_data[1]);//SHT30湿度字符串
			sprintf(Date, "%d-%02d-%02d %d", newtime.Y, newtime.M, newtime.D, newtime.W);//日期字符串
			sprintf(Time, "%02d:%02d:%02d", newtime.h, newtime.m, newtime.s);//时间字符串
			OLED_ShowString(0, 0, (u8 *)Sht_Temp, 12);//OLED显示温度
			OLED_ShowString(0, 2, (u8 *)Sht_Hum, 12);//OLED显示湿度
			OLED_ShowString(0, 4, (u8 *)Date, 12);//OLED显示日期
			OLED_ShowString(0, 6, (u8 *)Time, 12);//OLED显示时间
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
				OLED_Clear();
				if (fire_flag == 0 && mq5_flag == 0 && buzzer_cmd == 0)
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
				OLED_Clear();
				if(sht_flag == 0 && mq5_flag == 0 && buzzer_cmd == 0)
					buzzer_off();
			}
		}
		
		// 获取mq5的值	常态是1800±	判断高于2300为有易燃气体泄漏
		mq5_value = mq5_get_value();
		if(mq5_value >= 2300) {
			buzzer_on();
		} else if (mq5_value < 2300 && sht_flag == 0 && fire_flag == 0 && buzzer_cmd == 0){
			buzzer_off();
		}
		
		// 数码管显示
		while(i--)
			digit_show_data(mq5_value);
		i = 500;
		
		iwdg_feed_dog(); // 喂狗
		
		// 整理及上传数据
		led_on(1);
		upload_sensor_data(SERVER_IP, "24", sht_data[0]); // 温度
		led_off(1);
		delay_ms(200);
		led_on(1);
		upload_sensor_data(SERVER_IP, "25", sht_data[1]); // 湿度
		led_off(1);
		iwdg_feed_dog(); // 喂狗
		delay_ms(200);
		led_on(1);
		upload_sensor_data(SERVER_IP, "26", mq5_value); // MQ5
		delay_ms(200);
		led_off(1);
		if(FAN_VALUE)
			sprintf(status, "%d", 1);
		else
			sprintf(status, "%d", 0);
		upload_sensor_status(SERVER_IP, FAN_ID, status);
		led_on(1);
		delay_ms(200);
		led_off(1);
		if(BUZZER_VALUE)
			sprintf(status, "%d", 1);
		else
			sprintf(status, "%d", 0);
		upload_sensor_status(SERVER_IP, BUZZER_ID, status);
		led_on(1);
		delay_ms(200);
		led_off(1);
		
		iwdg_feed_dog(); // 喂狗
		
		// 下载数据
		led_on(1);
		download_sensor_status(SERVER_IP, FAN_ID);
		led_off(1);
		delay_ms(200);
		led_on(1);
		download_sensor_status(SERVER_IP, BUZZER_ID);
		led_off(1);
		delay_ms(200);
	}
}


