#ifndef __SystemInit_H_
#define __SystemInit_H_

#define FALSE   0x00
#define TRUE	0x01

#include <STC8.H>

sbit START	   = P2^6; // 连接主板的P3.2
sbit COLLECTOR = P2^7; // 连接主板的P3.3

void SystemInit(void);
void StartProcess(void);
void TrimingBoardCollector(void);

#endif