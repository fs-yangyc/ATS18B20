#include <SystemInit.h>
#include <DelayTime.h>
#include <intrins.h>

bit bRunStopFlag = 0x00;
bit bStartFlag = 0x00;

extern bit bBoardIsHere,bTestEndFlag;

void SystemInit(void)
{
	P_SW2 |= 0x80; // EAXFR = 1,才可以访问位于外部RAM区的特殊功能寄存器
	XOSCCR = 0xc0; // 外部晶振控制器，bit7 = 1 使能外部晶振，bit6 = 1 外部时钟源为晶体振荡器，晶振连接到P1.7和P1.6
	while(!(XOSCCR & 0x01)); // 等待外部晶振稳定
	CLKDIV = 0x00; // 系统时钟分频寄存器，0x00代表不分频
	CKSEL = 0x01;  // 系统时钟选择控制寄存器，0x01选择外部晶振
	P_SW2 = 0x00;
	
	P0 =  0x00; 
	P1 &= 0xc0;
	P2 =  0x00;
	P4 &= 0xe0;
	P5 &= 0xe0;
	P6 =  0x00;
	P7 =  0x00;
	
	P3M0 |= 0x02;
	P3M1 &= 0xfd; // P3.1需要强推，见芯片说明,P34-P37做power驱动用了，需要强推


	IT0 = 0x01; // INT0 仅下降沿中断
	IT1 = 0x01; // INT1 仅下降沿中断
	
	P_SW1 = 0x00; //RxD = P3.0,TxD = P3.1
	
	EX0 = 0x00;
	EX1 = 0x00;
	ES = 0x00;
	EA = 0x01;
	
	bBoardIsHere = 0x00;
	bTestEndFlag = 0x00;
	bRunStopFlag = 0x00;
	bStartFlag = 0x00;
}

void Ext1Interrupt() interrupt 2 using 1  
{
	EX1 = 0x00;                        
	while(1)
	{	
		if(DataCollect)
		{
			bRunStopFlag = 0x01;
			break;
		}
	}
}

void Ext0Interrupt() interrupt 0 using 1  
{
	EX0 = 0x00;                        
	while(1)
	{	
		if(ScanStart)
		{
			bStartFlag = 0x01;
			break;
		}
	}
}