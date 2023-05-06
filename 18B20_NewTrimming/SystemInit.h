#ifndef __SystemInit_H_
#define __SystemInit_H_

#include <stc8.h>

sbit ScanStart = P3^2;
sbit DataCollect = P3^3;

void SystemInit(void);

#endif