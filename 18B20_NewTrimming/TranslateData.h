#ifndef __TranslateData_H_
#define __TranslateData_H_

#define BTR1200 	0x00
#define BTR2400 	0x01
#define BTR4800		0x02
#define BTR9600		0x03
#define BTR19200	0x04
#define BTR38400	0x05
#define BTR57600	0x06
#define BTR115200	0x07

#define TO92	0x07
#define SOP8	0x06
#define CDFN8	0x05

#define TRIMING	0x08
#define READING 0x00

#define UserV3V3	0x70
#define UserV5V		0x60

#define Version_B 0x01
#define Version_C 0x02


#include <stc8.h>

void UartOneInit(char nBtr);
/**
*** 函数名称 ： SendTempDataToContrlMcu
*** 函数功能 ： 以十六进制的形式发送字符串到总控制MCU
*** 入口参数 ： 1、*pIntData指向待发送的整型数组
***             2、nIntDataCunt发送数据的个数
*** 返回值   ： 无
*/
void SendTempDataToContrlMcu(int *pIntData,unsigned char nIntDataCunt);

/**
*** 函数名称 ： GetPt100TempData
*** 函数功能 ： 解析来自pt100的测温数据,共8个数据
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void GetPt100TempData(float *pPt100Data);
void GetChipPackageType(char *nPackageType);

/**
*** 函数名称 ： SendTrimingProcessEnd
*** 函数功能 ： 向总控制板发送修调过程完成
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void SendTrimingProcessEnd(void);

void GetVBEandRotationValues(void);

#endif