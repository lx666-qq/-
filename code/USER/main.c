#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "time.h" 
	
u8 Pre_state=0;      	//上一触发时刻状态 1 左来车 2左来人 3 无人/无车
u8 Current_state=3;   	//当前触发时刻状态 1 左来车 2左来人 3 无人/无车 
u8 Current_state_AC=3;		//上一触发时刻状态 1 左来车 2左来人 3 无人/无车 
u8 Pre_state_AC=0;			//当前触发时刻状态 1 左来车 2左来人 3 无人/无车
u8 OC=0;             //判断是否来车
u8 Pre_OC=0;          //判断是否来车，天线
u8 OC_AC=0;             //判断是否来车
u8 Pre_OC_AC=0;          //判断是否来车，天线

 int main(void)
{
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart1_init(57600);	 //串口1初始化为57600
	uart2_init(9600);	 //串口2初始化为9600 
	uart5_init(9600);	 //串口5初始化为9600,收发模式 	
	Laba_Init();//喇叭初始化
	TIM4_Init(8000-1,18000-1);  //定时器4设定周期为2秒
 	while(1)
	{		
		Received_send();
		Received_AC800();
		delay_ms(500);	
	}
}



