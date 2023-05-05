#include <Chamber.h>
#include <DelayTime.h>
#include <DataTrans.h>
#include <cfguo.h>
#include <Crc16_MODBUS.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

unsigned char xdata ChamberData[100]; // 雅士林温箱的数据
unsigned int nCrcData;
float fXJYData[32],fChamberData[300]; // 来自巡检仪PT100的数据

/**
*** 函数名称 ： SetChamberOff
*** 函数功能 ： 关闭温箱到待机模式
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
*** 返回值   ： 无
*/
void SetChamberOff(char nChamberName,char nChamberIndex)
{
	if(GwsChamber == nChamberName){
		RS485RE = 0x01;
		SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR19200);
		Delay_MS(1);
		SendStringData(ComPort_2,"1,POWER,OFF\n"); 
		
		return;
	}
	
	if(YslChamber == nChamberName){
		ChamberData[0] = 0x01;  // 设备地址
		ChamberData[1] = 0x10;  // 功能码“写”
		ChamberData[2] = 0x00;  // 起始地址高8位
		ChamberData[3] = 0x2f;  // 起始地址低8位
		ChamberData[4] = 0x00;  // 寄存器数量高8位
		ChamberData[5] = 0x01;  // 寄存器数量低8位
		ChamberData[6] = 0x02;  // 要写的内容的字节数
		ChamberData[7] = 0x00;  // 数据1高八位
		ChamberData[8] = 0x00;  // 数据1低八位
	
		nCrcData = CRC16_MODBUS(ChamberData, 9); // 计算前面9各字节的CRC码
		ChamberData[9] = nCrcData; // CRC低8位
		ChamberData[10] = nCrcData >> 0x08; // CRC高8位
		
		if(TCXXXChamber == nChamberIndex){ //测温芯片所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,YSL_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); 
		}
		
		if(TransistorsChamber == nChamberIndex){ //测温三极管所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); 
		}
		
		return;
	}
}

/**
*** 函数名称 ： SetChamberOn
*** 函数功能 ： 开启温箱进入工作状态
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
*** 返回值   ： 无
*/
void SetChamberOn(char nChamberName,char nChamberIndex)
{
	if(GwsChamber == nChamberName){
		RS485RE = 0x01;
		SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR19200);
		Delay_MS(1);
		SendStringData(ComPort_2,"1,POWER,ON\n"); 
		
		return;
	}
	
	if(YslChamber == nChamberName){
		ChamberData[0] = 0x01;  // 设备地址
		ChamberData[1] = 0x10;  // 功能码“写”
		ChamberData[2] = 0x00;  // 起始地址高8位
		ChamberData[3] = 0x2f;  // 起始地址低8位
		ChamberData[4] = 0x00;  // 寄存器数量高8位
		ChamberData[5] = 0x01;  // 寄存器数量低8位
		ChamberData[6] = 0x02;  // 要写的内容的字节数
		ChamberData[7] = 0x00;  // 数据1高八位
		ChamberData[8] = 0x01;  // 数据1低八位
	
		nCrcData = CRC16_MODBUS(ChamberData, 9); // 计算前面9各字节的CRC码
		ChamberData[9] = nCrcData; // CRC低8位
		ChamberData[10] = nCrcData >> 0x08; // CRC高8位
		
		if(TCXXXChamber == nChamberIndex){ //测温芯片所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,YSL_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); 
		}
		
		if(TransistorsChamber == nChamberIndex){ //测温三极管所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); 
		}
		
		return;
	}
}

/**
*** 函数名称 ： SetChamberTemperature
*** 函数功能 ： 设定温箱的温度
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
***             3、fTempData 需要设定的温度值
*** 返回值   ： 无
*/
void SetChamberTemperature(char nChamberName,char nChamberIndex,float fTempData)
{
	int nTempData;
	
	if(GwsChamber == nChamberName){
		RS485RE = 0x01;
		memset(ChamberData,'\0',sizeof(ChamberData));
		sprintf(ChamberData,"1,TEMP,S%.1f\n",fTempData);
		SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR19200);
		Delay_MS(1);
		SendStringData(ComPort_2,ChamberData); // 设置温箱的温度
		
		return;
	}
	
	if(YslChamber == nChamberName){
		if(fTempData >= 0){
			nTempData = (int)(10 * fTempData);
		}
		else{
			nTempData = 65536 + (int)(10 * fTempData);
		}
		
		ChamberData[0] = 0x01;  // 设备地址
		ChamberData[1] = 0x10;  // 功能码“写”
		ChamberData[2] = 0x00;  // 起始地址高8位
		ChamberData[3] = 0x2b;  // 起始地址低8位
		ChamberData[4] = 0x00;  // 寄存器数量高8位
		ChamberData[5] = 0x01;  // 寄存器数量低8位
		ChamberData[6] = 0x02;  // 要写的内容的字节数
		ChamberData[7] = nTempData >> 0x08; // 数据1高八位
		ChamberData[8] = nTempData; // 数据1低八位
	
		nCrcData = CRC16_MODBUS(ChamberData, 9); // 计算前面9各字节的CRC码
		ChamberData[9] = nCrcData; // CRC低8位
		ChamberData[10] = nCrcData >> 0x08; // CRC高8位
		
		if(TCXXXChamber == nChamberIndex){ //测温芯片所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,YSL_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); // 设置温箱的温度 ，SendData
		}
		
		if(TransistorsChamber == nChamberIndex){ //测温三极管所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,11); // 设置温箱的温度 ，SendData
		}
		
		return;
	}
}

/**
*** 函数名称 ： ReadChamberTemperature
*** 函数功能 ： 读取温箱箱体内当前温度数据
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
***             3、nPvSv 只对雅士林温箱有意义
*** 返回值   ： pData 温箱箱体内当前温度数据
*/
void ReadChamberTemperature(char nChamberName,char nChamberIndex,float *pData,char nPvSv)
{
	unsigned char nRxDataCunt,nTempDataCunt,nTempData[20];
	int nData;
	
	memset(nTempData,'\0',sizeof(nTempData));
	memset(ChamberData,'\0',sizeof(ChamberData));
	if(GwsChamber == nChamberName){
		RS485RE = 0x01;
		SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR19200);
		Delay_MS(1);
		SendStringData(ComPort_2,"1,TEMP?\n");  
		
		nRxDataCunt = 0;
		RS485RE = 0x00;
		S2CON &= 0xfe;
		
		while(1){
			if(S2CON & 0x01){ // 等待数据到
				S2CON &= 0xfe;
				
				if(S2BUF == '\n')
				break;
				
				ChamberData[nRxDataCunt] = S2BUF; // 保存接收到的数据
				nRxDataCunt++;
			}
		}
	
		for(nRxDataCunt = 0; nRxDataCunt < 50; nRxDataCunt++){ // 当前温度
			if(ChamberData[nRxDataCunt] == ','){
				pData[0] = atof(nTempData);
				memset(nTempData,'\0',sizeof(nTempData));
				break;
			}
			
			nTempData[nRxDataCunt] = ChamberData[nRxDataCunt];
		}

		RS485RE = 0x01;
		return;
	}
	
	if(YslChamber == nChamberName){
		ChamberData[0] = 0x01;  // 设备地址
		ChamberData[1] = 0x03;  // 功能码"读"
		ChamberData[2] = 0x00;  // 起始地址高8位
		
		if(nPvSv == PV){
			ChamberData[3] = 0x00;  // 起始地址低8位,读当前温度PV/0000
		}
		
		if(nPvSv == SV){
			ChamberData[3] = 0x2b;  // 起始地址低8位,读当前温度SV/0043/0051
		}
		
		ChamberData[4] = 0x00;  // 寄存器数量高8位
		ChamberData[5] = 0x01;  // 寄存器数量低8位
	
		nCrcData = CRC16_MODBUS(ChamberData, 6); // 计算前面6字节的CRC码
		ChamberData[6] = nCrcData; // CRC低8位
		ChamberData[7] = nCrcData >> 0x08; // CRC高8位
		
		if(TCXXXChamber == nChamberIndex){ //测温芯片所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,YSL_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,8); 
			RS485RE = 0x00; // 准备读取数据
		}
		
		if(TransistorsChamber == nChamberIndex){ //测温三极管所在的雅士林温箱
			RS485RE = 0x01;
			SetUsartTwoPosionAndBtr(Com2_Other,GWS_CHAMBER,BTR9600);
			Delay_MS(1);
			SendData(ComPort_2,ChamberData,8); 
			RS485RE = 0x00; // 准备读取数据
		}
	
		S2CON &= 0xfe;
		while(1){
			if(S2CON & 0x01){ // 等待数据到
				S2CON &= 0xfe;
				nTempData[nRxDataCunt] = S2BUF;
				nRxDataCunt++;
			
				if(nRxDataCunt == 3){
					nTempDataCunt = nTempData[2] + 5; // 温箱回传的数据个数
				}
				
				if(nRxDataCunt == nTempDataCunt) // 包括CRC码在内
				break;
			}
		}
		
		RS485RE = 0x01;
		nData = (char)nTempData[3];
		nData = nData << 0x08;
		nData = nData + nTempData[4];
		
		pData[0] = (float)nData / 10.0;
		return;
	}
}

/**
*** 函数名称 ： WaitChamberStable
*** 函数功能 ： 等待温箱稳定
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
***             3、fSetTempData 温箱温度数据的设定值
***             4、fTempOffset 箱内温度与设定值之差
***             5、nPt100Index 使用的PT100的编号
***             6、nPt100NumberInUse PT100的使用数量
***             7、nTempSampleCunt 温箱温度数据采集个数，最大300个
***             8、nWaitTimeCunt 温箱向内温度稳定后再采集的温度数据个数，又叫等待时间 = 3s * nWaitTimeCunt
*** 返回值   ： 成功返回 1，否则返回 0
*/
char WaitChamberStable(
	char nChamberName,      // 温箱名称
	char nChamberIndex,     // 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数?
	float fSetTempData,     // 温箱温度数据的设定值
	float fTempOffset,      // 箱内温度与设定值之差
	char *nPt100Index,      // 使用的PT100的编号
	char nPt100NumberInUse, // PT100的使用数量
	int nTempSampleCunt,    // 温箱温度数据采集个数，最大300个
	int nWaitTimeCunt       //温箱向内温度稳定后再采集的温度数据个数，又叫等待时间 = 3s * nWaitTimeCunt
)
{
	int nPt100DataCunt,nPt100Number;
	float fMin,fMax,fChamberTrueData;        // 保存温湿度巡检仪的数据;
	
	if(nTempSampleCunt > 300){
		nTempSampleCunt = 300;
	}
	
	for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt; nPt100DataCunt++){ // 采集300个温度数据
		fChamberTrueData = 0.0;
		if(RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
			for(nPt100Number = 0; nPt100Number < nPt100NumberInUse; nPt100Number++){
				fChamberTrueData += fXJYData[nPt100Index[nPt100Number]];
			}
			
			fChamberData[nPt100DataCunt] = fChamberTrueData / nPt100NumberInUse;
		}

		else{ 	 // 读取巡检仪的温度数据错误
			return 0x00;
		}
	}
	
	while(1){ // 等待温箱温度稳定
		fMin = fChamberData[0];
		fMax = fChamberData[0];
		for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt; nPt100DataCunt++){ // 寻找300个温箱温度数据中的最大值和最小值
			if(fMin > fChamberData[nPt100DataCunt]){
				fMin = fChamberData[nPt100DataCunt];
			}
			
			if(fMax < fChamberData[nPt100DataCunt]){
				fMax = fChamberData[nPt100DataCunt];
			}
		}
		
		if((fMax - fMin) > 0.2){ // 温箱内的温度环境还没有稳定在设定值
			for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt - 1; nPt100DataCunt++){
				fChamberData[nPt100DataCunt] = fChamberData[nPt100DataCunt + 1];
			}
			
			fChamberTrueData = 0.0;
			if(RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
				for(nPt100Number = 0; nPt100Number < nPt100NumberInUse; nPt100Number++){
					fChamberTrueData += fXJYData[nPt100Index[nPt100Number]];
				}
			
				fChamberData[nTempSampleCunt - 1] = fChamberTrueData / nPt100NumberInUse;
			}
			
			else{ 	 // 读取巡检仪的温度数据错误
				return 0x00;
			}
		}
		
		else  // 箱内温度已经稳定
			break;
	}
		
	for(nPt100DataCunt = 0; nPt100DataCunt < nWaitTimeCunt; nPt100DataCunt++){ // 箱内温度已经稳定，再采集nWaitTimeCunt个温度数据
		fChamberTrueData = 0.0;
		if(RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
			for(nPt100Number = 0; nPt100Number < nPt100NumberInUse; nPt100Number++){
				fChamberTrueData += fXJYData[nPt100Index[nPt100Number]];
			}
		}
		
		else{ 	 // 读取巡检仪的温度数据错误
			return 0x00;
		}
	}
		
	fChamberTrueData = fChamberTrueData / nPt100NumberInUse;
	if(fabs(fChamberTrueData - fSetTempData) > fTempOffset){ // 箱内温度与设定值之差大于fTempOffset度，需要修调
		fChamberTrueData = 2.0 * fSetTempData - fChamberTrueData;
		SetChamberTemperature(nChamberName,nChamberIndex,fChamberTrueData); // 调整温箱的温度
		
		for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt; nPt100DataCunt++){ // 采集300个温度数据
			fChamberTrueData = 0.0;
			if(RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
				for(nPt100Number = 0; nPt100Number < nPt100NumberInUse; nPt100Number++){
					fChamberTrueData += fXJYData[nPt100Index[nPt100Number]];
				}
			
				fChamberData[nPt100DataCunt] = fChamberTrueData / nPt100NumberInUse;
			}

			else{ 	 // 读取巡检仪的温度数据错误
				return 0x00;
			}
		}
			
		while(1){
			fMin = fChamberData[0];
			fMax = fChamberData[0];
			for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt; nPt100DataCunt++){ // 寻找300个温箱温度数据中的最大值和最小值
				if(fMin > fChamberData[nPt100DataCunt]){
					fMin = fChamberData[nPt100DataCunt];
				}
				
				if(fMax < fChamberData[nPt100DataCunt]){
					fMax = fChamberData[nPt100DataCunt];
				}
			}
			
			if((fMax - fMin) > 0.2){ // 温箱内的温度环境还没有稳定在设定值
				for(nPt100DataCunt = 0; nPt100DataCunt < nTempSampleCunt - 1; nPt100DataCunt++){
					fChamberData[nPt100DataCunt] = fChamberData[nPt100DataCunt + 1];
				}
				
				fChamberTrueData = 0.0;
				if(RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
					for(nPt100Number = 0; nPt100Number < nPt100NumberInUse; nPt100Number++){
						fChamberTrueData += fXJYData[nPt100Index[nPt100Number]];
					}
			
					fChamberData[nTempSampleCunt - 1] = fChamberTrueData / nPt100NumberInUse;
				}
			
				else{ 	 // 读取巡检仪的温度数据错误
					return 0x00;
				}
			}
			
			else
				break;
		}
		
		for(nPt100DataCunt = 0; nPt100DataCunt < nWaitTimeCunt; nPt100DataCunt++){ // 采集nWaitTimeCunt个温度数据
			if(!RequireCFGuoAllChannelTemperature(fXJYData)){ 	 // 正确读取巡检仪的温度,共32个通道
				return 0x00;
			}
		}
	}
	
	return 0x01;
}