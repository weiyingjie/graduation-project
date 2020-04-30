#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x_conf.h"

extern void eeprom_init(void); // 初始化EEPROM

extern void eeprom_byte_write(u8 daddr, u8 waddr, u8 data); // 按字节写

extern void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size); // 按页写

extern u8 eeprom_cur_read(u8 daddr); // 当前地址读

extern u8 eeprom_rand_read(u8 daddr, u8 waddr); // 随机地址读

extern void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size); // 顺序读

#endif


