#ifndef __USART2_H
#define __USART2_H
/*
TX	PA2
RX	PA3
*/
#include "stm32f10x_conf.h"

typedef void (*usart_2_handler)(unsigned char );

extern void usart_2_init(void);

extern void usart_2_send_byte(char c);

extern void usart_2_send_data(char *buf);

extern unsigned char usart_2_recv_byte(void);

extern void set_usart_2_handler(usart_2_handler h);

#endif









