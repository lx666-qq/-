#include "sys.h"
#include "usart.h"	  
#include "led.h"
#include "delay.h"

float V_Tar=0; 
float V_Tar_AC=0;
u8  COM_GO_FLAG=0;
u8  COMEGO_FLAG=0;
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	  
void uart1_init(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);			//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;		   //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); 	  //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
			Res =USART_ReceiveData(USART1);	//读取接收到的数据
			if((USART1_RX_STA&0x8000)==0)//接收未完成
			{
				if(USART1_RX_STA&0x4000)//接收到了0x0d
				{
					if(Res!=0x0a)USART1_RX_STA=0; //接收错误,重新开始
					else USART1_RX_STA|=0x8000;	  //接收完成了 
				}
			else //还没收到0X0D
				{	
					if(Res==0x0d)USART1_RX_STA|=0x4000;
					else
					{
						USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
						USART1_RX_STA++;
						//接收数据错误,重新开始接收
						if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;	  
					}		 
				}
			}   		 
     } 

} 
#endif	 
 

#if EN_USART2_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART2_RX_STA=0;       //接收状态标记	  
u8  FilterNum=0;
u8  Buffer[20]={0};
u16  USART2_Life=0;

void uart2_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟	//使能USART1，GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
    //USART1_RX  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1 
}

void USART2_IRQHandler(void)//串口1中断服务程序
{
	u8 Res2;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	//接收中断(接收到的数据必须是0x0d 0x0a结尾)
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res2 =USART_ReceiveData(USART2);	//读取接收到的数据
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res2!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res2==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res2 ;
					USART2_RX_STA++;
					//接收数据错误,重新开始接收
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;	  
				}		 
			}
		}
	} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
		
} 
#endif	
////////////////////////////////////////////////////////////////////////////////// 
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART3_RX_STA=0;       //接收状态标记	  
  
void uart3_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能GPIOB时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO复用时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10_USART3_TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11_USART3_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11


	//USART 初始化设置
	USART_DeInit(USART3); //复位串口1
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1	

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断

  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

void USART3_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res3;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res3 =USART_ReceiveData(USART3);	//读取接收到的数据
		
		if((USART3_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res3!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res3==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res3 ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

////////////////////////////////////////////////////////////////////////////////// 
#if EN_USART5_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART5_RX_BUF[USART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART5_RX_STA=0;       //接收状态标记	  
void uart5_init(u32 bound)
{
			//GPIO端口设置
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure;
			NVIC_InitTypeDef NVIC_InitStructure;	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//使能GPIOD时钟

			//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//使能AFIO复用时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能UART5时钟

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //PC12
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
			GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC12

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
			GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD2


			//USART 初始化设置
			USART_DeInit(UART5); //复位串口4
			USART_InitStructure.USART_BaudRate = bound;//串口波特率
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
			USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
			USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

			//Uart5 NVIC 配置
			NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口4中断
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级2
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
			NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
			
			USART_Init(UART5,&USART_InitStructure);//初始化串口5
			USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
			USART_Cmd(UART5, ENABLE);                    //使能串口5
	}


void UART5_IRQHandler(void)                	//串口4中断服务程序
	{
	u8 Res5;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res5 =USART_ReceiveData(UART5);	//读取接收到的数据
		
		if((USART5_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART5_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res5!=0x0a)USART5_RX_STA=0;//接收错误,重新开始
				else USART5_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res5==0x0d)USART5_RX_STA|=0x4000;
				else
					{
					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res5 ;
					USART5_RX_STA++;
					if(USART5_RX_STA>(USART5_REC_LEN-1))USART5_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
		} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	
