#include <TstBrdContrl.h>
#include <DelayTime.h>
#include <DataTrans.h>
#include <string.h>
#include <stdlib.h>

extern char nTestBoardName[8]; 			// 来源于 “DataTrans.c”
extern unsigned int nComTimeOutCunt; 	// 记录串口超时,来源于文件“DataTrans.c”
char nTempP7PortData,nTestBoardIndex,nPowerTestBoardIndex[8] = {PwrTstBrd_1,PwrTstBrd_2,PwrTstBrd_3,PwrTstBrd_4,PwrTstBrd_5,PwrTstBrd_6,PwrTstBrd_7,PwrTstBrd_8};
char nBoardIsHerePwr[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
char nBoardIndex;
extern unsigned char RotationValues; 	
/**
*** 函数名称 ： TstBrdPowerInit
*** 函数功能 ： 关闭测试板和DUT的所有电源
*** 入口参数 ： 无
*** 返回值   ： 无
*/
void TstBrdPowerInit(void)
{
	P7 = AllTestBoardPowerOff;  // 控制三极管9013关闭，以切断测试板和DUT的所有电源
}

void SetTstBrdPowerOn(
	char nBoardIndex  			// 测试板号
)
{
	nTempP7PortData = 0x00;
	for(nTestBoardIndex = 0; nTestBoardIndex < 8; nTestBoardIndex++){
		if(nBoardIndex & nBoardIsHerePwr[nTestBoardIndex]){
			nTempP7PortData |= nPowerTestBoardIndex[nTestBoardIndex];
		}
	}
	
	P7 = ~nTempP7PortData;  // 控制三极管9012导通，以便给测试板和DUT的所有电源上电
}

/**
*** 函数名称 ： SearchTestMainBoard
*** 函数功能 ： 查找系统中测试主板的块数
*** 入口参数 ： 1、nTestMainBoardIndex  系统中测试主板的块数以及位置信息
*** 返回值   ： 无
*/

void SearchTestMainBoard(char *nTestMainBoardIndex)
{
	*nTestMainBoardIndex = 0x00;
	for(nBoardIndex = 0; nBoardIndex < 8; nBoardIndex++){
		nComTimeOutCunt = 0x0000;
		SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nBoardIndex],BTR115200);
		SendStringData(ComPort_2,"IS?@");
		
		S2CON &= 0xfe;
		while(1){
			if(S2CON & 0x01){ // 等待数据到
				S2CON &= 0xfe;
				if('Y' == S2BUF){
					*nTestMainBoardIndex |= nBoardIsHerePwr[nBoardIndex];
					break;
				}
			}
			
			if(++nComTimeOutCunt == 20000)
			break;
		}
	}
}

/**
*** 函数名称 ： EndAllProcess
*** 函数功能 ： 结束全温区全电压测试
*** 入口参数 ： 1、nTestMainBoardIndex  系统中测试主板的块数以及位置信息
*** 返回值   ： 无
*/

void EndAllProcess(char nTestMainBoardIndex)
{
	for(nBoardIndex = 0; nBoardIndex < 8; nBoardIndex++){
		if(nTestMainBoardIndex & nBoardIsHerePwr[nBoardIndex]){
			SetUsartTwoPosionAndBtr(Com2_TstBrd,nTestBoardName[nBoardIndex],BTR115200);
			SendStringData(ComPort_2,"END@");
		}
	}
}
