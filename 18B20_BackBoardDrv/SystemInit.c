#include <SystemInit.h>
#include <DelayTime.h>
#include <pc.h>
#include <intrins.h>
#include <string.h>

bit bStartFlag,bInitFlag,bRunModeFlag;
extern char xdata nRecieveData[200];    // 来源于 “DataTrans.c”

void SystemInit(void)
{
	P_SW2 |= 0x80; // EAXFR = 1,才可以访问位于外部RAM区的特殊功能寄存器
	XOSCCR = 0xc0; // 外部晶振控制器，bit7 = 1 使能外部晶振，bit6 = 1 外部时钟源为晶体振荡器，晶振连接到P1.7和P1.6
	while(!(XOSCCR & 0x01)); // 等待外部晶振稳定
	CLKDIV = 0x00; // 系统时钟分频寄存器，0x00代表不分频
	CKSEL = 0x01;  // 系统时钟选择控制寄存器，0x01选择外部晶振
	P_SW2 = 0x00;
	
	P7 = 0xff;     // 断开所有测试板和DUT的电源
	bStartFlag = 0x00;
	bInitFlag = 0x00;
	bRunModeFlag = 0x00;
	
	IT0 = 0x01; // INT0 仅下降沿中断
	IT1 = 0x01; // INT1 仅下降沿中断
	
	EX0 = 0x00;
	EX1 = 0x00;
	EA = 0x01;
	
	P6M0 = 0xff;
	P6M1 = 0x00;
	
	P7M0 = 0xff;
	P7M1 = 0x00;
	
	P1M0 |= 0x02;
	P1M1 &= 0xfd;
}

void Delay100MS(void)
{
	int cunt1 = 0,cunt2 = 0;
	for(cunt1 = 0; cunt1 < 920; cunt1++)
	{
		for(cunt2 = 0; cunt2 < 200; cunt2++)
		_nop_();
	}
}

void ScanPressKey() interrupt 0 using 3 // 按键
{
	EX0 = 0x00; // 关闭串口中断，接下来采用查询方式接收数据
	while(1){
		Delay100MS();
		if(P32){ // 等待数据到
			bStartFlag = 0x01;
			break;
		}
	}
}

void RecievePressKeyCommandFromPC() interrupt 4 using 3 //接收来自电脑端的开始命令，串口1
{
	char nRxDataCunt = 0;
	
	ES = 0x00; // 关闭串口中断，接下来采用查询方式接收数据
	while(1){
		if(RI){ // 等待数据到
			RI = 0;
			
			nRecieveData[nRxDataCunt] = SBUF; // 保存接收到的数据
			if(nRecieveData[nRxDataCunt] == '@')
			break;
			
			nRxDataCunt++;
		}
	}
	
	if((strstr(nRecieveData,"INIT") != NULL) ||
	   (strstr(nRecieveData,"init") != NULL) ||
	   (strstr(nRecieveData,"Init") != NULL)){ 
		bInitFlag = 0x01;  
	}
	   
	if((strstr(nRecieveData,"START") != NULL) ||
	   (strstr(nRecieveData,"Start") != NULL) ||
	   (strstr(nRecieveData,"start") != NULL)){ 
		bStartFlag = 0x01;
	}
	if(strstr(nRecieveData,"RunMode") != NULL){ 
		bRunModeFlag = 0x01;
	}   
	if((strstr(nRecieveData,"STOP") != NULL) ||
	   (strstr(nRecieveData,"Stop") != NULL) ||
	   (strstr(nRecieveData,"stop") != NULL)){ 
		   IAP_CONTR |= 0x20; // 软件复位到用户区从头开始执行代码
	}
	if(strstr(nRecieveData,"NextStart") != NULL){ 
		IAP_CONTR |= 0x20; // 软件复位到用户区从头开始执行代码
	}
}

void StartProcess(void)
{
	START = 0x01;
	Delay_MS(2);
	START = 0x00;
	Delay_MS(5);
	START = 0x01;
	Delay_MS(2);
}

void TrimingBoardCollector(void)
{
	COLLECTOR = 0x01;
	Delay_MS(2);
	COLLECTOR = 0x00;
	Delay_MS(5);
	COLLECTOR = 0x01;
	Delay_MS(2);
}