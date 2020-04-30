#include "sht.h"
#include "gpio_iic.h"
#include "delay.h"

void sht_init(void)//初始化的函数
{
	IIC_Init();
}

void sht_write_mode(void)//配置SHT30传感器内部采样速率
{
	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR); // 发送设备地址 + 写标志
	while(IIC_Wait_Ack());
	IIC_Send_Byte(PERIODIC_MODE_HIGH >> 8); // 发送指令的高8位
	while(IIC_Wait_Ack());
	IIC_Send_Byte(PERIODIC_MODE_HIGH & 0xFF); // 发送指令的低8位
	while(IIC_Wait_Ack());
	IIC_Stop();
	delay_ms(5);
}

void sht_write_read_cmd(void)//发送读取SHT30采集数据的命令
{
	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR); // 发送设备地址 + 写标志
	while(IIC_Wait_Ack());
	IIC_Send_Byte(READOUT_PERIODIC_MODE >> 8); // 发送指令的高8位
	while(IIC_Wait_Ack());
	IIC_Send_Byte(READOUT_PERIODIC_MODE & 0xFF); // 发送指令的低8位
	while(IIC_Wait_Ack());
}

void sht_read_data(double sht_data[2])//读取SHT30采集到的数据
{
	u8 buf[6] = {0};//存储SHT30读出的数据
	u8 i = 0;//i变量为循环变量
	u16 temp = 0, hum = 0;
	
	IIC_Start();
	IIC_Send_Byte(SLAVE_ADDR | 1);
	delay_us(5);
	while(IIC_Wait_Ack());
	for(i = 0; i < 6; i++)
	{
		if(i == 5)
			buf[i] = IIC_Recv_Byte(0);//读数据，不需要产生ACK
		else
			buf[i] = IIC_Recv_Byte(1);//读数据，产生ACK
	}
	IIC_Stop();
	
	// 转换数据
	temp = (buf[0] << 8) | buf[1];
	sht_data[0] = (double)(-45 + 175 * (double)temp / 65535);
	hum = (buf[3] << 8) | buf[4];
	sht_data[1] = (double)(100 * (double)hum / 65535);
}

