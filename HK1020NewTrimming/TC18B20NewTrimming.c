#include <SystemInit.h>
#include <DelayTime.h>
#include <RWData.h>
#include <TranslateData.h>
#include <string.h>
#define MAXTEMPDATACUNT	10

extern bit bBoardIsHere,bTestEndFlag,bRunStopFlag,bStartFlag;
extern int nTargetPosition[48];
extern int VBE,Rotation;
extern unsigned char Version;

char code nTC18B20_CDFN8_NumberInOnePt100[9][4] = { // 每一根PT100覆盖4颗TC18B20
	{ 0, 1, 5, 6},   // 第一路PT100覆盖的4颗TC18B20
	{ 3, 4, 8, 9},	 // 第二路PT100覆盖的4颗TC18B20
	{15,16,20,21},	 // 第三路PT100覆盖的4颗TC18B20
	{18,19,23,24}, 	 // 第四路PT100覆盖的4颗TC18B20
	{ 2, 7,99,99}, 	 // 第一、二路PT100覆盖的2颗TC18B20
	{10,11,99,99}, 	 // 第一、三路PT100覆盖的2颗TC18B20
	{13,14,99,99}, 	 // 第二、四路PT100覆盖的2颗TC18B20
	{17,22,99,99}, 	 // 第三、四路PT100覆盖的2颗TC18B20
	{12,99,99,99} 	 // 第一、二、三、四路PT100覆盖的1颗TC18B20
};

void main(void)
{
//	char nTemperaturePoint,nDataCunt,nSiteIndex,nPT100Number,nTC1821IndexInOnePt100,nBitCunt,nTempMoveValue,nChipPackageType;
//	int xdata nSwingIndex[48],nMoveIndex[48],nTempData[48];
	
	char nDataCunt,nSiteIndex,nTempMoveValue,nChipPackageType;
	int xdata nMoveIndex[48],nTempData[48];
	float fOffsetValue;
	unsigned char nFuseAdj1[48],nFuseAdj2[48],nPwr[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

//	float fTemperatureWriteSlope00[2][48]; // fTemperatureWriteSlope00[0][48] -10度 Slope = 0x00时18B20测得的温度，fTemperatureWriteSlope00[1][48] 85度 Slope = 0x00时18B20测得的温度
//	float fSlopeTempData[2][48]; // fSlopeTempData[0][48] 同一个温度点Slope写0x00时18B20测得的温度，fSlopeTempData[1][48] 同一个温度点Slope写1024时18B20测得的温度
//	float fSlopeStepValue[2][48]; // -10度和85度旋转温度抬升量
//	float fMoveOneStepValue[48],fSwingOneStepValue[48];
	
	float xdata fTempData[48][2],fPt100Data[2][4];   
//	float fSwingValue,fOffsetValue;
//	unsigned char nFuseAdj1[48],nFuseAdj2[48],nTempAdj1,nAdj2TempData,nPwr[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	VBE = 0xFE; Rotation = 0x04;
	
	SystemInit();
	UartOneInit(BTR115200);
	
	ES = 0x01;
	while(1){ // 等待总控板询问本板是否存在
		if(bBoardIsHere){
			bBoardIsHere = 0;
			
			break;
		}
	}
	while(1){
		GetChipPackageType(&nChipPackageType);

		if(TRIMING == (nChipPackageType & 0x08)){
			
	/********************************************VBE  修调  开始*****************************************/	
	///*			
			EX0 = 0x01;
			GetVBEandRotationValues();
			while(1){
				if(bStartFlag){ // 启动进程开始
					bStartFlag = 0x00;
					Delay_MS(10);
					break;
				}
			}			
			if(Version == Version_B){
				WriteDataToTC18B20_TH_TL_Conf(WRITE_SCRATCHPAD,0x7f,0x80,0x7f);   // 12 bit
				Delay_MS(1);
				WriteSameDataToTC18B20_AdjRom(WRITE_SCRATCHPAD,0xfe,0x00|Rotation); // 
				Delay_MS(100);
				SendTrimingProcessEnd();

				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
					fTempData[nSiteIndex][0] = 0.0;
					fTempData[nSiteIndex][1] = 0.0;
				}
				
				for(nDataCunt = 0; nDataCunt < MAXTEMPDATACUNT; nDataCunt++){ // 旋转后平移量ADJ2为0时的测温数据
					EX1 = 0x01;
					while(1) 
					{
						if(bRunStopFlag){ // 接收来自总控MCU的温度数据采集指令
							bRunStopFlag = 0x00;
							Delay_MS(10);
							break;
						}
					}
					
					StartTC18B20Convert();
					Delay_MS(1000);
					ReadTemperatureDataFromTC18B20(nTempData);
					ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
					
					for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
						fTempData[nSiteIndex][0] += ((float)(nTargetPosition[nSiteIndex]) / 16.0) / MAXTEMPDATACUNT;
					}
					
					SendTempDataToContrlMcu(nTargetPosition,48);
				}				

				GetPt100TempData(fPt100Data[0]);

				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){ // 计算平移的熔丝代码，并填到nAdj2的高五位里面
					if(TO92 == (nChipPackageType & 0x07)){
						fOffsetValue = -(fTempData[nSiteIndex][0] - fPt100Data[0][nSiteIndex / 12] + 0.1) / 0.25;
					}
					
					nTempMoveValue = (int)fOffsetValue;
					if((fOffsetValue - (int)fOffsetValue) >= 0.5){
						nTempMoveValue += 1;
						if(nTempMoveValue > 31){
							nTempMoveValue = 31;
						}					
					}				
					nMoveIndex[nSiteIndex] = nTempMoveValue;
							
				}
				SendTempDataToContrlMcu(nMoveIndex,48); // 计算所得的平移熔丝代码送总控制板
				ChangePhysicalToLogicPositionPosition(nMoveIndex,nTargetPosition);  // 将载片板上的物理位置改成测试的逻辑位置
				
				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
					nTempMoveValue = nTargetPosition[nSiteIndex];
					if(nTempMoveValue & 0x10){
						nFuseAdj1[nSiteIndex] = 0xff;
					}
					else
					{
						nFuseAdj1[nSiteIndex] = 0xfe;
					}
					
					nTempMoveValue <<= 4;
					nFuseAdj2[nSiteIndex] = 0x00 | Rotation;
					nFuseAdj2[nSiteIndex] |= nTempMoveValue;	// & 0xf0;
				}
			}
			else if(Version == Version_C){
				WriteDataToTC18B20_TH_TL_Conf(WRITE_SCRATCHPAD,0x7f,0x80,0x7f);   // 12 bit
				Delay_MS(1);
				WriteSameDataToTC18B20_AdjRom(WRITE_SCRATCHPAD,0xFE & VBE,0x00 | Rotation); // 
				Delay_MS(100);
				SendTrimingProcessEnd();

				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
					fTempData[nSiteIndex][0] = 0.0;
					fTempData[nSiteIndex][1] = 0.0;
				}
				
				for(nDataCunt = 0; nDataCunt < MAXTEMPDATACUNT; nDataCunt++){ // 旋转后平移量ADJ2为0时的测温数据
					EX1 = 0x01;
					while(1) 
					{
						if(bRunStopFlag){ // 接收来自总控MCU的温度数据采集指令
							bRunStopFlag = 0x00;
							Delay_MS(10);
							break;
						}
					}
					
					StartTC18B20Convert();
					Delay_MS(1000);
					ReadTemperatureDataFromTC18B20(nTempData);
					ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
					
					for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
						fTempData[nSiteIndex][0] += ((float)(nTargetPosition[nSiteIndex]) / 16.0) / MAXTEMPDATACUNT;
					}
					
					SendTempDataToContrlMcu(nTargetPosition,48);
				}				

				GetPt100TempData(fPt100Data[0]);

				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){ // 计算平移的熔丝代码，并填到nAdj2的高五位里面
					if(TO92 == (nChipPackageType & 0x07)){
						fOffsetValue = -(fTempData[nSiteIndex][0] - fPt100Data[0][nSiteIndex / 12] + 0.1) / 0.125;
					}
					
					nTempMoveValue = (int)fOffsetValue;
					if((fOffsetValue - (int)fOffsetValue) >= 0.5){
						nTempMoveValue += 1;
						if(nTempMoveValue > 63){
							nTempMoveValue = 63;
						}					
					}
					
		//				if((fOffsetValue - (int)fOffsetValue) <= -0.5){
		//					nTempMoveValue -= 1;
		//					if(nTempMoveValue < -2){
		//						nTempMoveValue = -2;
		//					}					
		//				}
					
					nMoveIndex[nSiteIndex] = nTempMoveValue;
							
				}
				SendTempDataToContrlMcu(nMoveIndex,48); // 计算所得的平移熔丝代码送总控制板
				ChangePhysicalToLogicPositionPosition(nMoveIndex,nTargetPosition);  // 将载片板上的物理位置改成测试的逻辑位置
				
				for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
					nTempMoveValue = nTargetPosition[nSiteIndex];
					if(nTempMoveValue & 0x20){
						nFuseAdj1[nSiteIndex] = VBE | 0x01;
					}
					else
					{
						nFuseAdj1[nSiteIndex] = VBE & 0xfe;
					}
					
					nTempMoveValue <<= 3;
					nFuseAdj2[nSiteIndex] = 0x00 | Rotation;
					nFuseAdj2[nSiteIndex] |= nTempMoveValue;	// & 0xf0;
				}
			}
			

			
			EX0 = 0x01;
			while(1){
				if(bStartFlag){ // 启动进程开始
					bStartFlag = 0x00;
					Delay_MS(10);
					break;
				}
			}
			
			EX1 = 0x01;
			while(1) 
			{
				if(bRunStopFlag){ // 接收来自总控MCU的温度数据采集指令
					bRunStopFlag = 0x00;
					Delay_MS(10);
					break;
				}
			}
			
			WriteDifferenceDataToTC18B20_AdjRom(WRITE_SCRATCHPAD,WriteAdj1,nFuseAdj1);
			Delay_MS(1);
			WriteDifferenceDataToTC18B20_AdjRom(WRITE_E2PROM,WriteAdj2,nFuseAdj2);     // 旋转量和平移量拷贝到E2Prom中
			
			Delay_MS(1000);
			SendTrimingProcessEnd();
			Delay_MS(3000);
			ReadAdjRomFromTC18B20(READ_E2PROM,nFuseAdj1,nFuseAdj2); // 读取写入E2PROM中的校准值
	
				/***********Send VBE*************/
			memset(nTempData,0,sizeof(nTempData));
			for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
				if(Version == Version_B){		
					nTempData[nSiteIndex] = nFuseAdj1[nSiteIndex];
				}
				else if(Version == Version_C){
					nTempData[nSiteIndex] = nFuseAdj1[nSiteIndex] & 0xFE;
				}
			}
			ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
			SendTempDataToContrlMcu(nTargetPosition,48);
			/***********Send offset*************/
			memset(nTempData,0,sizeof(nTempData));
			for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
				if(Version == Version_B){		
					nTempData[nSiteIndex] = nFuseAdj2[nSiteIndex] & 0xf0;
				}
				else if(Version == Version_C){
					nTempData[nSiteIndex] = ((nFuseAdj2[nSiteIndex] & 0xf8) >> 3) | ((nFuseAdj1[nSiteIndex] & 0x01) << 5);
				}				
			}
			ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
			SendTempDataToContrlMcu(nTargetPosition,48);
			/***********Send Rotation*************/
			memset(nTempData,0,sizeof(nTempData));
			for(nSiteIndex = 0; nSiteIndex < 48; nSiteIndex++){
				if(Version == Version_B){						
					nTempData[nSiteIndex] = nFuseAdj2[nSiteIndex] & 0x0f;
				}
				else if(Version == Version_C){
					nTempData[nSiteIndex] = nFuseAdj2[nSiteIndex] & 0x07;
				}				
			}
			ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
			SendTempDataToContrlMcu(nTargetPosition,48);
			
			
			Delay_MS(1000);
			
			for(nDataCunt = 0; nDataCunt < MAXTEMPDATACUNT; nDataCunt++){ // 测温数据
				EX1 = 0x01;
				while(1) 
				{
					if(bRunStopFlag){ // 接收来自总控MCU的温度数据采集指令
						bRunStopFlag = 0x00;
						Delay_MS(10);
						break;
					}
				}
				
				StartTC18B20Convert();
				Delay_MS(1000);
				ReadTemperatureDataFromTC18B20(nTempData);
				ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
				
				SendTempDataToContrlMcu(nTargetPosition,48);
			}	
		}			
		
	
  /********************************************VBE  修调  结束*****************************************/			

		bTestEndFlag = 0x00;
		bStartFlag = 0x00;
		EX0 = 0x01;
		while(1){ 					// 全温区全电压测温
			if(bTestEndFlag)
				break;
			
			if(bStartFlag){
				ES = 0x00;			// 注意，在这里要关闭串口中断，否则下面发送数据会失败
				bStartFlag = 0x00;
				Delay_MS(10);
				
				WriteDataToTC18B20_TH_TL_Conf(WRITE_SCRATCHPAD,0x7f,0x80,0x7f);   // 12 bit
				Delay_MS(1);
				for(nDataCunt = 0; nDataCunt < MAXTEMPDATACUNT; nDataCunt++){
					EX1 = 0x01;
					while(1) 
					{
						if(bRunStopFlag){ // 接收来自总控MCU的温度数据采集指令
							bRunStopFlag = 0x00;
							Delay_MS(10);
							
							StartTC18B20Convert();
							Delay_MS(1000);
							ReadTemperatureDataFromTC18B20(nTempData);
							ChangeLogicPositionToPhysicalPosition(nTempData,nTargetPosition); // 将芯片的逻辑测试位置更改成载片板上的实际位置
							SendTempDataToContrlMcu(nTargetPosition,48);
							
							break;
						}
					} 
				} // for end
				
				EX0 = 0x01;
				RI = 0x00;
				TI = 0x00;
				ES = 0x01;
			}
		} // 全温区全电压测温结束
	}
}