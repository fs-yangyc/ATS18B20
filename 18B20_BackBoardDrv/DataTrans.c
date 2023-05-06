#include <DataTrans.h>
#include <DelayTime.h>
#include <pc.h>
#include <stdio.h>
#include <string.h>

extern char nBoardIsHerePwr[8];   // 来源于 “TstBrdContrl.c”
	
char xdata nRecieveData[200],nSendData[20],nTempP6PortData,nDeviceIndex,nRxDataCunt;
char nOtherDeviceName[8] = {E3631A,CFG,YSL_CHAMBER,GWS_CHAMBER,RS485_1,RS485_2,RS485_3,RS232_1};
char nTestBoardName[8] = {TSTBRD_1,TSTBRD_2,TSTBRD_3,TSTBRD_4,TSTBRD_5,TSTBRD_6,TSTBRD_7,TSTBRD_8};
unsigned int nComTimeOutCunt; // 记录串口超时

extern unsigned char Version;

/**
*** 函数名称 ： UsartInit
*** 函数功能 ： 初始化串口到指定的波特率
*** 入口参数 ： 1、nBtr 波特率
*** 返回值   ： 无
*/
void UsartInit(char nComPortIndex,char nBtr)
{ 
	if(nComPortIndex == ComPort_1){ // COM1
		SCON = 0x50;		//8位数据,可变波特率
		AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
		AUXR &= 0xfe;		//串口1选择定时器1为波特率发生器
		TMOD &= 0x0f;		//设定定时器1为16位自动重装方式
	
		if(nBtr == BTR115200){TL1 = 0xd0;TH1 = 0xff;}
		if(nBtr == BTR57600) {TL1 = 0xa0;TH1 = 0xff;}
		if(nBtr == BTR38400) {TL1 = 0x70;TH1 = 0xff;}
		if(nBtr == BTR19200) {TL1 = 0xe0;TH1 = 0xfe;}
		if(nBtr == BTR9600)  {TL1 = 0xc0;TH1 = 0xfd;}
		if(nBtr == BTR4800)  {TL1 = 0x80;TH1 = 0xfb;}
		if(nBtr == BTR2400)  {TL1 = 0x00;TH1 = 0xf7;}
		if(nBtr == BTR1200)  {TL1 = 0x00;TH1 = 0xee;}
	
		ET1 = 0;			//禁止定时器1中断
		TR1 = 1;			//启动定时器1
	}
	
	if(nComPortIndex == ComPort_2){ // COM2
		S2CON = 0x50;
		if(nBtr == BTR115200){T2L = 0xd0;T2H = 0xff;}
		if(nBtr == BTR57600) {T2L = 0xa0;T2H = 0xff;}
		if(nBtr == BTR38400) {T2L = 0x70;T2H = 0xff;}
		if(nBtr == BTR19200) {T2L = 0xe0;T2H = 0xfe;}
		if(nBtr == BTR9600)  {T2L = 0xc0;T2H = 0xfd;}
		if(nBtr == BTR4800)  {T2L = 0x80;T2H = 0xfb;}
		if(nBtr == BTR2400)  {T2L = 0x00;T2H = 0xf7;}
		if(nBtr == BTR1200)  {T2L = 0x00;T2H = 0xee;}
	
		AUXR |= 0x14; // 这里不能直接赋值0x14，否者串口1会停止工作
	}
}

/**
*** 函数名称 ： UsartInterrupt
*** 函数功能 ： 使能或者关闭串口中断
*** 入口参数 ： 1、nEnbDisable 使能或者关闭
*** 返回值   ： 无
*/
void UsartInterrupt(char nComPortIndex,char nEnbDisable)
{
	if(nComPortIndex == ComPort_1){
		if(nEnbDisable == COMENB){
			ES = 0x01;      // Enable Com1
		}
		
		else{
			ES = 0x00;      // Disable Com1
		}
	}
	
	if(nComPortIndex == ComPort_2){
		if(nEnbDisable == COMENB){
			IE2 |= 0x01;	// Enable Com2
		}
		
		else{
			IE2 &= 0xfe;	// Disable Com2
		}
	}
}

/**
*** 函数名称 ： SetUartPosionAndBtr
*** 函数功能 ： 改变串口的位置和波特率
*** 入口参数 ： 1、nSerialPortPosion 串口映射的引脚位置
***             2、nDeviceName 与串口通信的设备名称
***             3、nBtr 波特率
*** 返回值   ： 无
*/
void SetUsartTwoPosionAndBtr(char nSerialPortPosion,char nDeviceName,char nBtr)
{ 
	UsartInit(ComPort_2,nBtr);
	nTempP6PortData = P6; 			// 读取P6端口的状态
		
	if(nSerialPortPosion == Com2_Other){ // 与巡检仪、温箱、E3631A等通信
		P_SW2 |= 0x01;
			
		nTempP6PortData &= 0x1f; 	// 清零P6端口的高三位：P6.5 P6.6 P6.7
		for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
			if(nOtherDeviceName[nDeviceIndex] == nDeviceName){
				nTempP6PortData |= nOtherDeviceName[nDeviceIndex];
				break;
			}
		}
	}
	
	if(nSerialPortPosion == Com2_TstBrd){ // 与测试板通信
		P_SW2 &= 0xfe;
			
		nTempP6PortData &= 0xf8; 	// 清零P6端口的低三位：P6.0 P6.1 P6.2
		for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
			if(nTestBoardName[nDeviceIndex] == nDeviceName){
				nTempP6PortData |= nTestBoardName[nDeviceIndex];
				break;
			}
		}
	}
		
	P6 = nTempP6PortData; // 设置P6端口的状态
	Delay_MS(1);
}

/**
*** 函数名称 ： SendStringData
*** 函数功能 ： 以ASCII码的形式发送字符串
*** 入口参数 ： 1、*pData指向待发送的数组
*** 返回值   ： 无
*/
void SendStringData(char nComPortIndex,char *pData)
{
	if(nComPortIndex == ComPort_1){ // COM1
		while(*pData){
			SBUF = *pData++;
			while(TI != 1);
			TI = 0;
		}
	}
	
	if(nComPortIndex == ComPort_2){ // COM2
		while(*pData){
			S2BUF = *pData++;
			while((S2CON & 0x02) != 0x02);  // S2TI == 1
			S2CON &= 0xfd; 					// S2TI = 0
		}
	}
}

/**
*** 函数名称 ： SendFloatData
*** 函数功能 ： 发送浮点型数
*** 入口参数 ： 1、*pFloatData指向待发送的浮点型数组
***             2、nFloatDataCunt 待发送的浮点型数的个数
*** 返回值   ： 无
*/
void SendFloatData(char nComPortIndex,float *pFloatData,unsigned int nFloatDataCunt)
{
	unsigned int nCunt = 0;
	char *pData;
	
	for(nCunt = 0; nCunt < nFloatDataCunt; nCunt++){
		sprintf(nSendData,"%f\t",pFloatData[nCunt]);
		pData = nSendData;
		
		SendStringData(nComPortIndex,pData);
	}
}

/**
*** 函数名称 ： SendIntData
*** 函数功能 ： 发送整型数
*** 入口参数 ： 1、*pIntData指向待发送的整型数组
***             2、nIntDataCunt 待发送的整型数的个数
*** 返回值   ： 无
*/
void SendIntData(char nComPortIndex,int *pIntData,unsigned int nIntDataCunt)
{
	unsigned int nCunt = 0;
	char *pData;
	
	for(nCunt = 0; nCunt < nIntDataCunt; nCunt++){
		sprintf(nSendData,"%d\t",pIntData[nCunt]);
		pData = nSendData;
		
		SendStringData(nComPortIndex,pData);
	}
}
void SendIntDataSelect(char nComPortIndex,int *pIntData,unsigned int nStartDataCunt,unsigned int nStopDataCunt)
{
	unsigned int nCunt = 0;
	char *pData;
	
	for(nCunt = nStartDataCunt; nCunt < nStopDataCunt; nCunt++){
		sprintf(nSendData,"%d\t",pIntData[nCunt]);
		pData = nSendData;
		
		SendStringData(nComPortIndex,pData);
	}
}

/**
*** 函数名称 ： SendData
*** 函数功能 ： 以十六进制的形式发送字符串
*** 入口参数 ： 1、*pData指向待发送的数组
***             2、nIntDataCunt 待发送的数据的个数
*** 返回值   ： 无
*/

void SendData(char nComPortIndex,unsigned char *pData,unsigned int nIntDataCunt)
{
	unsigned int nCunt = 0;
	for(nCunt = 0; nCunt < nIntDataCunt; nCunt++){
		if(nComPortIndex == ComPort_1){
			SBUF = pData[nCunt];
			while(TI != 1);
			TI = 0;
		}
		
		if(nComPortIndex == ComPort_2){
			S2BUF = pData[nCunt];
			while((S2CON & 0x02) != 0x02);  // S2TI == 1
			S2CON &= 0xfd; 					// S2TI = 0
		}
	}
}

/**
*** 函数名称 ： SendTC18B20PackageTypeToBoard
*** 函数功能 ： 送芯片的封装形式给修调读温板
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、nPkgType 封装形式
*** 返回值   ： 无
*/

void SendTC18B20PackageTypeToBoard(char nTrimingBoardIndex,char nPkgType)
{
	char nPkgData[20];

	memset(nPkgData,'\0',sizeof(nPkgData));
	sprintf(nPkgData,"PKG,%d@",(int)nPkgType);
	for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
		if(nTrimingBoardIndex & nBoardIsHerePwr[nDeviceIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nDeviceIndex],BTR115200);
			SendStringData(ComPort_2,nPkgData);
		}
	}
}

/**
*** 函数名称 ： WaitForTrimingProcess
*** 函数功能 ： 等待Triming Board修调命令执行完成
*** 入口参数 ： nTrimingBoardIndex 修调板是否存在
*** 返回值   ： 无
*/

void WaitForTrimingProcess(char nTrimingBoardIndex)
{
	for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
		if(nTrimingBoardIndex & nBoardIsHerePwr[nDeviceIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nDeviceIndex],BTR115200);
			S2CON &= 0xfe;
			while(1){
				S2BUF = '%';
				while((S2CON & 0x02) != 0x02);  // S2TI == 1
				S2CON &= 0xfd; 					// S2TI = 0
				
				if(S2CON & 0x01){
					S2CON &= 0xfe;
					if('%' == S2BUF)
					break;
				}
			}
		}
	}
}

/**
*** 函数名称 ： ReadTemperatureDataFromTrimingBoard
*** 函数功能 ： 从校准板读取温度数据
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、pData 温度数据，共48个
*** 返回值   ： 无
*/

void ReadTemperatureDataFromTrimingBoard(char nTrimingBoardIndex,int *pData)
{
	int nTemperatureData;
	
	for(nRxDataCunt = 0; nRxDataCunt < 48;nRxDataCunt++){
		pData[nRxDataCunt] = 0x0000;
	}
	
	for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
		if(nTrimingBoardIndex & nBoardIsHerePwr[nDeviceIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nDeviceIndex],BTR115200);
			break;
		}
	}
	
	nRxDataCunt = 0;
	S2CON &= 0xfe;
	while(1){
		S2BUF = 0x02;
		while((S2CON & 0x02) != 0x02);  // S4TI == 1
		S2CON &= 0xfd; 
		
		if(S2CON & 0x01){ // 等待数据到
			S2CON &= 0xfe;
			
			nRecieveData[nRxDataCunt] = S2BUF; // 保存接收到的第一个数据
			nRxDataCunt++;
			break;
		}
	}
	
	while(1){
		if(S2CON & 0x01){ // 等待数据到
			S2CON &= 0xfe;
			nRecieveData[nRxDataCunt] = S2BUF; // 保存接收到的数据
			nRxDataCunt++;
			if(nRxDataCunt == 96)
			break;
		}
	}
		
	for(nRxDataCunt = 0; nRxDataCunt < 96; nRxDataCunt = nRxDataCunt + 2){
		nTemperatureData = nRecieveData[nRxDataCunt];	
		nTemperatureData = nTemperatureData << 0x08;
		nTemperatureData = nTemperatureData + (unsigned char)(nRecieveData[nRxDataCunt + 1]);
			
		pData[nRxDataCunt / 2] = nTemperatureData;
	}
}

/**
*** 函数名称 ： SendPt100DataToTrimingBoard
*** 函数功能 ： 将巡检仪的温度数据送到校准板
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、pData 温度数据
*** 返回值   ： 无
*/

void SendPt100DataToTrimingBoard(char nTrimingBoardIndex,float *pData)
{
	sprintf(nRecieveData,"PT,%.3f,%.3f,%.3f,%.3f@",pData[0],pData[1],pData[2],pData[3]);
	for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
		if(nTrimingBoardIndex & nBoardIsHerePwr[nDeviceIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nDeviceIndex],BTR115200);
		}
	}
	
	SendStringData(ComPort_2,nRecieveData);
}
void SendTrimmingVBEandRotationValues(char nTrimingBoardIndex,int *pData)
{
	char nVBEData[50];
	
	memset(nVBEData,'\0',sizeof(nVBEData));
	if(Version == Version_B){
		sprintf(nVBEData,"B,%d,%d@",pData[0],pData[1]);
	}
	else if(Version == Version_C){
		sprintf(nVBEData,"C,%d,%d@",pData[0],pData[1]);
	}
	for(nDeviceIndex = 0; nDeviceIndex < 8; nDeviceIndex++){
		if(nTrimingBoardIndex & nBoardIsHerePwr[nDeviceIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nDeviceIndex],BTR115200);
			SendStringData(ComPort_2,nVBEData);
		}
	}		
}