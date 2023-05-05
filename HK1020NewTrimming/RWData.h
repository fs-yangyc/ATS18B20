#ifndef __RWData_H_
#define __RWData_H_

#define HIGH 0x01
#define LOW  0x00

#define WRITE_SCRATCHPAD	0x00
#define WRITE_E2PROM		0x01

#define READ_SCRATCHPAD		0x02
#define READ_E2PROM			0x03

#define ReadRomCommand		0x33
#define MatchRomCommand		0x55
#define SearchRomCommand	0xf0
#define AlertSearchCommand	0xec
#define SkipRomCommand		0xcc

#define WriteScratchpad		0x4e
#define ReadScratchpad		0xbe
#define CopyScratchpad		0x48 // 将暂存寄存器中的内容拷贝到E2PROM中
#define ConverTemperature	0x44
#define RecallE2PROM		0xb8 // 将E2PROM中的内容拷贝到暂存寄存器
#define ReadPowerSupply		0xb4

#define OpenDoor			0x68
#define WriteAdj1			0x95
#define WriteAdj2			0x63
#define CopyAdj1_Adj2ToE2Prom	0x94

#define ReadAdj1			0x93
#define ReadAdj2			0x68 // 这个命令和开门命令一样

#include <stc8.h>

sbit DIR0 = P3^6;
sbit DIR1 = P3^7;
sbit OE   = P5^0;

/************************************************************************************************************************************************************/
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/
/*** | STC8端口 | IC编号  | STC8端口 | IC编号  | STC8端口 | IC编号  | STC8端口 | IC编号  | STC8端口 | IC编号  | STC8端口 | IC编号  | STC8端口 | IC编号  | ***/                        
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/
/*** |  P00     | Site 27 |  P10     | Site 15 |  P20     | Site 40 |  P40     | Site 20 |          |         |  P60     | Site 10 |  P70     | Site 48 | ***/
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/   
/*** |  P01     | Site 33 |  P11     | Site 11 |  P21     | Site 39 |  P41     | Site 42 |  P51     | Site 36 |  P61     | Site 03 |  P71     | Site 41 | ***/    
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/    
/*** |  P02     | Site 26 |  P12     | Site 05 |  P22     | Site 38 |  P42     | Site 45 |  P52     | Site 12 |  P62     | Site 09 |  P72     | Site 47 | ***/    
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/    
/*** |  P03     | Site 23 |  P13     | Site 04 |  P23     | Site 44 |  P43     | Site 18 |  P53     | Site 06 |  P63     | Site 08 |  P73     | Site 46 | ***/    
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/    
/*** |  P04     | Site 17 |  P14     | Site 02 |  P24     | Site 37 |  P44     | Site 07 |  P54     | Site 19 |  P64     | Site 14 |  P74     | Site 32 | ***/    
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/   
/*** |  P05     | Site 22 |  P15     | Site 01 |  P25     | Site 43 |          |         |  P55     | Site 13 |  P65     | Site 29 |  P75     | Site 25 | ***/ 
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/ 
/*** |  P06     | Site 16 |          |         |  P26     | Site 28 |          |         |          |         |  P66     | Site 30 |  P76     | Site 31 | ***/ 
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/ 
/*** |  P07     | Site 21 |          |         |  P27     | Site 34 |          |         |          |         |  P67     | Site 35 |  P77     | Site 24 | ***/ 
/*** +----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+----------+---------+ ***/ 
/************************************************************************************************************************************************************/

void SetDqStatus(char nHighLow);
void ResetTC18B20(void); // 该函数执行完毕后，245的输出被关闭，处于高阻状态，DIR0 = DIR1 = 0x01，MCU -> DQ，MCU连接DQ端口均被清零，但因上拉使得DQ = 1
void StartTC18B20Convert(void);
void WriteDataToTC18B20_TH_TL_Conf(
	unsigned char nWriteE2PromOrScratchpad,
	unsigned char nTh,
	unsigned char nTl,
	unsigned char nConf
);
	
void ReadTemperatureDataFromTC18B20(int *pTemperatureData);
void WriteSameDataToTC18B20_AdjRom(
	unsigned char nWriteE2PromOrScratchpad,
	unsigned char nAdj1Data,
	unsigned char nAdj2Data
);
	
void WriteDifferenceDataToTC18B20_AdjRom(
	unsigned char nWriteE2PromOrScratchpad,
	unsigned char nAdj1OrAdj2,
	unsigned char *nAdjData
);
	
void ReadAdjRomFromTC18B20(
	unsigned char nRead2PromOrScratchpad,
	unsigned char *nAdj1Data,
	unsigned char *nAdj2Data
);

void ChangeLogicPositionToPhysicalPosition(int *nLogicPosition,int *PhysicalPosition);
void ChangePhysicalToLogicPositionPosition(int *PhysicalPosition,int *nLogicPosition);
#endif