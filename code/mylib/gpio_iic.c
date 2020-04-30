#include "gpio_iic.h"
#include "delay.h"
#include "bitband.h"

/*
用GPIO的方式模拟IIC总线这种方法,和硬件连接的GPIO有关
我的设备接到不同的GPIO上,将来就需要配置响应的GPIO
因为我们做的实验有OLED EEPROM SHT30,而这些设备都是连接在了相同的管脚上
所以,在使用时从设备地址一定不要写错
*/

//以下代码是硬件相关的，移植时候需要修改
GPIO_InitTypeDef  GPIO_InitForSCL;
GPIO_InitTypeDef  GPIO_InitForSDAOut;
GPIO_InitTypeDef  GPIO_InitForSDAIn;

#define IIC_SCL					PBOut(6)
#define IIC_SDA					PBOut(7)
#define IIC_SDA_STATUS 	PBIn(7)

void IIC_Init(void)
{			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//PB6 SCL
	GPIO_InitForSCL.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitForSCL.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitForSCL.GPIO_Speed = GPIO_Speed_50MHz;
	
	//PB7 SDA OUT
	GPIO_InitForSDAOut.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitForSDAOut.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitForSDAOut.GPIO_Speed = GPIO_Speed_50MHz;
	
	//PB7 SDA IN
	GPIO_InitForSDAIn.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitForSDAIn.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitForSDAIn.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);
	GPIO_Init(GPIOB, &GPIO_InitForSCL);
	
	IIC_SCL = 1;
	IIC_SDA = 1;
}

void IIC_Sda_In(void)
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAIn);
}

void IIC_Sda_Out(void)
{
	GPIO_Init(GPIOB, &GPIO_InitForSDAOut);
}
//以上代码是硬件相关的，移植时候需要修改

//以下代码硬件无关，只跟iic协议有关
void IIC_Start(void)
{
	IIC_Sda_Out();
	IIC_SDA=1;	  	  
	IIC_SCL=1;  
	delay_us(2);
 	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=0;
}	  

void IIC_Stop(void)
{
	IIC_Sda_Out();
	IIC_SCL=0;
	IIC_SDA=0;
 	delay_us(2);
	IIC_SCL=1; 
	delay_us(1);
	IIC_SDA=1;
	delay_us(2);							   	
}

u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	IIC_Sda_In();
	IIC_SCL = 0;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);
	while(IIC_SDA_STATUS) {
		ucErrTime++;
		if(ucErrTime>250) {
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0; 	   
	return 0;  
} 

void IIC_Ack(void)
{
	IIC_SCL=0;
	IIC_Sda_Out();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
    
void IIC_NAck(void)
{
	IIC_SCL=0;
	IIC_Sda_Out();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     

void IIC_Send_Byte(u8 txd)
{                        
	u8 t;  
	
	IIC_Sda_Out();    

	for (t = 0;t < 8; t++) {  
		IIC_SCL = 0;
		delay_us(1);	
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1; 	  
		delay_us(1);   
		IIC_SCL=1;
		delay_us(2); 
	}	
	IIC_SCL = 0;
} 	    

u8 IIC_Recv_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	IIC_Sda_In();
	for(i=0;i<8;i++ ) {
		IIC_SCL=0; 
		delay_us(2);
		IIC_SCL=1;
		delay_us(2);
		receive<<=1;
		if(IIC_SDA_STATUS)
			receive |= 1;   
	}						 
	IIC_SCL = 0;
	if (!ack)
		IIC_NAck();
	else
		IIC_Ack(); 
	return receive;
}
