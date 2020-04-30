#include "eeprom.h"
#include "gpio_iic.h"
#include "stdlib.h"



void eeprom_init(void) // 初始化EEPROM
{
	IIC_Init();
}

void eeprom_byte_write(u8 daddr, u8 waddr, u8 data) // 按字节写
{
	IIC_Start();
	IIC_Send_Byte(daddr); // 发送设备地址 + 写标志
	IIC_Wait_Ack();
	IIC_Send_Byte(waddr); // 发送word address
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void eeprom_page_write(u8 daddr, u8 waddr, u8 *buf, u8 size) // 按页写
{
	u8 i = 0; // 循环变量
	
	IIC_Start();
	IIC_Send_Byte(daddr); // 发送设备地址 + 写标志
	IIC_Wait_Ack();
	IIC_Send_Byte(waddr);
	IIC_Wait_Ack();
	for(i = 0; i < size; i++) {
		IIC_Send_Byte(buf[i]);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}

u8 eeprom_cur_read(u8 daddr) // 当前地址读
{
	u8 data = 0; // 存储接收的数据
	
	IIC_Start();
	IIC_Send_Byte(daddr | 1);
	IIC_Wait_Ack();
	data = IIC_Recv_Byte(0); // 读数据，不要ACK
	IIC_Stop();
	
	return data;
}

u8 eeprom_rand_read(u8 daddr, u8 waddr) // 随机地址读
{
	u8 data = 0; // 存储接收的数据
	
	IIC_Start();
	IIC_Send_Byte(daddr); // 发送设备地址 + 写标志
	IIC_Wait_Ack();
	IIC_Send_Byte(waddr); // 发送word address
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(daddr | 1); // 发送设备地址 + 读标志
	IIC_Wait_Ack();
	data = IIC_Recv_Byte(0); // 读一字节数据，不要ACK
	IIC_Stop();
	
	return data;
}

void eeprom_sequ_read(u8 daddr, u8 waddr, u8 *buf, u8 size) // 顺序读
{
	u8 i = 0; // 循环变量
	
	IIC_Start();
	IIC_Send_Byte(daddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(waddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(daddr | 1);
	IIC_Wait_Ack();
	for(i = 0; i < size-1; i++) {
		buf[i] = IIC_Recv_Byte(1); // 读数据，有ACK
	}
	buf[i] = IIC_Recv_Byte(0);
	IIC_Stop();
}

