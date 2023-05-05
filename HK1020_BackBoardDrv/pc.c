#include <pc.h>
#include <DataTrans.h>
#include <string.h>
#include <stdlib.h>

unsigned char nCunt;
extern char xdata nRecieveData[200];    // 来源于 “DataTrans.c”
extern int VBEandRotation[2];
unsigned char Version = 0x01;

void SendStringDataToPC(char *pData)
{
//	ES = 0x00;
	UsartInit(ComPort_1,BTR115200);
	SendStringData(ComPort_1,pData);
}

void SendFloatDataToPC(float *pFloatData,unsigned char nFloatDataCunt)
{
//	ES = 0x00;
	UsartInit(ComPort_1,BTR115200);
	SendFloatData(ComPort_1,pFloatData,nFloatDataCunt); 
}

void SendIntDataToPC(int *pIntData,int nIntDataCunt)
{
//	ES = 0x00;
	UsartInit(ComPort_1,BTR115200);
	SendIntData(ComPort_1,pIntData,nIntDataCunt);
}

void ReadTC18B20PackageAndUsePt100Index(char *nPackageType,char *nPt100Index)
{
	char nTempRxData[10];
	char nPt100Cunt = 0,nBitCunt = 0;
	
	UsartInit(ComPort_1,BTR115200);
	
	nCunt = 0;
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			nRecieveData[nCunt] = SBUF;
			if('@' == nRecieveData[nCunt]){
				break;
			}
			
			nCunt++;
		}
	}
	
	*nPackageType = 0x00;
	for(nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++){
		nPt100Index[nPt100Cunt] = 99;
	}
	
	if((strstr(nRecieveData,"TO92") != NULL) || (strstr(nRecieveData,"to92") != NULL)){
		*nPackageType |= TO92;
	}
	
	else if((strstr(nRecieveData,"SOP8") != NULL) || (strstr(nRecieveData,"sop8") != NULL)){
		*nPackageType |= SOP8;
	}
	
	else if((strstr(nRecieveData,"CDFN8") != NULL) || (strstr(nRecieveData,"cdfn8") != NULL)){
		*nPackageType |= CDFN8;
	}
	
	else{
		*nPackageType |= NONEPKG;
	}
		
	if(strstr(nRecieveData,"3.3V") != NULL){
		*nPackageType |= UserV3V3;
	}
	
	else if((strstr(nRecieveData,"5.0V") != NULL) || (strstr(nRecieveData,"5V") != NULL)){
		*nPackageType |= UserV5V;
	}
	
	else{
		*nPackageType |= NoneVoltage;
	}
	
// SOP8-5.0V,PT100,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32@	
	memset(nTempRxData,'\0',sizeof(nTempRxData));
	nCunt = 0;
	while(1){
		if(',' == nRecieveData[nCunt]){ // 第一个逗号
			nCunt++;
			break;
		}
		nCunt++;
	}
	
	while(1){
		if(',' == nRecieveData[nCunt]){ // 第二个逗号
			nCunt++;
			break;
		}
		nCunt++;
	}
	
	for(nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++){ // 解析铂电阻的编号
		memset(nTempRxData,'\0',sizeof(nTempRxData));
		
		nBitCunt = 0;
		while(1){
			if((',' == nRecieveData[nCunt]) || ('@' ==  nRecieveData[nCunt]))
			break;
			
			else{			
				nTempRxData[nBitCunt] = nRecieveData[nCunt];
				nBitCunt++;
				nCunt++;
			}
		}
		
		nPt100Index[nPt100Cunt] = (char)(atoi(nTempRxData)) - 0x01; // PT100的编号从“1”开始
		if('@' ==  nRecieveData[nCunt]){
			break;
		}
		
		else{
			nCunt++;
		}
	}
}
	//#VBE,Rotation@
void ReadTrimmingDataFromPC(void)
{
	char nTempRxData[10];
	char nCuntA = 0,nBitCunt = 0;
	
	UsartInit(ComPort_1,BTR115200);
	
	nCunt = 0;
	RI = 0x00;
	while(1){
		if(RI){
			RI = 0x00;
			
			nRecieveData[nCunt] = SBUF;
			if('@' == nRecieveData[nCunt]){
				break;
			}
			
			nCunt++;
		}
	}
	if(strstr(nRecieveData,"B") != NULL){ 
		Version = Version_B;
		SendStringDataToPC("\r\nTrimming Program Version is B\r\n");
	}
	else if(strstr(nRecieveData,"C") != NULL){ 
		Version = Version_C;
		SendStringDataToPC("\r\nTrimming Program Version is C\r\n");
	}
	memset(nTempRxData,'\0',sizeof(nTempRxData));
	nCunt = 0;
	while(1){
		if('#' == nRecieveData[nCunt]){
			nCunt++;
			break;
		}
		nCunt++;
	}
	
	for(nCuntA = 0; nCuntA < 2; nCuntA++){ // 解析铂电阻的编号
		memset(nTempRxData,'\0',sizeof(nTempRxData));
		
		nBitCunt = 0;
		while(1){
			if((',' == nRecieveData[nCunt]) || ('@' ==  nRecieveData[nCunt]))
				break;
			
			else{			
				nTempRxData[nBitCunt] = nRecieveData[nCunt];
				nBitCunt++;
				nCunt++;
			}
		}
		
		VBEandRotation[nCuntA] = atoi(nTempRxData); // PT100的编号从“1”开始
		
		if('@' ==  nRecieveData[nCunt]){
			SendStringDataToPC("\r\n  ");
			if(Version == Version_B){
				UsartInit(ComPort_1,BTR115200);
				SendStringDataToPC("Rotation:");
				SendIntDataSelect(ComPort_1,VBEandRotation,1,2);		
			}				
			else{
				SendStringDataToPC("VBE and Rotation:");
				SendIntDataToPC(VBEandRotation,2);
			}	
			SendStringDataToPC("\r\n\r\n");
			break;		
		}
		
		else{
			nCunt++;			
		}
		
	}
}