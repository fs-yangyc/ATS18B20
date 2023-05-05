#ifndef __DelayTime_H_
#define __DelayTime_H_

#include <stc8.h>
#include <intrins.h>

/**
*** 	说明 ：系统时钟 24 MHz
**/
void Delay_MS(int nDelayMs);
void Delay_US(unsigned char nTimes);
void Delay_5US(void);
void Delay_60US(void);
void Delay_600US(void);

#endif
