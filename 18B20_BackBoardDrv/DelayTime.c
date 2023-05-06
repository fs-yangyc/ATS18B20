#include <DelayTime.h>

char nRunStopCunt = 0;

unsigned char nTimer0Cunt = 0;

void Delay_MS(unsigned int nDelayMs) //22.1184MHz 时钟下面延时1ms，示波器下面观察的结果
{
	int nDelayCunt = 0;

	AUXR |= 0x80;	//定时器0时钟1T模式
	TMOD &= 0xF0;	//设置定时器模式
	
	TL0 = 0xCD;		//设置定时初值
	TH0 = 0xD4;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0  = 0x01;
	TR0  = 0x01;
	
	while(1){
		if(nTimer0Cunt == 2){
			nTimer0Cunt = 0;
			nDelayCunt++;
		}

		if(nDelayCunt == nDelayMs) // 计时时间到或者是按了Stop键
		{
			TR0  = 0x00;
			ET0  = 0x00;
			break;
		}
	}
}

void Delay_5US(void)
{
	nTimer0Cunt = 0;
	
	AUXR |= 0x80;		//定时器0时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x91;		//设置定时初值
	TH0 = 0xFF;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0  = 0x01;
	TR0  = 0x01;
	
	while(1)
	{
		if(nTimer0Cunt == 1)
		{
			TR0  = 0x00;
			ET0  = 0x00;
			nTimer0Cunt = 0;
			break;
		}
	}
}
/****************************
void Delay_60US(void)
{
	nTimer0Cunt = 0;
	
	AUXR |= 0x80;		//定时器0时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xD1;		//设置定时初值
	TH0 = 0xFA;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	
	ET0  = 0x01;
	TR0  = 0x01;
	
	while(1)
	{
		if(nTimer0Cunt == 1)
		{
			TR0  = 0x00;
			ET0  = 0x00;
			nTimer0Cunt = 0;
			break;
		}
	}
}
*****************************/
void Timer0Interrupt (void) interrupt 1 using 3
{
	nTimer0Cunt++;
}