#include <NewPower.h>
#include <DelayTime.h>

unsigned char code nPwrDevice[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
void ResetI2CPort(void)
{
	NewPower_SCL = 0x01;
	NewPower_SDA = 0x01;
}

void I2CStart(void)
{
	ResetI2CPort();
	Delay_MS(1);
	
	NewPower_SDA = 0x00;
	Delay_5US();
	NewPower_SCL = 0x00;
	Delay_5US();
}

void I2CDeviceAck(void)
{
	NewPower_SDA = 0x01;
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	NewPower_SCL = 0x00;
}

void I2CStop(void)
{
	NewPower_SDA = 0x00;
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	NewPower_SDA = 0x01;
	Delay_5US();
}

void I2CWriteDataToDevice(unsigned char nDeviceAddr,unsigned char *nData, unsigned char nDataCunt)
{
	unsigned char nCunt,nBitCunt;
	
	I2CStart();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nDeviceAddr & nPwrDevice[nBitCunt]){
			NewPower_SDA = 0x01;
		}
		
		else{
			NewPower_SDA = 0x00;
		}
		
		Delay_5US();
		NewPower_SCL = 0x01;
		Delay_5US();
		NewPower_SCL = 0x00;
	}
	
	I2CDeviceAck();
	for(nCunt = 0; nCunt < nDataCunt; nCunt++){
		for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
			if(nData[nCunt] & nPwrDevice[nBitCunt]){
				NewPower_SDA = 0x01;
			}
			
			else{
				NewPower_SDA = 0x00;
			}
			
			Delay_5US();
			NewPower_SCL = 0x01;
			Delay_5US();
			NewPower_SCL = 0x00;
		}
	
		I2CDeviceAck();
	}
	
	I2CStop();
}

void ReadDataFromADS1100(unsigned char *nReadData)
{
	unsigned char nAds100Addr,nDataCunt,nBitCunt;
	
	for(nDataCunt = 0; nDataCunt < 3; nDataCunt++){
		nReadData[nDataCunt] = 0x00;
	}
	
	nAds100Addr = ADS1100_ADDR | 0x01; // Read_Addr
	I2CStart();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nAds100Addr & nPwrDevice[nBitCunt]){
			NewPower_SDA = 0x01;
		}
		
		else{
			NewPower_SDA = 0x00;
		}
		
		Delay_5US();
		NewPower_SCL = 0x01;
		Delay_5US();
		NewPower_SCL = 0x00;
	}
	
	I2CDeviceAck();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		Delay_5US();
		NewPower_SCL = 0x01;
		Delay_5US();
		if(NewPower_SDA){
			nReadData[0] |= nPwrDevice[nBitCunt];
		}
		
		else{
			nReadData[0] |= 0x00;
		}
		
		NewPower_SCL = 0x00;
	}
	
	NewPower_SDA = 0x00;  // ACK
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	
	NewPower_SCL = 0x00;
	NewPower_SDA = 0x01;
	Delay_5US();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		NewPower_SCL = 0x01;
		Delay_5US();
		if(NewPower_SDA){
			nReadData[1] |= nPwrDevice[nBitCunt];
		}
		
		else{
			nReadData[1] |= 0x00;
		}
		
		NewPower_SCL = 0x00;
		if(7 != nBitCunt){
			Delay_5US();
		}
	}
	
	NewPower_SDA = 0x00;  // ACK
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	
	NewPower_SCL = 0x00;
	NewPower_SDA = 0x01;
	Delay_5US();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		NewPower_SCL = 0x01;
		Delay_5US();
		if(NewPower_SDA){
			nReadData[2] |= nPwrDevice[nBitCunt];
		}
		
		else{
			nReadData[2] |= 0x00;
		}
		
		NewPower_SCL = 0x00;
		if(7 != nBitCunt){
			Delay_5US();
		}
	}
	
	NewPower_SDA = 0x00;  // ACK
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	
	NewPower_SCL = 0x00;
	Delay_5US();
	NewPower_SCL = 0x01;
	Delay_5US();
	NewPower_SDA = 0x01;  // STOP
	Delay_5US();
}

void WriteDataToMAX5821(unsigned char nDeviceAddr,int nData, unsigned char nCommand)
{
	unsigned char nWriteData[2];
	
	nWriteData[1] = nData;
	nWriteData[0] = MAX5812_LOAD_DAC | (nData >> 0x08);
	
	I2CWriteDataToDevice(nDeviceAddr,nWriteData,0x02);
	I2CWriteDataToDevice(nDeviceAddr,&nCommand,0x01);
}

void SetVoltage(float fVoltage)
{
	int nDigtal;
	float fVref = 5.0;
	
//	nDigtal = fVoltage * 4096.0 / (1.98 * fVref); // 输出电压被放大了2倍
	nDigtal = fVoltage * 4096.0 / (3 * fVref); // 输出电压被放大了3倍
	WriteDataToMAX5821(MAX5812_ADDR,nDigtal,0x40);
}

void ReadOPA544OutputCurrent(unsigned char nAds1100Gain,unsigned char nSampleCunt,float *fIccurent)
{
	unsigned char nConfigureData,nReadDataCunt,nReadData[3];
	int nAds100OutData = 0;
	float fIcc = 0.0,fIccGain,fVref = 5.0,fRSample = 0.5;
	
	*fIccurent = 0.0;
	if(ADS1100_PGA_GAIN_1 == nAds1100Gain){ // LM358放大了2倍
		fIccGain = 2.0;
	}
	
	if(ADS1100_PGA_GAIN_2 == nAds1100Gain){ // LM358放大了2倍
		fIccGain = 4.0;
	}
	
	if(ADS1100_PGA_GAIN_4 == nAds1100Gain){ // LM358放大了2倍
		fIccGain = 8.0;
	}
	
	if(ADS1100_PGA_GAIN_8 == nAds1100Gain){ // LM358放大了2倍
		fIccGain = 16.0;
	}
	
	nConfigureData = ADS1100_SC_SINGLE | ADS1100_DR_SPS_8 | nAds1100Gain;
	I2CWriteDataToDevice(ADS1100_ADDR,&nConfigureData,0x01);
	Delay_MS(1);
	nConfigureData = ADS1100_SC_SINGLE_START | ADS1100_SC_SINGLE | ADS1100_DR_SPS_8 | nAds1100Gain;
	for(nReadDataCunt = 0; nReadDataCunt < nSampleCunt; nReadDataCunt++){
		I2CWriteDataToDevice(ADS1100_ADDR,&nConfigureData,0x01); // 启动一次转换
		while(1){ // 等待转换结束
			ReadDataFromADS1100(nReadData);
			if(!(nReadData[2] & 0x80)){
				break;
			}
			
			else{
				Delay_MS(1);
			}
		}
		
		ReadDataFromADS1100(nReadData);
		
		nAds100OutData = nReadData[0];
		nAds100OutData <<= 0x08;
		nAds100OutData |= nReadData[1];
		
		fIcc = ((fVref * (float)nAds100OutData) / (32768.0 * fIccGain)) / fRSample;
		*fIccurent = *fIccurent + fIcc / ((float)nSampleCunt);
	}
}
