#ifndef __CFGuo_H_
#define __CFGuo_H_

#include <stc8.h>

/**
*** 函数名称 ： RequireCFGuoAllChannelTemperature
*** 函数功能 ： 读取巡检仪所有通道的温度数据
*** 入口参数 ： 无
*** 返回值   ： 接收数据正确返回 1，错误返回 0
*/
char RequireCFGuoAllChannelTemperature(float *fTempData);

#endif