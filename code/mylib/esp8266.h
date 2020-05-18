#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f10x.h"

typedef void (*wifi_recv_handler)(char *buf, int len);

extern void set_wifi_recv_handler(wifi_recv_handler h);
extern void esp8266_init(void);//初始化esp8266
extern int esp8266_link_wifi(char *ssid, char *passwd);//连接wifi
extern uint32_t esp8266_connect(char *ip, char *port);//建立连接
extern uint32_t esp8266_disconnect(void);//断开连接
extern uint32_t esp8266_send(char *msg, char *len);//发送数据
extern void dataHandler(unsigned char c);

#endif

