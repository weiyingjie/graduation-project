#include "esp8266.h"
#include "stdio.h"
#include "delay.h"
#include "iot.h"
#include "stdlib.h"

int upload_sensor_data(const char *ip, const char *sensor, double data)
{
	// 数值型传感器
	int s_id = 0;
	char send_data[512] = {0};
	char len_buf[32] = {0};
	
	s_id = atoi(sensor);
	sprintf(send_data, "GET /iot/php/esp_upload.php?s_id=%d&s_data=%f HTTP/1.1\r\nHost: www.wyjiot.cn\r\n\r\n", s_id, data);
	esp8266_connect((char *)ip, "80");
	delay_ms(500);
	esp8266_send(send_data, len_buf); 
	delay_ms(500);
	esp8266_disconnect();

	return 0;
}

int upload_sensor_status(const char *ip, const char *sensor, char *status)
{
	// 开关型传感器
	int s_id = 0;
	int s_data = 0;
	char send_data[512] = {0};
	char len_buf[32] = {0};
	
	s_id = atoi(sensor);
	s_data = atoi(status);
	sprintf(send_data, "GET /iot/php/esp_upload.php?s_id=%d&s_data=%d HTTP/1.1\r\nHost: www.wyjiot.cn\r\n\r\n", s_id, s_data);
	esp8266_connect((char *)ip, "80");
	delay_ms(200);
	esp8266_send(send_data, len_buf); 
	delay_ms(500);
	esp8266_disconnect();

	return 0;
}

int download_sensor_status(const char *ip, const char *sensor)
{
	int s_id = 0;
	char send_data[512] = {0};
	char len_buf[32] = {0};
	
	delay_ms(500);
	s_id = atoi(sensor);
	sprintf(send_data, "GET /iot/php/esp_download.php?s_id=%d HTTP/1.1\r\nHost: www.wyjiot.cn\r\n\r\n", s_id);
	esp8266_connect((char *)ip, "80");//建立链接
	esp8266_send(send_data, len_buf);
	delay_ms(300);
	esp8266_disconnect();

	return 0;
}

