#ifndef __USART1_H
#define __USART1_H
/*
串口USART1
RX	PA10	接收	只需要配置成浮空输入
TX	PA9		发送	需要配置成复用推挽
*/

#include "stm32f10x_conf.h"
#define BUFSIZE	128

typedef void (*usart1_handler)(char *);

extern void usart_1_init(void); // 初始化USART1

extern void usart_1_send_byte(unsigned char c); // 发送数据

extern unsigned char usart_1_recv_byte(void); // 接收数据

extern void usart_1_send_data(char *buf); // 发送字符串

extern void set_usart_1_handler(usart1_handler h);

#endif

