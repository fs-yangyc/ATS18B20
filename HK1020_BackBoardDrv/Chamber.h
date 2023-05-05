#ifndef __YSL_H_
#define __YSL_H_

#define YslChamber 	0x00
#define GwsChamber	0x01

/////////////////////////////////// 雅士林温箱 ///////////////////////////////

#define PV			0x00
#define SV			0x01

#define TCXXXChamber	    0x00
#define TransistorsChamber	0x01


#include <stc8.h>

/**
*** 函数名称 ： SetChamberOff
*** 函数功能 ： 关闭温箱到待机模式
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
*** 返回值   ： 无
*/
void SetChamberOff(char nChamberName,char nChamberIndex);

/**
*** 函数名称 ： SetChamberOn
*** 函数功能 ： 开启温箱进入工作状态
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
*** 返回值   ： 无
*/
void SetChamberOn(char nChamberName,char nChamberIndex);

/**
*** 函数名称 ： SetChamberTemperature
*** 函数功能 ： 设定温箱的温度
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
***             3、fTempData 需要设定的温度值
*** 返回值   ： 无
*/
void SetChamberTemperature(char nChamberName,char nChamberIndex,float fTempData);

/**
*** 函数名称 ： ReadChamberTemperature
*** 函数功能 ： 读取温箱箱体内当前温度数据
*** 入口参数 ： 1、nChamberName 温箱名称
***             2、nChamberIndex 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
***             3、nPvSv 只对雅士林温箱有意义
*** 返回值   ： pData 温箱箱体内当前温度数据
*/
void ReadChamberTemperature(char nChamberName,char nChamberIndex,float *pData,char nPvSv);

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
	char nChamberIndex,     // 温箱里面放的是芯片还是三极管，如果第一个参数是广五所的温箱，则第二个参数无意义，但需要填写该参数
	float fSetTempData,     // 温箱温度数据的设定值
	float fTempOffset,      // 箱内温度与设定值之差
	char *nPt100Index,      // 使用的PT100的编号
	char nPt100NumberInUse, // PT100的使用数量
	int nTempSampleCunt,    // 温箱温度数据采集个数，最大300个
	int nWaitTimeCunt       //温箱向内温度稳定后再采集的温度数据个数，又叫等待时间 = 3s * nWaitTimeCunt
);
	
#endif