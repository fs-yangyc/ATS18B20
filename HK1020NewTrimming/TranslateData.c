#include <TranslateData.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char nSendData[30];
char nRxData[200]; //接收来自总控制MCU发来的数据
bit bBoardIsHere,bTestEndFlag;
int VBE;
int Rotation;
unsigned char Version;

void UartOneInit(char nBtr){	
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xfe;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0f;		//设定定时器1为16位自动重装方式
	
	if(nBtr == BTR115200){
		TL1 = 0xd0;			//设定定时初值
		TH1 = 0xff;			//设定定时初值
	}
	
	if(nBtr == BTR57600){
		TL1 = 0xa0;			//设定定时初值
		TH1 = 0xff;			//设定定时初值
	}
	
	if(nBtr == BTR38400){
		TL1 = 0x70;			//设定定时初值
		TH1 = 0xff;			//设定定时初值
	}
	
	if(nBtr == BTR19200){
		TL1 = 0xe0;			//设定定时初值
		TH1 = 0xfe;			//设定定时初值
	}
	
	if(nBtr == BTR9600){
		TL1 = 0xc0;			//设定定时初值
		TH1 = 0xfd;			//设定定时初值
	}
	
	if(nBtr == BTR4800){
		TL1 = 0x80;			//设定定时初值
		TH1 = 0xfb;			//设定定时初值
	}
	
	if(nBtr == BTR2400){
		TL1 = 0x00;			//设定定时初值
		TH1 = 0xf7;			//设定定时初值
	}
	
	if(nBtr == BTR1200){
		TL1 = 0x00;			//设定定时初值
		TH1 = 0xee;			//设定定时初值
	}
	
	ET1 = 0;			//禁止定时器1中断
	TR1 = 1;			//启动定时器1
}

/**
*** 函数名称 ： SendTempDataToContrlMcu
*** 函数功能 ： 以十六进制的形式发送字符串到总控制MCU
*** 入口参数 ： 1、*pIntData指向待发送的整型数组
***             2、nIntDataCunt发送数据的个数
*** 返回值   ： 无
*/
void SendTempDataToContrlMcu(int *pIntData,unsigned char nIntDataCunt) 
{
	unsigned char nCunt = 0,nLowByte;
	char nHighByte;
	int nTempIntData = 0;
	unsigned char nRxDataCunt = 0;
	
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			if(SBUF == 0x02) // 总控制MCU请求发送温度的命令0x02
			break;
		}
	}
	
	TI = 0;
	for(nCunt = 0; nCunt < nIntDataCunt; nCunt++){
		nTempIntData = pIntData[nCunt];
		nLowByte = nTempIntData;
		nHighByte = nTempIntData >> 0x08;
		
		SBUF = nHighByte;
		while(TI != 1);
		TI = 0;
		
		SBUF = nLowByte;
		while(TI != 1);
		TI = 0;
	}
}

void RxDataInterrupt() interrupt 4 using 3 //接收来自总控制MCU发来的数据，串口1
{
	unsigned char nRxDataCunt = 0;
	ES = 0x00; // 关闭串口中断，接下来采用查询方式接收数据
	while(1){
		if(RI){ // 等待数据到
			RI = 0;
			
			nRxData[nRxDataCunt] = SBUF; // 保存接收到的数据
			if(nRxData[nRxDataCunt] == '@')
			break;
			
			nRxDataCunt++;
		}
	}
	
	if(strstr(nRxData,"IS?") != NULL){ // 总控制板询问本板是否存在,
		TI = 0;
		SBUF = 'Y'; // 发送存在响应
		while(!TI);
		TI = 0;
		
		bBoardIsHere = 0x01;
	}
	if(strstr(nRxData,"END") != NULL){ // 测试过程完成
		bTestEndFlag = 0x01;
	}
	
	else{
		bTestEndFlag = 0x00;
	}
}

/**
*** 函数名称 ： GetPt100TempData
*** 函数功能 ： 解析来自pt100的测温数据,共8个数据
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void GetPt100TempData(float *pPt100Data)
{
	char nDataCunt = 0,nBitCunt = 0;
	unsigned char nRxDataCunt = 0;
	char nTempRxData[10];
	
	memset(nTempRxData,'\0',sizeof(nTempRxData));
	for(nDataCunt = 0; nDataCunt < 4; nDataCunt++){
		pPt100Data[nDataCunt] = 0.0;
	}
	
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			nRxData[nRxDataCunt] = SBUF;
			if(nRxData[nRxDataCunt] == '@')
				break;
			
			nRxDataCunt++;
		}
	}
	
	nRxDataCunt = 0;
	if(strstr(nRxData,"PT") != NULL){
		while(1){
			if(nRxData[nRxDataCunt] == ',')
			break;
			
			nRxDataCunt++;
		}
		nRxDataCunt++;
		
		for(nDataCunt = 0; nDataCunt < 4; nDataCunt++)
		{
			nBitCunt = 0;
			while(1){
				if((nRxData[nRxDataCunt] == ',') || (nRxData[nRxDataCunt] == '@'))
				break;
				
				nTempRxData[nBitCunt] = nRxData[nRxDataCunt];
				nBitCunt++;
				nRxDataCunt++;
			}
			nRxDataCunt++;
			pPt100Data[nDataCunt] = atof(nTempRxData);
			memset(nTempRxData,'\0',sizeof(nTempRxData));
		}
	}
}

/**
*** 函数名称 ： SendTrimingProcessEnd
*** 函数功能 ： 向总控制板发送修调过程完成
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void SendTrimingProcessEnd(void) 
{
	RI = 0x00;
	TI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			if(SBUF == '%'){
				SBUF = '%';
				while(!TI);
				TI = 0x00;
				break;
			}
		}
	}
}

void GetChipPackageType(char *nPackageType)
{
	char nDataCunt = 0,nBitCunt = 0,nPkgData[10];
	unsigned char nRxDataCunt = 0;
	
	memset(nRxData,'\0',sizeof(nRxData));
	memset(nPkgData,'\0',sizeof(nPkgData));
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			nRxData[nRxDataCunt] = SBUF;
			if(nRxData[nRxDataCunt] == '@')
				break;
			
			nRxDataCunt++;
		}
	}
	
	nRxDataCunt = 0;
	if(strstr(nRxData,"PKG") != NULL){ // PWR,0@
		while(1){
			if(nRxData[nRxDataCunt] == ',')
			break;
			
			nRxDataCunt++;
		}
		nBitCunt = 0;
		nRxDataCunt++;
		
		while(1){
			if(nRxData[nRxDataCunt] == '@')
				break;
			
			nPkgData[nBitCunt] = nRxData[nRxDataCunt];
			nBitCunt++;
			nRxDataCunt++;
		}
		
		*nPackageType = atoi(nPkgData);
	}
}
//VBE,0xff,0x00@
void GetVBEandRotationValues(void)
{
	char nDataCunt = 0,nBitCunt = 0;
	unsigned char nRxDataCunt = 0;
	char nVBEData[10];
	
	memset(nRxData,'\0',sizeof(nRxData));
	memset(nVBEData,'\0',sizeof(nVBEData));
	
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			nRxData[nRxDataCunt] = SBUF;
			if(nRxData[nRxDataCunt] == '@')
				break;
			
			nRxDataCunt++;
		}
	}
	
	nRxDataCunt = 0;
	if(strstr(nRxData,"B") != NULL){
		Version = Version_B;
	}
	else if(strstr(nRxData,"C") != NULL){
		Version = Version_C;
	}
	while(1){
		if(nRxData[nRxDataCunt] == ',')
		break;
		
		nRxDataCunt++;
	}
	nRxDataCunt++;
	
	for(nDataCunt = 0; nDataCunt < 2; nDataCunt++)
	{
		nBitCunt = 0;
		while(1){
			if((nRxData[nRxDataCunt] == ',') || (nRxData[nRxDataCunt] == '@'))
			break;
			
			nVBEData[nBitCunt] = nRxData[nRxDataCunt];
			nBitCunt++;
			nRxDataCunt++;
		}
		nRxDataCunt++;
		if(nDataCunt == 0){
			VBE = atoi(nVBEData);
		}
		else{
			Rotation = atoi(nVBEData);
		}
		memset(nVBEData,'\0',sizeof(nVBEData));
	}
}