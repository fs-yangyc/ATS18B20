#ifndef __Crc16_MODBUS_H_
#define __Crc16_MODBUS_H_

/**
*** 函数名称 ： RequireCFGuoAllChannelTemperature
*** 函数功能 ： 读取巡检仪所有通道的温度数据
*** 入口参数 ： 无
*** 返回值   ： 接收数据正确返回 1，错误返回 0
*/
void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf);  
void InvertUint16(unsigned int *dBuf,unsigned int *srcBuf);  
unsigned int CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen); 

#endif