#include "esp8266.h"
#include "usart2.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "led.h"
#include "usart1.h"

#define O_F			1
#define K_F			2
static int flag = O_F;
static int ok_flag = 0;
static int len;

//tcp pc ---- stm32-tcp客户端 ---uart---esp8266    pc-TCP调试助手
// wifi  connect send
//+IPD,13:1234567890abc
#define ADD_S		0
#define I_S			1
#define P_S			2
#define D_S			3
#define H_S			4
#define LEN_S		5
#define DATA_S		6
static int data_flag = ADD_S;


wifi_recv_handler wifi_handler = NULL;

void set_wifi_recv_handler(wifi_recv_handler h)
{
	wifi_handler = h;
}

void dataHandler(unsigned char c)
{
	static char len_buf[5];
	static int i = 0;
	static char data_buf[512];
	static char res_buf[27] = {0};
	static int index = 0;
	static int done = 0;
			
	// {"s_id":"16","data":false}

	if (c == '{') {
		done = 1;
	}
	
	if (done == 1) {
		res_buf[index++] = c;
		if (c == '}') {
			done = 0;
			res_buf[index] = '\0';
			//usart_1_send_data(res_buf);
			if(wifi_handler)
				wifi_handler(res_buf, len);
			index = 0;
		}
	}
	
	
	switch (data_flag) {
		case ADD_S:
				if (c == '+') {
					data_flag = I_S;
				}
				break;
		case I_S:
				if (c == 'I') {
					data_flag = P_S;
				} else {
					data_flag = ADD_S;
				}
				break;
		case P_S:
				if (c == 'P') {
					data_flag = D_S;
				} else {
					data_flag = ADD_S;
				}
				break;
		case D_S:
				if (c == 'D') {
					data_flag = H_S;
				} else {
					data_flag = ADD_S;
				}
				break;
		case H_S:
				if (c == ',') {
					data_flag = LEN_S;
				} else {
					data_flag = ADD_S;
				}
				break;
		case LEN_S:
			if (c == ':') {
				len_buf[i] = '\0';
				len = atoi(len_buf);
				data_flag = DATA_S;
				i = 0;
				break;
			} else {
				len_buf[i] = c;
			}
			i++;	
			break;
		case DATA_S:
			data_buf[i] = c;
			i++;
			if (i == len) {
				i = 0;
				data_flag = ADD_S;
				if(wifi_handler)
					wifi_handler(data_buf, len);
				break;
			}
			break;
		default:
			break;
	}
		
	if (c == '>') {
		ok_flag = 1;
	}
	if (ok_flag == 0) {
		switch (flag) {
			case O_F:
				if (c == 'O') {
					flag = K_F;
				} else {
					goto end;
				}
				break;
			case K_F:
				if (c == 'K') {
					flag = O_F;
					ok_flag = 1;
				} else {
					flag = O_F;
				}
				break;
			default:
				break;
		}
	}
	end:
	return;
}

void esp8266_init(void)
{
	usart_2_init();
}

int esp8266_link_wifi(char *ssid, char *passwd)
{
	//重启模块
	usart_2_send_data("AT+RST\r\n");//默认在AT指令之后加\r和\n
	delay_ms(2000);
	
	//选择模式
	ok_flag = 0;
	usart_2_send_data("AT+CWMODE=1\r\n");//AP模式
	while (ok_flag == 0);
	
	//链接wifi
	ok_flag = 0;
	usart_2_send_data("AT+CWJAP=");
	usart_2_send_byte(34); // "
	usart_2_send_data(ssid);
	usart_2_send_byte(34); // "
	usart_2_send_byte(44); //,
	usart_2_send_byte(34); // "
	usart_2_send_data(passwd);
	usart_2_send_byte(34); // "
	usart_2_send_data("\r\n");
	while (ok_flag == 0);
	
	//必须读IP，不读的话无法建立连接
	usart_2_send_data("AT+CIFSR\r\n");
	
	ok_flag = 0;
	usart_2_send_data("AT+CIPMUX=0\r\n");
	while (ok_flag == 0);
	
	ok_flag = 0;
	usart_2_send_data("AT+CIPMODE=1\r\n");
	while (ok_flag == 0);
	
	return 0;
}

uint32_t esp8266_connect(char *ip, char *port)
{
	ok_flag = 0;
	usart_2_send_data("AT+CIPSTART=");
	usart_2_send_byte(34); // "
	usart_2_send_data("TCP");
	usart_2_send_byte(34); // "
	usart_2_send_byte(44); //,
	usart_2_send_byte(34); // "
	usart_2_send_data(ip);
	usart_2_send_byte(34); // "
	usart_2_send_byte(44); //,
	usart_2_send_data(port);
	usart_2_send_data("\r\n");
	while (ok_flag == 0);
		
	return 0;
}

uint32_t esp8266_disconnect(void)
{	
	usart_2_send_data("AT+CIPCLOSE\r\n");
	
	return 0;
}

uint32_t esp8266_send(char *msg, char *len)
{
	ok_flag = 0;
	usart_2_send_data("AT+CIPSEND\r\n");
	while (ok_flag == 0); // OK
	
	ok_flag = 0;
	while (ok_flag == 0); // >

	usart_2_send_data(msg); // SEND OK

	delay_ms(700);
	
	usart_2_send_data("+++");
	
	delay_ms(500);
	
	return 0;
}


