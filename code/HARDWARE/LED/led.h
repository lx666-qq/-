#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "stm32f10x_rcc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103ZE核心板
//LED驱动代码	

extern u8  RX_BUF0[113];
extern u8  RX_BUF1[113];
extern u8  RX_BUF2[113];
extern u8  RX_BUF3[113];
extern u8  RX_BUF4[113];
extern u8  RX_BUF5[113];
extern u8  RX_BUF6[113];
extern u8  RX_BUF7[113];
extern u8  RX_BUF8[113];
						  
////////////////////////////////////////////////////////////////////////////////// 
#define LED1 PBout(5)// PB5
#define LED2 PBout(6)// PB6
#define LED3 PBout(7)// PB7
#define LED4 PBout(8)// PB8
#define LED5 PBout(9)// PB9
#define LED6 PBout(10)// PB10
#define LED7 PBout(11)// PB11
#define LED8 PBout(12)// PB12
void Laba_Init(void);//初始化

	

//#define OE  PCout(0)// PB5
//#define A   PCout(1)// PB5
//#define B   PCout(2)// PB5
//#define SCK PCout(3)// PB5
//#define LAT PCout(4)// PB5
//#define R1  PDout(5)// PB5
//#define G1  PDout(6)// PB5

#define OE_PIN   GPIO_Pin_12
#define OE_PORT  GPIOE
#define OE_CLK   RCC_APB2Periph_GPIOE
#define A_PIN    GPIO_Pin_11
#define A_PORT   GPIOE
#define A_CLK    RCC_APB2Periph_GPIOE
#define B_PIN    GPIO_Pin_10
#define B_PORT   GPIOE
#define B_CLK    RCC_APB2Periph_GPIOE
#define CLK_PIN  GPIO_Pin_14
#define CLK_PORT GPIOE
#define CLK_CLK  RCC_APB2Periph_GPIOE
#define LAT_PIN  GPIO_Pin_13
#define LAT_PORT GPIOE
#define LAT_CLK  RCC_APB2Periph_GPIOE
#define R1_PIN   GPIO_Pin_13
#define R1_PORT  GPIOD
#define R1_CLK   RCC_APB2Periph_GPIOD
#define G1_PIN   GPIO_Pin_12
#define G1_PORT  GPIOD
#define G1_CLK   RCC_APB2Periph_GPIOD

#define OE_PIN_H  GPIO_SetBits(OE_PORT,OE_PIN)
#define A_PIN_H   GPIO_SetBits(A_PORT,A_PIN)
#define B_PIN_H   GPIO_SetBits(B_PORT,B_PIN)
#define CLK_PIN_H GPIO_SetBits(CLK_PORT,CLK_PIN)
#define LAT_PIN_H GPIO_SetBits(LAT_PORT,LAT_PIN)
#define R1_PIN_H  GPIO_SetBits(R1_PORT,R1_PIN)
#define G1_PIN_H  GPIO_SetBits(G1_PORT,G1_PIN)

#define OE_PIN_L  GPIO_ResetBits(OE_PORT,OE_PIN)
#define A_PIN_L   GPIO_ResetBits(A_PORT,A_PIN)
#define B_PIN_L   GPIO_ResetBits(B_PORT,B_PIN)
#define CLK_PIN_L GPIO_ResetBits(CLK_PORT,CLK_PIN)
#define LAT_PIN_L GPIO_ResetBits(LAT_PORT,LAT_PIN)
#define R1_PIN_L  GPIO_ResetBits(R1_PORT,R1_PIN)
#define G1_PIN_L  GPIO_ResetBits(G1_PORT,G1_PIN)

void LED_MATRIX_GPIO_Init(void);
void LedDisplay(void);
void clearinit1(void);
void clearinit0(void);
void LedDisplay123(void);
void LedDisplay456(void);
void Received_send(void);
void Received_AC800(void);



#endif
