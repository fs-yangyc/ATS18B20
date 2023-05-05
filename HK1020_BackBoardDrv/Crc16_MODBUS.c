#include <Crc16_MODBUS.h>

void InvertUint8(unsigned char *dBuf,unsigned char *srcBuf)  
{  
    int i;  
    unsigned char tmp[4];  
    tmp[0] = 0;  
    for(i=0; i< 8; i++)  
    {  
      if(srcBuf[0] & (1 << i))  
        tmp[0]|=1<<(7-i);  
    }  
    dBuf[0] = tmp[0];  
}

void InvertUint16(unsigned int *dBuf,unsigned int *srcBuf)  
{  
    int i;  
    unsigned int tmp[4];  
    tmp[0] = 0;  
    for(i=0;i< 16;i++)  
    {  
      if(srcBuf[0]& (1 << i))  
        tmp[0]|=1<<(15 - i);  
    }  
    dBuf[0] = tmp[0];  
}

unsigned int CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)  
{  
	unsigned int wCRCin = 0xFFFF;  
	unsigned int wCPoly = 0x8005;  
	unsigned char wChar = 0; 
	int i;
    
	while (usDataLen--)     
	{  
        wChar = *(puchMsg++);  
        InvertUint8(&wChar,&wChar);  
        wCRCin ^= (wChar << 8);  
        for(i = 0; i < 8; i++)  
        {  
          if(wCRCin & 0x8000)  
            wCRCin = (wCRCin << 1) ^ wCPoly;  
          else  
            wCRCin = wCRCin << 1;  
        }  
	} 
	
	InvertUint16(&wCRCin,&wCRCin);  
	return (wCRCin);  
}