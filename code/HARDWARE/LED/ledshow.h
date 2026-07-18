#ifndef __LEDSHOW_H
#define __LEDSHOW_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//LED端口定义
#define A PBout(0)	// A
#define B PBout(1)	// B
#define C PBout(2)	// C
#define D PEout(5)	// D	
#define R1 PBout(5)	// R1
#define R2 PEout(1)	// R2
#define STB PBout(6)	// STB
#define CLK PBout(7)	// CLK
#define OE PEout(0)	// OE

void LED_Init(void);	//初始化
void LED_WRY_Byte(void);//刷新
void LED_Clear(void);//清空显示
void LED_Fill(void);//铺满显示
void LED_DrawPoint(u8 x,u8 y,u8 t);//画点
void LED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);//显示字符
void LED_ShowChinese(u8 x,u8 y,u8 num,u8 size,u8 mode);//显示汉字
void LED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);//显示数字
void LED_ShowString(u8 x,u8 y,const u8 *p,u8 size);//显示字符串
#endif


