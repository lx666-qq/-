#include "ledshow.h"
#include "ledfont.h"
#include "delay.h"

u8 LED_GRAM[8][16];//缓冲区

//显示一个字节
//dat数据 e=0上半区 e=0下半区 
void LED_WRX_Byte(u8 dat,u8 e)//
{
	 u8 i;
	 for(i=0;i<8;i++)
	 {
			CLK=0;
		  if(e==0)
				R1=dat&0x01;
			if(e==1)
				R2=dat&0x01;
			CLK=1;
			STB=1;
	    STB=0;
			dat=dat>>1;
	 }
}

//刷新显示
//从上到下按行刷新
void LED_WRY_Byte(void)
{
	 u8 yNUM,E,xNUM;
	 // ?**列数改为16**
	 for(yNUM=0;yNUM<16;yNUM++)//列刷新
	 {
			A=(yNUM<<7)>>7;  // A=yNUM (0-15)
			B=(yNUM<<6)>>7;  // B=yNUM//2 (0-7)
			C=(yNUM<<5)>>7;  // C=yNUM//4 (0-3)
			D=(yNUM<<4)>>7;  // D=yNUM//8 (0-1)
			// ?**始终上半区**
			E=0; // ?**删除原条件判断**
			STB=0;
			OE=1;
			for(xNUM=0;xNUM<8;xNUM++)
			{
				LED_WRX_Byte(LED_GRAM[xNUM][yNUM],E);
			}
	    STB=1;
	    STB=0;
	    OE=0;
			delay_us(400);
			OE=1;		
	 }
}

//画一个点，其他显示函数的基础
//x,y坐标
//t=1亮 t=0不亮
void LED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;// ?**修改范围检查为32列16行**
	if(x>31||y>15)return;//超出范围了.
	pos=x/8;
	bx=x%8;
	temp=1<<bx;
	if(t)LED_GRAM[pos][y]|=temp; 
	else LED_GRAM[pos][y]&=~temp;
}
void LED_Clear(void)
{
	int i,j;
	// ?**修改循环范围为16行**
	for(i=0;i<16;i++)  // 原64→16
	{
		for(j=0;j<32;j++)  // 列保持不变
		{
			LED_DrawPoint(i,j,0);
		}
	}
	LED_WRY_Byte();
}

void LED_Fill(void)
{
	int i,j;
	// ?**修改循环范围为16行**
	for(i=0;i<32;i++)  // 原64→16
	{
		for(j=0;j<16;j++)  // 列保持不变
		{
			LED_DrawPoint(i,j,1);
		}
	}
	LED_WRY_Byte();
}

//显示字符
//x,y坐标
//chr需显示的字符
//size字符大小
//mode需显示为1
void LED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1205[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1607[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2411[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LED_DrawPoint(x,y,mode);
			else LED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//显示中文
//x,y左上角坐标
//num字库第几个汉字
//size字符大小
//mode需点亮1
void LED_ShowChinese(u8 x,u8 y,u8 num,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*size;		//得到字体一个字符对应点阵集所占的字节数 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=chinese_11[num][t]; 	 	//调用1206字体
		else if(size==16)temp=chinese_15[num][t];	//调用1608字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LED_DrawPoint(x,y,mode);
			else LED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void LED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void LED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
//        if(x>(128-(size/2))){x=0;y+=size;}
//        if(y>(64-size)){y=x=0;LED_Clear();}
        LED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	


