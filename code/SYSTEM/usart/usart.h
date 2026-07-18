#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F103ZE核心板
//串口1初始化		   

//********************************************************************************
#define USART1_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART1_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);


/////////////////////////////////////////////////////////////////////////////////////////////
#define USART2_REC_LEN  			250  	//定义最大接收字节数 200
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	
extern u8  FilterNum;
//如果想串口中断接收，请不要注释以下宏定义
void uart2_init(u32 bound);
/////////////////////////////////////////////////////////////////////////////////////////////
extern float V_Tar;
extern float V_Tar_AC;
extern u8   COM_GO_FLAG;
extern u8   COMEGO_FLAG;
extern u8   Buffer[20];
extern u16  USART2_Life;



/////////////////////////////////////////////////////////////////////////////////////////////
#define USART3_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart3_init(u32 bound);
/////////////////////////////////////////////////////////////////////////////////////////////
#define USART5_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART5_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART5_RX_BUF[USART5_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART5_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart5_init(u32 bound);
/////////////////////////////////////////////////////////////////////////////////////////////


#endif

