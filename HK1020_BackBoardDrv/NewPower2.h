#ifndef __Power_New_H_
#define __Power_New_H_

#include <stc8.h>

#define ON  0x00			
#define OFF 0x01		


#define LOAD_DAC			0xc0 // 将数据加载到输入寄存器，DAC的输出也将更新
#define LOAD_INRegister	    0xd0 // 将数据加载到输入寄存器，但DAC的输出保持不变
#define LOAD_INRegister_DAC	0xe0 // 将数据加载到输入寄存器，但DAC的输出为前一次输入寄存器的值
#define UPDATE_DAC			0xf0 // 将输入寄存器的值更新到DAC的输出，然后后续输入的数据都将被忽略

#define READ_DAC_Register	0x80

#define PowerUp			0x00	// Power Up Device	
#define PowerDownMode0	0x04	// Power Down and output floating
#define PowerDownMode1	0x08	// Power Down and output 1K to GND	
#define PowerDownMode2	0x0c	// Power Down and output 100K to GND

sbit POWER_N     = P5^3;

sbit MAX5821_SCL = P0^1;
sbit MAX5821_SDA = P0^0;

void SetVoltage(float fVoltage);

#endif