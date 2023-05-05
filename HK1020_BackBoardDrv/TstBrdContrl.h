#ifndef __TstBrdContrl_H_
#define __TstBrdContrl_H_

#define AllTestBoardPowerOff 0xff
#define AllTestBoardPowerOn  0x00

/*
#define PwrTstBrd_1		0x08  // P7.3
#define PwrTstBrd_2		0x10  // P7.4
#define PwrTstBrd_3		0x04  // P7.2
#define PwrTstBrd_4		0x40  // P7.6
#define PwrTstBrd_5		0x02  // P7.1
#define PwrTstBrd_6		0x20  // P7.5
#define PwrTstBrd_7		0x01  // P7.0
#define PwrTstBrd_8		0x80  // P7.7
*/

#define PwrTstBrd_1		0x01  // P7.0
#define PwrTstBrd_2		0x80  // P7.7
#define PwrTstBrd_3		0x02  // P7.1
#define PwrTstBrd_4		0x20  // P7.5
#define PwrTstBrd_5		0x04  // P7.2
#define PwrTstBrd_6		0x40  // P7.6
#define PwrTstBrd_7		0x08  // P7.3
#define PwrTstBrd_8		0x10  // P7.4

#define AllPwrTstBrd	0xff
#define NoneTestBoard	0x00

#define DeviceN6701C	0x00	// 电源
#define DeviceN9020A	0x01	// 频谱仪
#define Device33500B	0x02	// 信号源
#define DeviceB2912A	0x03	// 精密电源，100mA
	
/**
*** 函数名称 ： TstBrdPowerInit
*** 函数功能 ： 关闭测试板和DUT的所有电源
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void TstBrdPowerInit(void);

void SetTstBrdPowerOn(
	char nBoardIndex  // 测试板号
);

/**
*** 函数名称 ： SearchTestMainBoard
*** 函数功能 ： 查找系统中测试主板的块数
*** 入口参数 ： 1、nTestMainBoardIndex  系统中测试主板的块数以及位置信息
*** 返回值   ： 无
*/
void SearchTestMainBoard(char *nTestMainBoardIndex);

/**
*** 函数名称 ： EndAllProcess
*** 函数功能 ： 结束全温区全电压测试
*** 入口参数 ： 1、nTestMainBoardIndex  系统中测试主板的块数以及位置信息
*** 返回值   ： 无
*/

void EndAllProcess(char nTestMainBoardIndex);	
	
	
#endif