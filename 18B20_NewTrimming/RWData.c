#include <RWData.h>
#include <DelayTime.h>

char code nICLogicPositionToBoardPosition[48] = { // 芯片在载片板上的实际位置位置，该数组的下标代表逻辑位置
	26,32,25,22,16,21,15,20,    // P0
	14,10,4,3,1,0,              // P1
	39,38,37,43,36,42,27,33,    // P2
	19,41,44,17,6,              // P4
	35,11,5,18,12,              // P5
	9,2,8,7,13,28,29,34,        // P6
	47,40,46,45,31,24,30,23     // P7
};

char code nICBoardPositionToLogicPosition[48] = { // 芯片在在测试程序中的逻辑位置，该数组的下标代表芯片在载片板上的实际位置位置
	13,12,33,11,10,29,26,35,
	34,32, 9,28,31,36,8 , 6,
	4 ,25,30,22, 7, 5, 3,47,
	45, 2, 0,20,37,38,46,44,
	1 ,21,39,27,18,16,15,14,
	41,23,19,17,24,43,42,40
};

int xdata n18B20BitPwr[16] = {0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
unsigned char idata nPortData[16][7]; // 该变量的每一个 bit 代表一颗芯片，总共 48 颗芯片
unsigned char nOnePortChipNumber[7] = {0x08,0x06,0x08,0x05,0x05,0x08,0x08}; // 单片机每一个端口上面挂载的芯片数量
unsigned char idata nP0Data,nP1Data,nP2Data,nP4Data,nP5Data,nP6Data,nP7Data;
char nPositionCunt;
int nTargetPosition[48];

void ChangeLogicPositionToPhysicalPosition(int *nLogicPosition,int *nPhysicalPosition)
{
	for(nPositionCunt = 0; nPositionCunt < 48; nPositionCunt++){
		nPhysicalPosition[nICLogicPositionToBoardPosition[nPositionCunt]] = nLogicPosition[nPositionCunt];
	}
}

void ChangePhysicalToLogicPositionPosition(int *PhysicalPosition,int *nLogicPosition)
{
	for(nPositionCunt = 0; nPositionCunt < 48; nPositionCunt++){
		nLogicPosition[nICBoardPositionToLogicPosition[nPositionCunt]] = PhysicalPosition[nPositionCunt];
	}
}

void SetDqStatus(char nHighLow)
{
	if(nHighLow == HIGH){
		P0 =  0xff;
		P1 |= 0x3f;
		P2 =  0xff;
		P4 |= 0x1f;
		P5 |= 0x3e;
		P6 =  0xff;
		P7 =  0xff;
	}
	
	else{
		P0 =  0x00; 
		P1 &= 0xc0;
		P2 =  0x00;
		P4 &= 0xe0;
		P5 &= 0xc1;
		P6 =  0x00;
		P7 =  0x00;
	}
}

void ResetTC18B20(void)
{
	OE = 0x01;  		// 关闭245输出，因上拉电阻所有的DQ = 0x01
	P3 |= 0xc0; 		// DIR0 = DIR1 = 0x01，MCU -> DQ
	Delay_600US();
	
	SetDqStatus(LOW);   // MCU Reset 18B20
	OE = 0x00;          // 所有的DQ = 0x00
	Delay_600US();
	
	OE = 0x01; 			// 关闭245输出使能，DQ因上拉而置位为“1”
	Delay_600US(); 		// 等待18B20应答
}

void StartTC18B20Convert(void)
{
	unsigned char nCommand,nBitCunt;
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = ConverTemperature;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
}

void WriteDataToTC18B20_TH_TL_Conf(unsigned char nWriteE2PromOrScratchpad,unsigned char nTh,unsigned char nTl,unsigned char nConf)
{
	unsigned char nCommand,nBitCunt;
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = WriteScratchpad;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = nTh;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = nTl;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = nConf;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	if(WRITE_E2PROM == nWriteE2PromOrScratchpad){ // 写E2PROM
		ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
		nCommand = SkipRomCommand;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
		
		Delay_600US();
		nCommand = CopyScratchpad; // 将暂存寄存器中的内容拷贝到E2PROM中
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
		
		Delay_MS(100);
	}
}

void ReadTemperatureDataFromTC18B20(int *pTemperatureData)
{
	unsigned char nCommand,nBitCunt,nPortDataCunt,nCunt,nChips;
	
	for(nCunt = 0; nCunt < 48; nCunt++){
		pTemperatureData[nCunt] = 0x0000;
	}
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = ReadScratchpad;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	for(nBitCunt = 0; nBitCunt < 16; nBitCunt++){
		P3 |= 0xc0; 		// DIR0 = DIR1 = 0x01，MCU -> DQ
		SetDqStatus(LOW); 	// SLOT 开始
		OE = 0x00;     		// DQ = 0x00
		Delay_5US();
		
		OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01，SLOT 结束
		SetDqStatus(HIGH); 	// 复位MCU端口，准备接收1bit数据
		P3 &= 0x3f; 		// DIR0 = DIR1 = 0x00，DQ -> MCU
		OE = 0x00;     
		Delay_5US();
		
		nP0Data = P0;
		nP1Data = P1;
		nP2Data = P2;
		nP4Data = P4;
		nP5Data = P5 & 0x3e;
		nP6Data = P6;
		nP7Data = P7;
		
		nPortData[nBitCunt][0] = nP0Data; 
		nPortData[nBitCunt][1] = nP1Data; 
		nPortData[nBitCunt][2] = nP2Data; 
		nPortData[nBitCunt][3] = nP4Data; 
		nPortData[nBitCunt][4] = nP5Data >> 0x01; 
		nPortData[nBitCunt][5] = nP6Data; 
		nPortData[nBitCunt][6] = nP7Data; 
		
		Delay_60US(); // 等待DQ自己复位
	}
	
	for(nBitCunt = 0; nBitCunt < 16; nBitCunt++){ // 接收芯片发出来的数据
		nPortDataCunt = 0x00;
		for(nCunt = 0; nCunt < 7; nCunt++){ // 7个端口
			for(nChips = 0; nChips < nOnePortChipNumber[nCunt]; nChips++){ // 每个端口颗芯片的颗数
				if(nPortData[nBitCunt][nCunt] & n18B20BitPwr[nChips]){
					pTemperatureData[nPortDataCunt] = pTemperatureData[nPortDataCunt] + n18B20BitPwr[nBitCunt];
				}
				nPortDataCunt++;
			}
		}
	}
}

void WriteSameDataToTC18B20_AdjRom(unsigned char nWriteE2PromOrScratchpad,unsigned char nAdj1Data,unsigned char nAdj2Data)
{
	unsigned char nCommand,nBitCunt;
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = OpenDoor;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}

	Delay_600US();
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = WriteAdj1;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}

	Delay_600US();
	nCommand = nAdj1Data;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}

	Delay_600US();
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = WriteAdj2;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}

	Delay_600US();
	nCommand = nAdj2Data;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}	
	
	if(WRITE_E2PROM == nWriteE2PromOrScratchpad){ // 写ADJ1/2 E2PROM
		Delay_600US();
		ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
		nCommand = SkipRomCommand;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
		
		Delay_600US();
		nCommand = CopyAdj1_Adj2ToE2Prom;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
	}
}

void WriteDifferenceDataToTC18B20_AdjRom(unsigned char nWriteE2PromOrScratchpad,unsigned char nAdj1OrAdj2,unsigned char *nAdjData)
{
	unsigned char nCommand,nPortDataCunt = 0x00;
	unsigned char nBitCunt,nMcuPortCunt,nCunt2;
	unsigned char nS1ToS48Data[8][7]; // 使用7个端口写数据
	unsigned char n1821BitPwr[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		for(nMcuPortCunt = 0; nMcuPortCunt < 7; nMcuPortCunt++){
			nS1ToS48Data[nBitCunt][nMcuPortCunt] = 0x00;
		}
	}
		
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){ // 8 bits 数据
		nPortDataCunt = 0x00;
		for(nMcuPortCunt = 0; nMcuPortCunt < 7; nMcuPortCunt++){ // 7个端口
			for(nCunt2 = 0; nCunt2 < nOnePortChipNumber[nMcuPortCunt]; nCunt2++){ // 每个端口颗芯片的颗数
				if(nAdjData[nPortDataCunt] & n1821BitPwr[nBitCunt]){
					nS1ToS48Data[nBitCunt][nMcuPortCunt] = nS1ToS48Data[nBitCunt][nMcuPortCunt] + n1821BitPwr[nCunt2]; // 把48颗芯片的数据的同一个位置的bit(共48bits)存放在7个8bits的寄存器中(nS1ToS48Data[0~47][0~6])
				}
				nPortDataCunt++;
			}
		}
	} // 为了写数据时序的严谨，先将所有要写的数据按照发送时序的要求处理好
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = OpenDoor;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}

	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = nAdj1OrAdj2;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		nP0Data = nS1ToS48Data[nBitCunt][0];
		nP2Data = nS1ToS48Data[nBitCunt][2];
		nP6Data = nS1ToS48Data[nBitCunt][5];
		nP7Data = nS1ToS48Data[nBitCunt][6];
		
		nS1ToS48Data[nBitCunt][4] <<= 0x01; // P51~P55
		nS1ToS48Data[nBitCunt][4] &= 0xfe;  // P50 = OE
		
		SetDqStatus(LOW);
		OE = 0x00;     // DQ = 0x00
		
		nP1Data = 0xc0 & P1 | nS1ToS48Data[nBitCunt][1]; 	// 读取 P1 的高两位
		nP4Data = 0xe0 & P4 | nS1ToS48Data[nBitCunt][3]; 	// 读取 P4 的高三位
		nP5Data = 0xc1 & P5 | nS1ToS48Data[nBitCunt][4]; 	// 读取 P5 的高两位
		
		P0 = nP0Data;	// DQ = 芯片号   1-8  databit
		P1 = nP1Data; 	// DQ = 芯片号   9-14 databit
		P2 = nP2Data; 	// DQ = 芯片号  15-22 databit
		P4 = nP4Data; 	// DQ = 芯片号 	23-27 databit
		P5 = nP5Data; 	// DQ = 芯片号 	28-32 databit
		P6 = nP6Data; 	// DQ = 芯片号  33-40 databit
		P7 = nP7Data; 	// DQ = 芯片号  41-48 databit
		
		Delay_60US();
		OE = 0x01; // 关闭245输出，因上拉电阻所有的DQ = 0x01
	}
	
	if(WRITE_E2PROM == nWriteE2PromOrScratchpad){ // 写E2PROM
		ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
		nCommand = SkipRomCommand;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
		
		Delay_600US();
		nCommand = CopyAdj1_Adj2ToE2Prom;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
	}
}

void ReadAdjRomFromTC18B20(unsigned char nRead2PromOrScratchpad,unsigned char *nAdj1Data,unsigned char *nAdj2Data)
{
	unsigned char nCommand,nBitCunt,nPortDataCunt,nCunt,nChips;
	
	for(nCunt = 0; nCunt < 48; nCunt++){
		nAdj1Data[nCunt] = 0x00;
		nAdj2Data[nCunt] = 0x00;
	}
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	if(READ_E2PROM == nRead2PromOrScratchpad){
		nCommand = SkipRomCommand;
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
		
		Delay_600US();
		nCommand = RecallE2PROM; // 将E2PROM中的内容拷贝到暂存寄存器
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			SetDqStatus(LOW);
			OE = 0x00;     			// DQ = 0x00
			Delay_5US();
		
			if(nCommand & 0x01) {	
				OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
			}
		
			Delay_60US();
			OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
			nCommand = nCommand >> 0x01;
		}
	}
	
	Delay_600US();
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = OpenDoor;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = ReadAdj1;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){ // CRC
		P3 |= 0xc0; 		// DIR0 = DIR1 = 0x01，MCU -> DQ
		SetDqStatus(LOW); 	// SLOT 开始
		OE = 0x00;     		// DQ = 0x00
		Delay_5US();
		
		OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01，SLOT 结束
		SetDqStatus(HIGH); 	// 复位MCU端口，准备接收1bit数据
		P3 &= 0x3f; 		// DIR0 = DIR1 = 0x00，DQ -> MCU
		OE = 0x00;     
		Delay_5US();
		
		nP0Data = P0;
		nP1Data = P1;
		nP2Data = P2;
		nP4Data = P4;
		nP5Data = P5 & 0x3e;
		nP6Data = P6;
		nP7Data = P7;
		
		nPortData[nBitCunt][0] = nP0Data; 
		nPortData[nBitCunt][1] = nP1Data; 
		nPortData[nBitCunt][2] = nP2Data; 
		nPortData[nBitCunt][3] = nP4Data; 
		nPortData[nBitCunt][4] = nP5Data >> 0x01; 
		nPortData[nBitCunt][5] = nP6Data; 
		nPortData[nBitCunt][6] = nP7Data; 
		
		Delay_60US(); // 等待DQ自己复位
	}
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){ // 接收芯片发出来的数据
		nPortDataCunt = 0x00;
		for(nCunt = 0; nCunt < 7; nCunt++){ // 7个端口
			for(nChips = 0; nChips < nOnePortChipNumber[nCunt]; nChips++){ // 每个端口颗芯片的颗数
				if(nPortData[nBitCunt][nCunt] & n18B20BitPwr[nChips]){
					nAdj1Data[nPortDataCunt] = nAdj1Data[nPortDataCunt] + n18B20BitPwr[nBitCunt];
				}
				nPortDataCunt++;
			}
		}
	}
	
	ResetTC18B20(); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
	nCommand = SkipRomCommand;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	Delay_600US();
	nCommand = ReadAdj2;
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		SetDqStatus(LOW);
		OE = 0x00;     			// DQ = 0x00
		Delay_5US();
		
		if(nCommand & 0x01) {	
			OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01
		}
		
		Delay_60US();
		OE = 0x01; 				// 关闭245输出，因上拉电阻所有的DQ = 0x01
		nCommand = nCommand >> 0x01;
	}
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){ // CRC
		P3 |= 0xc0; 		// DIR0 = DIR1 = 0x01，MCU -> DQ
		SetDqStatus(LOW); 	// SLOT 开始
		OE = 0x00;     		// DQ = 0x00
		Delay_5US();
		
		OE = 0x01; 			// 关闭245输出，因上拉电阻所有的DQ = 0x01，SLOT 结束
		SetDqStatus(HIGH); 	// 复位MCU端口，准备接收1bit数据
		P3 &= 0x3f; 		// DIR0 = DIR1 = 0x00，DQ -> MCU
		OE = 0x00;     
		Delay_5US();
		
		nP0Data = P0;
		nP1Data = P1;
		nP2Data = P2;
		nP4Data = P4;
		nP5Data = P5;
		nP6Data = P6;
		nP7Data = P7;
		
		nPortData[nBitCunt][0] = nP0Data;
		nPortData[nBitCunt][1] = nP1Data;
		nPortData[nBitCunt][2] = nP2Data;
		nPortData[nBitCunt][3] = nP4Data;
		nPortData[nBitCunt][4] = nP5Data >> 0x01;
		nPortData[nBitCunt][5] = nP6Data;
		nPortData[nBitCunt][6] = nP7Data;
		
		Delay_60US(); // 等待DQ自己复位
	}
	
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){ // 接收芯片发出来的数据
		nPortDataCunt = 0x00;
		for(nCunt = 0; nCunt < 7; nCunt++){ // 7个端口
			for(nChips = 0; nChips < nOnePortChipNumber[nCunt]; nChips++){ // 每个端口颗芯片的颗数
				if(nPortData[nBitCunt][nCunt] & n18B20BitPwr[nChips]){
					nAdj2Data[nPortDataCunt] = nAdj2Data[nPortDataCunt] + n18B20BitPwr[nBitCunt];
				}
				nPortDataCunt++;
			}
		}
	}
}