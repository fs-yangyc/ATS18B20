#ifndef __PC_H_
#define __PC_H_

#define TO92	0x07
#define SOP8	0x06
#define CDFN8	0x05
#define NONEPKG	0xf0

#define TRIMING	0x08
#define READING 0x00

#define Version_B 0x01
#define Version_C 0x02


#define UserV3V3	0x70
#define UserV5V		0x60
#define NoneVoltage	0x0f

void SendStringDataToPC(char *pData);
void SendFloatDataToPC(float *pFloatData,unsigned char nFloatDataCunt);
void SendIntDataToPC(int *pIntData,int nIntDataCunt);
void SendDataToPC(unsigned char *pIntData,int nIntDataCunt);
void ReadPT100IndexFromPC(char *nPT100Index);
void ReadTC18B20PackageTypeFromPC(char *nPackageType,char *nPackageName);
void ReadTC18B20PackageAndUsePt100Index(char *nPackageType,char *nPt100Index);
void ReadTrimmingDataFromPC(void);
#endif