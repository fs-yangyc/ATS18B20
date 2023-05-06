#ifndef __Power_New_H_
#define __Power_New_H_

#define MAX5812_LOAD_DAC			0xc0 // 将数据加载到输入寄存器，DAC的输出也将更新
#define MAX5812_LOAD_INRegister	    0xd0 // 将数据加载到输入寄存器，但DAC的输出保持不变
#define MAX5812_LOAD_INRegister_DAC	0xe0 // 将数据加载到输入寄存器，但DAC的输出为前一次输入寄存器的值
#define MAX5812_UPDATE_DAC			0xf0 // 将输入寄存器的值更新到DAC的输出，然后后续输入的数据都将被忽略

#define MAX5812_READ_DAC_Register	0x80

#define MAX5812_PowerUp				0x00	// Power Up Device	
#define MAX5812_PowerDownMode0		0x04	// Power Down and output floating
#define MAX5812_PowerDownMode1		0x08	// Power Down and output 1K to GND	
#define MAX5812_PowerDownMode2		0x0c	// Power Down and output 100K to GND

#define MAX5812_ADDR				0x20
#define ADS1100_ADDR				0x90

#define ADS1100_PGA_GAIN_1			0x00
#define ADS1100_PGA_GAIN_2			0x01
#define ADS1100_PGA_GAIN_4			0x02
#define ADS1100_PGA_GAIN_8			0x03

#define ADS1100_DR_SPS_8			0x0c
#define ADS1100_DR_SPS_16			0x08
#define ADS1100_DR_SPS_32			0x04
#define ADS1100_DR_SPS_128			0x00

#define ADS1100_SC_SINGLE			0x10
#define ADS1100_SC_CONTINUOUS		0x00

#define ADS1100_SC_SINGLE_START		0x80

#include <stc8.h>

sbit NewPower_SCL = P0^1;
sbit NewPower_SDA = P0^0;

void SetVoltage(float fVoltage);
void ReadOPA544OutputCurrent(unsigned char nAds1100Gain,unsigned char nSampleCunt,float *fIccurent);

#endif