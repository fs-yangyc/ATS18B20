#ifndef __DataTrans_H_
#define __DataTrans_H_

#define BTR1200 	0x00
#define BTR2400 	0x01
#define BTR4800		0x02
#define BTR9600		0x03
#define BTR19200	0x04
#define BTR38400	0x05
#define BTR57600	0x06
#define BTR115200	0x07

#define ComPort_1	0x00
#define ComPort_2	0x01

#define COMENB	0x01 // 使能串口中断
#define COMDIS	0x00 // 关闭串口中断

#define Com1_PC		0x00   // 与电脑的串口通信
#define Com2_Other	0x01   // 与巡检仪、温箱、E3631A等通信
#define Com2_TstBrd	0x03   // 与测试板通信

#define E3631A		0x60		// 0110 0000 ,E3631A所在的位置     ,CD4051_X5,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define CFG  		0x80		// 1000 0000 ,巡检仪所在的位置     ,CD4051_X2,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define YSL_CHAMBER	0x40		// 0100 0000 ,雅士林温箱所在的位置 ,CD4051_X4,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define GWS_CHAMBER	0xc0		// 1100 0000 ,广五所温箱所在的位置 ,CD4051_X6,P6.6 = A2,P6.7 = A1,P6.5 = A0

#define RS485_1		0xa0		// 1010 0000 ,另一个未用485-1的位置,CD4051_X3,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define RS485_2		0x20		// 0010 0000 ,另一个未用485-2的位置,CD4051_X1,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define RS485_3		0x00		// 0000 0000 ,另一个未用485-3的位置,CD4051_X0,P6.6 = A2,P6.7 = A1,P6.5 = A0
#define RS232_1		0xe0		// 1110 0000 ,另一个未用串口的位置 ,CD4051_X7,P6.6 = A2,P6.7 = A1,P6.5 = A0

/*
#define TSTBRD_1		0x05		// 0000 0101 ,第一块测试板串口的位置,CD4051_X5,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_2		0x06		// 0000 0110 ,第二块测试板串口的位置,CD4051_X3,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_3		0x07		// 0000 0111 ,第三块测试板串口的位置,CD4051_X7,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_4		0x00		// 0000 0000 ,第四块测试板串口的位置,CD4051_X0,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_5		0x03		// 0000 0011 ,第五块测试板串口的位置,CD4051_X6,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_6		0x04		// 0000 0100 ,第六块测试板串口的位置,CD4051_X1,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_7		0x01		// 0000 0001 ,第七块测试板串口的位置,CD4051_X4,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_8		0x02		// 0000 0010 ,第八块测试板串口的位置,CD4051_X2,P6.0 = A2,P6.1 = A1,P6.2 = A0
*/

#define TSTBRD_1		0x01		// 0000 0001 ,第一块测试板串口的位置,CD4051_X4,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_2		0x02		// 0000 0010 ,第二块测试板串口的位置,CD4051_X2,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_3		0x03		// 0000 0011 ,第三块测试板串口的位置,CD4051_X6,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_4		0x04		// 0000 0100 ,第四块测试板串口的位置,CD4051_X1,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_5		0x07		// 0000 0111 ,第五块测试板串口的位置,CD4051_X7,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_6		0x00		// 0000 0000 ,第六块测试板串口的位置,CD4051_X0,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_7		0x05		// 0000 0101 ,第七块测试板串口的位置,CD4051_X5,P6.0 = A2,P6.1 = A1,P6.2 = A0
#define TSTBRD_8		0x06		// 0000 0110 ,第八块测试板串口的位置,CD4051_X3,P6.0 = A2,P6.1 = A1,P6.2 = A0

#define NoneDevice		0xff

#include <stc8.h>

sbit RS485RE = P2^1;       // RS485数据传输方向控制

/**
*** 函数名称 ： UsartInit
*** 函数功能 ： 初始化串口到指定的波特率
*** 入口参数 ： 1、nBtr 波特率
*** 返回值   ： 无
*/
void UsartInit(char nComPortIndex,char nBtr);

/**
*** 函数名称 ： UsartInterrupt
*** 函数功能 ： 使能或者关闭串口中断
*** 入口参数 ： 1、nEnbDisable 使能或者关闭
*** 返回值   ： 无
*/
void UsartInterrupt(char nComPortIndex,char nEnbDisable);

/**
*** 函数名称 ： SetUartPosionAndBtr
*** 函数功能 ： 改变串口的位置和波特率
*** 入口参数 ： 1、nSerialPortPosion 串口映射的引脚位置
***             2、nDeviceName 与串口通信的设备名称
***             3、nBtr 波特率
*** 返回值   ： 无
*/
void SetUsartTwoPosionAndBtr(char nSerialPortPosion,char nDeviceName,char nBtr);

/**
*** 函数名称 ： SendStringData
*** 函数功能 ： 以ASCII码的形式发送字符串
*** 入口参数 ： 1、*pData指向待发送的数组
*** 返回值   ： 无
*/
void SendStringData(char nComPortIndex,char *pData);

/**
*** 函数名称 ： SendFloatData
*** 函数功能 ： 发送浮点型数
*** 入口参数 ： 1、*pFloatData指向待发送的浮点型数组
***             2、nFloatDataCunt 待发送的浮点型数的个数
*** 返回值   ： 无
*/
void SendFloatData(char nComPortIndex,float *pFloatData,unsigned int nFloatDataCunt);

/**
*** 函数名称 ： SendIntData
*** 函数功能 ： 发送整型数
*** 入口参数 ： 1、*pIntData指向待发送的整型数组
***             2、nIntDataCunt 待发送的整型数的个数
*** 返回值   ： 无
*/
void SendIntData(char nComPortIndex,int *pIntData,unsigned int nIntDataCunt);

/**
*** 函数名称 ： SendData
*** 函数功能 ： 以十六进制的形式发送字符串
*** 入口参数 ： 1、*pData指向待发送的数组
***             2、nIntDataCunt 待发送的数据的个数
*** 返回值   ： 无
*/
void SendData(char nComPortIndex,unsigned char *pData,unsigned int nIntDataCunt);

/**
*** 函数名称 ： SendTC18B20PackageTypeToBoard
*** 函数功能 ： 送芯片的封装形式给修调读温板
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、nPkgType 封装形式
*** 返回值   ： 无
*/
void SendTC18B20PackageTypeToBoard(char nTrimingBoardIndex,char nPkgType);

/**
*** 函数名称 ： WaitForTrimingProcess
*** 函数功能 ： 等待Triming Board修调命令执行完成
*** 入口参数 ： nTrimingBoardIndex 修调板是否存在
*** 返回值   ： 无
*/
void WaitForTrimingProcess(char nTrimingBoardIndex);

/**
*** 函数名称 ： ReadTemperatureDataFromTrimingBoard
*** 函数功能 ： 从校准板读取温度数据
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、pData 温度数据，共48个
*** 返回值   ： 无
*/
void ReadTemperatureDataFromTrimingBoard(char nTrimingBoardIndex,int *pData);

/**
*** 函数名称 ： SendPt100DataToTrimingBoard
*** 函数功能 ： 将巡检仪的温度数据送到校准板
*** 入口参数 ： 1、nTrimingBoardIndex 校准板编号：1~8
***             2、pData 温度数据
*** 返回值   ： 无
*/
void SendPt100DataToTrimingBoard(char nTrimingBoardIndex,float *pData);
void SendTrimmingVBEandRotationValues(char nTrimingBoardIndex,int *pData);

void SendIntDataSelect(char nComPortIndex,int *pIntData,unsigned int nStartDataCunt,unsigned int nStopDataCunt);

#endif