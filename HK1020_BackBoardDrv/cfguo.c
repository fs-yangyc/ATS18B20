#include <cfguo.h>
#include <Crc16_MODBUS.h>
#include <DelayTime.h>
#include <DataTrans.h>
#include <string.h>

#include <pc.h>
/**
*** 函数名称 ： RequireCFGuoAllChannelTemperature
*** 函数功能 ： 读取巡检仪所有通道的温度数据
*** 入口参数 ： 无
*** 返回值   ： 接收数据正确返回 1，错误返回 0
*/
char RequireCFGuoAllChannelTemperature(float *fTempData)
{
	char nUpdataCommand[10],nErrorCunt,nPassFail;
	unsigned char nDataCunt,cCfguoXJYRxData[250];
	int nData = 0,nTempDataCunt = 0;
	unsigned int nCRCCode,nXJYCrcCode;

RxDataStart:
	
	memset(nUpdataCommand,'\0',sizeof(nUpdataCommand));
	memset(cCfguoXJYRxData,'\0',sizeof(cCfguoXJYRxData));
	nDataCunt = 0;
	nErrorCunt = 0;
	
	UsartInterrupt(ComPort_2,COMDIS);
	SetUsartTwoPosionAndBtr(Com2_Other,CFG,BTR57600);
	
	Delay_MS(1);

	RS485RE = 0x00; // 设置MAX485接收使能
	S2CON &= 0xfe;
	while(1){
		// 查询方式接收数据
		if(S2CON & 0x01){ // 等待数据到
			S2CON &= 0xfe;
			if('<' == S2BUF){
				nUpdataCommand[nDataCunt] = S2BUF;
				nDataCunt++;
				while(1){
					if(S2CON & 0x01){ // 等待数据到
						S2CON &= 0xfe;
						nUpdataCommand[nDataCunt] = S2BUF;
						nDataCunt++;
						
						if(nDataCunt > 5)
						break;
						
						
					}
				}
			}
			
			if(strstr(nUpdataCommand,"<XJTH>") != NULL)
			break;
			
			nDataCunt = 0;
			memset(nUpdataCommand,'\0',sizeof(nUpdataCommand));
		}
	}
	
	RS485RE = 0x01;
	Delay_MS(1);
	
	S2CON &= 0xfd;
	S2BUF = 0x25;
	while((S2CON & 0x02) != 0x02);  // S4TI == 1

	Delay_MS(1);
	S2CON &= 0xfd;
	S2BUF = 0x23;
	while((S2CON & 0x02) != 0x02);  // S4TI == 1
	S2CON &= 0xfd;;

	RS485RE = 0x00; // 设置MAX485接收使能
	for(nDataCunt = 0; nDataCunt < 205; nDataCunt++){
		while(1){

			if(S2CON & 0x01){ // 等待数据到
				S2CON &= 0xfe;
				cCfguoXJYRxData[nDataCunt] = S2BUF;
				
				break;
			}
		}
	}
	 
	nCRCCode = CRC16_MODBUS(cCfguoXJYRxData,203); // 巡检仪前203个数据的CRC校验码
	nXJYCrcCode = cCfguoXJYRxData[204]; // CRC高位
	nXJYCrcCode <<= 0x08;
	nXJYCrcCode += cCfguoXJYRxData[203]; // CRC低位
	
	if(nCRCCode == nXJYCrcCode){ // 接收数据正确
		nErrorCunt = 0x00; // 接收数据正确,清除错误记录
		for(nDataCunt = 3; nDataCunt < 194; nDataCunt = nDataCunt + 6){
			nData = cCfguoXJYRxData[nDataCunt] & 0x07;
			nData = nData << 0x08;
			nData += cCfguoXJYRxData[nDataCunt + 1];
			fTempData[nTempDataCunt] = (float)nData + (float)(cCfguoXJYRxData[nDataCunt + 2]) / 100.0;
		
			if(cCfguoXJYRxData[nDataCunt] & 0x08){ // 负温度
				fTempData[nTempDataCunt] = -fTempData[nTempDataCunt];
			}
			nTempDataCunt++;
		}
		
		nPassFail = 0x01;
	}
	
	else{ // 接收数据不正确，重新接收
		nErrorCunt++;
		if(nErrorCunt > 10){
			nPassFail = 0x00;
		}
			
		else
		goto RxDataStart;
	}

	RS485RE = 0x01; // 恢复MAX485发送使能
	return nPassFail;
}