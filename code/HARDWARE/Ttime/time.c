#include "time.h"
#include "usart.h"	
#include "led.h"
/*******************************************************************************
* 函 数 名         : TIM4_Init
* 函数功能		   : TIM4初始化函数
* 输    入         : per:重装载值
					 psc:分频系数
* 输    出         : 无
*******************************************************************************/

	
u16 Pre_USART2_Life=0;
u16  SameLife=0; 

extern	u8 Pre_state;      //上一触发时刻状态 1 来向  2去向  3 无人/无车
extern	u8 Current_state;   //当前触发时刻状态 1 来向  2去向  3 无人/无车
extern 	u8 Current_state_AC;		//上一触发时刻状态 1 来向  2去向  3 无人/无车(天线)  
extern 	u8 Pre_state_AC;			//当前触发时刻状态 1 来向  2去向  3 无人/无车(天线)
extern 	u8 OC;             //判断是否来车
extern 	u8 Pre_OC;          //判断是否来车，天线
extern 	u8 OC_AC;             //判断是否来车
extern 	u8 Pre_OC_AC;          //判断是否来车，天线

void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   	//自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 	//分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	TIM_Cmd(TIM4,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	u8 b;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
				OC_AC=2,OC=2;
				if(OC!=Pre_OC)
				{
					Current_state=3;
					for(b=0;b<113;b++)
					{
						USART_SendData(USART1, RX_BUF0[b]);//向串口1发送数据
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束 
					}
					LED1=0;	
					LED2=0;	
					LED3=0;
					LED4=0;	
					LED5=0;	
					LED6=1;	
					LED7=0;
					LED8=0;//注意安全
					Pre_OC=OC;
				}
				if(OC_AC!=Pre_OC_AC)
				{
					Current_state_AC=3;
					for(b=0;b<113;b++)
					{
						USART_SendData(USART1, RX_BUF0[b]);//向串口1发送数据
						while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束 
					}
					LED1=0;	
					LED2=0;	
					LED3=0;
					LED4=0;	
					LED5=0;	
					LED6=1;	
					LED7=0;
					LED8=0;//注意安全
					Pre_OC_AC=OC_AC;
				}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}













