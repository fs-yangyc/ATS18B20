#include <NewPower.h>
#include <DelayTime.h>

unsigned char code nPwrMAX5821[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
void ResetMax5821I2CPort(void)
{
	MAX5821_SCL = 0x01;
	MAX5821_SDA = 0x01;
}

void MAX5821I2CStart(void)
{
	ResetMax5821I2CPort();
	Delay_MS(1);
	
	MAX5821_SDA = 0x00;
	Delay_5US();
	MAX5821_SCL = 0x00;
	Delay_5US();
}

void MAX5821I2CAck(void)
{
	MAX5821_SDA = 0x01;
	Delay_5US();
	MAX5821_SCL = 0x01;
	Delay_5US();
	MAX5821_SCL = 0x00;
	Delay_5US();
}

void MAX5821I2CStop(void)
{
	MAX5821_SDA = 0x00;
	Delay_5US();
	MAX5821_SCL = 0x01;
	Delay_5US();
	MAX5821_SDA = 0x01;
	Delay_5US();
}

void WriteDataToMAX5821(unsigned char nDeviceAddr,int nData, unsigned char nCommand)
{
	char nBitCunt;
	unsigned char nDataByte1,nDataByte2;
	
	nDataByte2 = nData;
	nDataByte1 = 0xc0 | (nData >> 0x08);
	
	MAX5821I2CStart();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nDeviceAddr & nPwrMAX5821[nBitCunt]){
			MAX5821_SDA = 0x01;
		}
		
		else{
			MAX5821_SDA = 0x00;
		}
		
		Delay_5US();
		MAX5821_SCL = 0x01;
		Delay_5US();
		MAX5821_SCL = 0x00;
	}
	
	MAX5821I2CAck();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nDataByte1 & nPwrMAX5821[nBitCunt]){
			MAX5821_SDA = 0x01;
		}
		
		else{
			MAX5821_SDA = 0x00;
		}
		
		Delay_5US();
		MAX5821_SCL = 0x01;
		Delay_5US();
		MAX5821_SCL = 0x00;
	}
	
	MAX5821I2CAck();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nDataByte2 & nPwrMAX5821[nBitCunt]){
			MAX5821_SDA = 0x01;
		}
		
		else{
			MAX5821_SDA = 0x00;
		}
		
		Delay_5US();
		MAX5821_SCL = 0x01;
		Delay_5US();
		MAX5821_SCL = 0x00;
	}
	
	MAX5821I2CAck();
	MAX5821I2CStop();
	
	MAX5821I2CStart();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nDeviceAddr & nPwrMAX5821[nBitCunt]){
			MAX5821_SDA = 0x01;
		}
		
		else{
			MAX5821_SDA = 0x00;
		}
		
		Delay_5US();
		MAX5821_SCL = 0x01;
		Delay_5US();
		MAX5821_SCL = 0x00;
	}
	
	MAX5821I2CAck();
	for(nBitCunt = 0; nBitCunt < 8; nBitCunt++){
		if(nCommand & nPwrMAX5821[nBitCunt]){
			MAX5821_SDA = 0x01;
		}
		
		else{
			MAX5821_SDA = 0x00;
		}
		
		Delay_5US();
		MAX5821_SCL = 0x01;
		Delay_5US();
		MAX5821_SCL = 0x00;
	}
	
	MAX5821I2CAck();
	MAX5821I2CStop();
}

void SetVoltage(float fVoltage)
{
	int nDigtal;
	float fVref = 5.0;
	
	char nOnOff = ON;
	
	if(ON == nOnOff){
		if(ON != POWER_N){
			WriteDataToMAX5821(0x20,0x0000,0x40);
			Delay_MS(10);
			POWER_N = ON;
			Delay_MS(50);
		}
		
		nDigtal = fVoltage * 4096.0 / (2.889 * fVref); // 输出电压被放大了3倍
		WriteDataToMAX5821(0x20,nDigtal,0x40);
		Delay_MS(50);
	}
	
	else{
		WriteDataToMAX5821(0x20,0x0000,0x40);
		Delay_MS(10);
		POWER_N = OFF;
		Delay_MS(50);
	}
}