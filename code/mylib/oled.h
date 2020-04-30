#ifndef __OLED_H
#define __OLED_H
/*
用GPIO的方式来模拟IIC协议
这样做的优点是移植性是最好的
通过硬件原理图可知
时钟线	IIC_SCL		PB6
数据线	IIC_SDA		PB7
oled.c 和 oled.h 是用来操作开发板上有关OLED的实验
*/
#include "stdlib.h"//包含了标准库的头文件

typedef unsigned char u8;//通过typedef定义u8类型
typedef unsigned int u32;//通过typedef定义u32类型
     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
extern void OLED_WR_Byte(u8 data, u8 cmd);//IIC为CMD写入一个字节的数据

extern void OLED_Display_On(void);//OLED开启显示

extern void OLED_Display_Off(void);//OLED关闭显示		   		    

extern void OLED_Init(void);//初始化OLED

extern void OLED_Clear(void);//OLED清屏

extern void OLED_DrawPoint(u8 x, u8 y, u8 t);//画一个点

extern void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);//画一个矩形

extern void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);//显示一个字符

extern void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size);//显示一个整数

extern void OLED_ShowString(u8 x, u8 y, u8 *p, u8 Char_Size);//显示一个字符串

extern void OLED_Set_Pos(u8 x, u8 y);//设置像素点的坐标

extern void OLED_ShowTEmp(u8 x, u8 y);//显示汉字温度

extern void OLED_ShowHUm(u8 x, u8 y);//显示汉字湿度

extern void OLED_ShowFIre(u8 x, u8 y);//显示汉字火焰

extern void OLED_ShowTIttle(u8 x, u8 y);//显示摄氏度的符号

extern void OLED_ShowCHinese(u8 x, u8 y);//显示汉字

extern void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);//显示图片

extern void fill_picture(u8 fill_Data);//填充图片

#endif  
	 



