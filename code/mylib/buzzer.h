#ifndef __BUZZER_H
#define __BUZZER_H

/*
PC7高电平触发
*/

#include "stm32f10x_conf.h"

extern void buzzer_init(void);

extern void buzzer_on(void);

extern void buzzer_off(void);

#endif

