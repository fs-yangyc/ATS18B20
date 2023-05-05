#include <SystemInit.h>
#include <DataTrans.h>
#include <NewPower.h>
#include <Chamber.h>
#include <cfguo.h>
#include <pc.h>
#include <TstBrdContrl.h>
#include <DelayTime.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAXTEMPDATACUNT 10

#define PASS 0x00
#define FAIL 0x01

#define DEBUG_MODE 0x00
#define NORMAL_MODE 0x01

// #define MAXTEMPDATACUNT	10

#define TemperaturePoint_1 1
#define TemperaturePoint_3 3
#define TemperaturePoint_6 6
#define TemperaturePoint_10 10
#define TemperaturePoint_11 11

unsigned char nTestTemperaturePoint;

extern bit bStartFlag, bInitFlag, bRunModeFlag, bNextStartFlag; // 来源于 “SystemInit.c”
extern char nBoardIsHerePwr[8];									// 来源于 “TstBrdContrl.c”
extern char nPowerTestBoardIndex[8];							// 来源于 “TstBrdContrl.c”
extern char xdata nRecieveData[200];							// 来源于 “DataTrans.c”
extern float fXJYData[32];										// 来源于 “Chamber.c”
unsigned char RotationValues;
int VBEandRotation[2];
extern unsigned char Version;
// SOP8-5.0V,PT100,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31@

float code fTrimingTemperature[2] = {20.0, 85.0}; // 修调温度点

float code fSetChamberTemperature_10[10] = {-55.0, -40.0, -25.0, -10.0, 0.0, 25.0, 50.0, 85.0, 100.0, 125.0}; // 全温区温度点
float code fUpOffset_10[10] = {2.15, 2.15, 2.15, 0.65, 0.65, 0.65, 0.65, 0.65, 2.15, 2.15};					  // 全温区温度误差
float code fLoOffset_10[10] = {-1.85, -1.85, -1.85, -0.35, -0.35, -0.35, -0.35, -0.35, -1.85, -1.85};		  // 全温区温度误差

/*float code fSetChamberTemperature_6[6] = {-10.0,0.0,25.0,45.0,60.0,85.0}; // 全温区温度点
float code fUpOffset_6[6] = {0.5,0.5,0.5,0.5,0.5,0.5};  // 全温区温度误差
float code fLoOffset_6[6] = {-0.5,-0.5,-0.5,-0.5,-0.5,-0.5};  // 全温区温度误差
*/

float code fSetChamberTemperature_6[6] = {-55.0, -10.0, 25.0, 60.0, 85.0, 125.0}; // 全温区温度点            //
float code fUpOffset_6[6] = {2.0, 0.5, 0.5, 0.5, 0.5, 2.0};						  // 全温区温度误差
float code fLoOffset_6[6] = {-2.0, -0.5, -0.5, -0.5, -0.5, -2.0};				  // 全温区温度误差

float code fSetChamberTemperature_3[3] = {-10.0, 25.0, 85.0}; // 全温区温度点
float code fUpOffset_3[3] = {0.5, 0.5, 0.5};				  // 全温区温度2.0误差
float code fLoOffset_3[3] = {-0.5, -0.5, -0.5};				  // 全温区温度误差

float code fSetChamberTemperature_1[1] = {25.0}; // 全温区温度点
float code fUpOffset_1[1] = {0.5};				 // 全温区温度误差
float code fLoOffset_1[1] = {-0.5};				 // 全温区温度误差

float code fSetChamberTemperature_11[11] = {-55.0, -40.0, -30.0, -10.0, 10.0, 25.0, 45.0, 60.0, 85.0, 100.0, 125.0}; // 全温区温度点
float code fUpOffset_11[11] = {2.0, 2.0, 2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.0, 2.0};								 // 全温区温度误差
float code fLoOffset_11[11] = {-2.0, -2.0, -2.0, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -2.0, -2.0};					 // 全温区温度误差

char DefaultPT100[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // 默认PT100序号 +1为实际号码

float fSetChamberTemperature[11]; // 全温区温度点
float fUpOffset[11];			  // 全温区温度误差
float fLoOffset[11];			  // 全温区温度误差

float fVccVoltage[3] = {3.0, 3.3, 3.6}; // 全温区电压点

// 调试变量
float xdata LabChamTemp;
unsigned char xdata aa, bb, cc = 0, dd, nTimeCunt;

/*******温箱稳定判断所需变量********/
unsigned char nChamberStable = 0;
bit bChamberStableFlag = 1;
bit bTestFlag = 1;

void main(void)
{
	char nTestMainBoardIndex, nTestBoardCunt, nChipCunt, nChipPackageType, nNeedPt100Number, nInputPt100NumberFromPc, nPt100Cunt, i, j, k;
	char nPT100Index[32], nTemperaturePoint, nTempDataCunt, nBoardPt100, nUseBoardCunt, nMaxChipCuntOneBoard = 48, nPassFailResult[8][48], nVoltagePoint, nTrimOrRead;
	bit bPackageIshere, bVoltageIsHere, bPt100IsHere, bRunMode, bDefaultFlag = 0;
	float xdata fXJYDataAverage[32]; // 保存巡检仪温度的平均值
	float xdata fTemperature[48];	 // 接收校准板的温度数据
	float xdata fTemperatureAverage[8][48], fBoardCfguoData[4];
	int xdata nTemperature[48];
	float xdata fPt100Offset, fVcc, fSupplyCurrent = 0.0; // 记录电源电流是否超标，如果超标则系统报错
	SystemInit();
	Delay_MS(4000);
	SendStringDataToPC("System Init...\r\n");
	SendStringDataToPC("\r\n	Program Version V1.6.1...\r\n");
	SendStringDataToPC("\r\nPlease Send Commad : Start Debug/Normal Read/Trim TemperaturePoints@\r\n");
	SendStringDataToPC("\r\nTemperaturePoints : One,Three,Six,Ten,Eleven\r\n");
	SendStringDataToPC("\r\nAdd 'Default' can skip input Pt100\r\n");
	SendStringDataToPC("\r\nDefault : TO92-3.3V,PT100,1,2,3,4,5,6,7,8@\r\n\r\n");
SysInit:
	EX0 = 0x01; // 开外部中断0 等待按键
	ES = 0x01;	// 开串口中断等待按键命令
	while (1)
	{
		if (bStartFlag)
		{
			if ((strstr(nRecieveData, "DEBUG") != NULL) ||
				(strstr(nRecieveData, "debug") != NULL) ||
				(strstr(nRecieveData, "Debug") != NULL))
			{ // 调试模式
				bRunMode = DEBUG_MODE;
				SendStringDataToPC("  Debug_");
			}
			else
			{ // 正常模式
				bRunMode = NORMAL_MODE;

				if ((strstr(nRecieveData, "Default") != NULL) ||
					(strstr(nRecieveData, "DEFAULT") != NULL) ||
					(strstr(nRecieveData, "default") != NULL))
				{ // 默认

					bDefaultFlag = 1;

					for (aa = 0; aa < 8; aa++)
					{
						nPT100Index[aa] = DefaultPT100[aa];
					}
					nInputPt100NumberFromPc = 8;
					SendStringDataToPC("  Normal Default_");
				}
				else
				{
					SendStringDataToPC("  Normal_");
				}
			}
			if ((strstr(nRecieveData, "read") != NULL) ||
				(strstr(nRecieveData, "Read") != NULL) ||
				(strstr(nRecieveData, "READ") != NULL))
			{ // 只读温度
				nTrimOrRead = READING;
				nChipPackageType |= READING;
				SendStringDataToPC("Read  ");
			}
			if (bRunMode == NORMAL_MODE)
			{
				if ((strstr(nRecieveData, "trim") != NULL) ||
					(strstr(nRecieveData, "Trim") != NULL) ||
					(strstr(nRecieveData, "TRIM") != NULL))
				{
					nTrimOrRead = TRIMING;
					nChipPackageType |= TRIMING;
					SendStringDataToPC("Trim  ");
				}
			}
			/******************旋转值和温度点*****************************/
			/******************************/
			if (strstr(nRecieveData, "One") != NULL)
			{
				nTestTemperaturePoint = 1;
				SendStringDataToPC("1 TemperaturePoint\r\n\r\n");
			}
			else if (strstr(nRecieveData, "Three") != NULL)
			{
				nTestTemperaturePoint = 3;
				SendStringDataToPC("3 TemperaturePoint\r\n\r\n");
			}
			else if (strstr(nRecieveData, "Six") != NULL)
			{
				nTestTemperaturePoint = 6;
				SendStringDataToPC("6 TemperaturePoint\r\n\r\n");
			}
			else if (strstr(nRecieveData, "Ten") != NULL)
			{
				nTestTemperaturePoint = 10;
				SendStringDataToPC("10 TemperaturePoint\r\n\r\n");
			}
			else if (strstr(nRecieveData, "Eleven") != NULL)
			{
				nTestTemperaturePoint = 11;
				SendStringDataToPC("11 TemperaturePoint\r\n\r\n");
			}
			for (dd = 0; dd < nTestTemperaturePoint; dd++)
			{
				if (TemperaturePoint_1 == nTestTemperaturePoint)
				{ // 温度点设置
					fSetChamberTemperature[dd] = fSetChamberTemperature_1[dd];
					fUpOffset[dd] = fUpOffset_1[dd];
					fLoOffset[dd] = fLoOffset_1[dd];
				}
				else if (TemperaturePoint_3 == nTestTemperaturePoint)
				{
					fSetChamberTemperature[dd] = fSetChamberTemperature_3[dd];
					fUpOffset[dd] = fUpOffset_3[dd];
					fLoOffset[dd] = fLoOffset_3[dd];
				}
				else if (TemperaturePoint_6 == nTestTemperaturePoint)
				{
					fSetChamberTemperature[dd] = fSetChamberTemperature_6[dd];
					fUpOffset[dd] = fUpOffset_6[dd];
					fLoOffset[dd] = fLoOffset_6[dd];
				}
				else if (TemperaturePoint_10 == nTestTemperaturePoint)
				{
					fSetChamberTemperature[dd] = fSetChamberTemperature_10[dd];
					fUpOffset[dd] = fUpOffset_10[dd];
					fLoOffset[dd] = fLoOffset_10[dd];
				}
				else if (TemperaturePoint_11 == nTestTemperaturePoint)
				{
					fSetChamberTemperature[dd] = fSetChamberTemperature_11[dd];
					fUpOffset[dd] = fUpOffset_11[dd];
					fLoOffset[dd] = fLoOffset_11[dd];
				}
			}
			if (TRIMING == nTrimOrRead)
			{
				/******************************/
				SendStringDataToPC("\r\nPlease input Trimming Data EG: Version #VBE,Rotation  //Version: B or C@\r\n");
				SendStringDataToPC("Version B | Rotation Values: 0 to 15\r\n");
				SendStringDataToPC("Version C | Rotation Values: 0 to 7   VBE Values: 0 to 254\r\n");
				ReadTrimmingDataFromPC();
			}
			/*************************************************************/
			break;
		}
	}

	if (bRunMode == NORMAL_MODE)
	{

		SendStringDataToPC("Testing YSL Chamber...\r\n");
		SetChamberOn(YslChamber, TCXXXChamber);								// 启动温箱
		ReadChamberTemperature(YslChamber, TCXXXChamber, &LabChamTemp, PV); // 获取温箱当前温度
		Delay_MS(1000);
	}

	SetVoltage(0.0f);
	TstBrdPowerInit(); // 断开所有的给测试主板的供电的输出继电器G3VM-41BR

	//	memset(nRecieveData,'\0',sizeof(nRecieveData));
	//	SendStringDataToPC("\r\nPress RUN/STOP Key Or [INIT@] for Init System.\r\n");
	//
	//	ES = 0x01;  // 开串口中断等待按键命令

	bInitFlag = 0x01;
	while (1)
	{ // 给测试主板上电，然后初始化系统
		if (bInitFlag)
		{
			bInitFlag = 0x00;

			Delay_MS(100);
			SetTstBrdPowerOn(AllPwrTstBrd); // 所有的测试主板

			Delay_MS(2000);
			SearchTestMainBoard(&nTestMainBoardIndex); // 查找系统中的测试主板nTestMainBoardIndex.0 ~7 = TestBoard.1~8
			if (NoneTestBoard == nTestMainBoardIndex)
			{
				TstBrdPowerInit(); // 断开所有的给测试主板的供电的输出继电器G3VM-41BR
				SendStringDataToPC("\r\nThere is no test board in the current system. Please check that the test board is plugged in, and then turn on the power again!!!");

				goto SysInit;
			}

			SendStringDataToPC("OK.\r\nCurrent System Board : \r\n\r\n");
			SendStringDataToPC("\t+=========+=========+=========+=========+=========+=========+=========+=========+\r\n");
			SendStringDataToPC("\t| Board-1 | Board-2 | Board-3 | Board-4 | Board-5 | Board-6 | Board-7 | Board-8 |\r\n");
			SendStringDataToPC("\t+=========+=========+=========+=========+=========+=========+=========+=========+\r\n");
			SendStringDataToPC("\t|");
			nNeedPt100Number = 0;
			for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
			{
				if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
				{
					SendStringDataToPC("   Yes   |");
					nNeedPt100Number += 4; // 实际需要使用的PT100的根数
				}

				else
				{
					SendStringDataToPC("   N/A   |");
				}
			}
			SendStringDataToPC("\r\n");
			SendStringDataToPC("\t+=========+=========+=========+=========+=========+=========+=========+=========+\r\n");

			SetTstBrdPowerOn(nTestMainBoardIndex); // 系统中存在的测试主板
			SendStringDataToPC("\r\nSystem Init end.\r\n");
			break;
		}
	}
	//***	/

	if (bRunMode == NORMAL_MODE)
	{
		if (bDefaultFlag == 0)
		{
			SendStringDataToPC("Please enter the Package Type of the 18B20 and the voltage of the Vcc Pin\r\nAnd enter the [%d] PT100s(1<= Number <= 32) used in the system in order(NormalEg: TO92-3.3V,PT100,1,4,7,8@ or DebugEg: TO92-3.3V,@):\r\n");
			while (1)
			{

				ReadTC18B20PackageAndUsePt100Index(&nChipPackageType, nPT100Index);
				SendStringDataToPC(nRecieveData);
				SendStringDataToPC("\r\n");
				memset(nRecieveData, '\0', sizeof(nRecieveData));

				bPackageIshere = FALSE;
				bVoltageIsHere = FALSE;

				if (TO92 == (nChipPackageType & 0x07))
				{
					bPackageIshere = TRUE;
					fPt100Offset = 0.4;
				}

				else if (CDFN8 == (nChipPackageType & 0x07))
				{
					bPackageIshere = TRUE;
					fPt100Offset = 0.4;
				}

				else if (SOP8 == (nChipPackageType & 0x07))
				{
					bPackageIshere = TRUE;
					fPt100Offset = 0.4;
				}

				if (UserV3V3 == (nChipPackageType & 0xf0))
				{
					bVoltageIsHere = TRUE;
					fVcc = 3.3;

					//			fVccVoltage[0] = 3.0;
					//			fVccVoltage[1] = 3.3;
					//			fVccVoltage[2] = 3.6;

					fVccVoltage[0] = 2.7;
					fVccVoltage[1] = 3.3;
					fVccVoltage[2] = 5.5;
				}

				else if (UserV5V == (nChipPackageType & 0xf0))
				{
					bVoltageIsHere = TRUE;
					fVcc = 5.0;

					fVccVoltage[0] = 4.75;
					fVccVoltage[1] = 5.00;
					fVccVoltage[2] = 5.25;
				}

				if ((!bPackageIshere) || (!bVoltageIsHere))
				{
					SendStringDataToPC("\r\nPackage Type Or Voltage Value is Wrong!!! Please re-enter !!!");
				}

				else
				{
					bPt100IsHere = TRUE;
					if (bRunMode == NORMAL_MODE)
					{
						nInputPt100NumberFromPc = 0;
						for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
						{
							if (nPT100Index[nPt100Cunt] < 33)
							{
								nInputPt100NumberFromPc++;
							}
						}

						if (nInputPt100NumberFromPc != nNeedPt100Number)
						{
							sprintf(nRecieveData, "Enter the number of PT100 are not correct.The system actually needs %d PT100s and the actual input PT100(s) is %d. Please re-enter !!!", (int)nNeedPt100Number, (int)nInputPt100NumberFromPc);
							SendStringDataToPC(nRecieveData);
						}

						else
						{
							for (i = 0; i < nNeedPt100Number - 1; i++)
							{ // 判断铂电阻的编号是否有重号
								for (j = i + 1; j < nNeedPt100Number; j++)
								{
									if (nPT100Index[i] == nPT100Index[j])
									{ // 铂电阻的编号有重号
										bPt100IsHere = FALSE;
										break;
									}
								}

								if (!bPt100IsHere)
									break;
							}

							if (bPt100IsHere) // 退出 while(1)
								break;

							else
							{
								SendStringDataToPC("The number of the input PT100 is duplicate, please re-enter !!! \r\n"); // 输入铂电阻的编号有重号，请重新输入
							}
						}
					}
					else
						break;
				} // else
			}
		}
		else
		{
			fPt100Offset = 0.4;
			fVcc = 3.3;

			nChipPackageType |= 0x77;

			fVccVoltage[0] = 2.7;
			fVccVoltage[1] = 3.3;
			fVccVoltage[2] = 5.5;
		}
	}

	//**/
	memset(nRecieveData, '\0', sizeof(nRecieveData));
	EX0 = 0x01; // 开外部中断0 等待按键
	ES = 0x01;	// 开串口中断等待按键命令
	while (1)
	{
		if (bStartFlag)
		{
			bStartFlag = 0x00;

			SendTC18B20PackageTypeToBoard(nTestMainBoardIndex, nChipPackageType & 0x0f);
			Delay_MS(5000);
			if (TRIMING == nTrimOrRead)
			{
				SendTrimmingVBEandRotationValues(nTestMainBoardIndex, VBEandRotation);
			}
			///////////////////////////////////////////////// 检测DUT有没有大电流或者短路 /////////////////////////////////////////////////

			SetVoltage(fVcc); // 检测DUT有没有大电流或者短路
			Delay_MS(100);
			ReadOPA544OutputCurrent(ADS1100_PGA_GAIN_2, 3, &fSupplyCurrent);
			SetVoltage(0.0f);
			sprintf(nRecieveData, "Isum = %f A", fSupplyCurrent);
			SendStringDataToPC(nRecieveData);
			if (fSupplyCurrent > 2.0)
			{
				SendStringDataToPC("\r\n The DUT power supply current is over the limit, please check to see if [DUT] is short-circuited !!!and then power it back on\r\n");
				while (1)
					;
			}

			///////////////////////////////////////////////// 打印排头 /////////////////////////////////////////////////
			SendStringDataToPC("\r\nVcc(V)\tTa(Celsius)\t");
			for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
			{
				if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
				{
					for (nChipCunt = 1; nChipCunt < 49; nChipCunt++)
					{
						sprintf(nRecieveData, "%.3d\t", (int)(48 * nTestBoardCunt + nChipCunt));
						SendStringDataToPC(nRecieveData);
					}
				}
			}
			if (bRunMode == NORMAL_MODE)
			{
				for (nPt100Cunt = 0; nPt100Cunt < nNeedPt100Number; nPt100Cunt++)
				{
					sprintf(nRecieveData, "PT100-%.2d\t", (int)(nPT100Index[nPt100Cunt] + 1));
					SendStringDataToPC(nRecieveData);
				}
			}
			SendStringDataToPC("Icc_sum(A)\r\n\r\n");

			///////////////////////////////////////////////// 修调过程开始 /////////////////////////////////////////////////
			Delay_MS(1000);

			if (bTestFlag == 0)
			{
				if (bRunMode == NORMAL_MODE)
				{
					//					SetChamberOn(LabEventChamber,TCXXXChamber); // 启动温箱
				}
			}

			Delay_MS(1000);

			if (TRIMING == nTrimOrRead)
			{
				/**********************************************
														校准程序开始
				***********************************************/
				SendStringDataToPC("\r\nTrimming Start ......\r\n");
				if (bTestFlag == 0)
				{
					/*************20度温箱校准*****************/
					sprintf(nRecieveData, "\r\n\r\n\tSet chamber's temperature to %.2f Celsius.\r\n", 25.0);
					SendStringDataToPC(nRecieveData);
					//					SetChamberTemperature(LabEventChamber,TCXXXChamber,25.0);

					/**************温箱稳定程序**********************/
					/*(判断模式区分 0:到达温度点30分钟 1:温箱稳定到 温度稳定值 以下，温度点，温度稳定值，稳定采样次数)*/
					//  				WaitChamberStable(0,20.0,0.5,8,0.1,5);

					for (i = 0; i < 60; i++)
					{
						Delay_MS(30000);
					}

					/************************************************/
				}
				/********************************************VBE  修调  开始*****************************************/
				Delay_MS(3000);
				SetVoltage(fVcc);
				sprintf(nRecieveData, "\r\n\t\tVcc = %.2f V\r\n", fVcc);
				SendStringDataToPC(nRecieveData);

				for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
				{
					fXJYDataAverage[nPt100Cunt] = 0.0f;
				}

				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					for (nChipCunt = 0; nChipCunt < 48; nChipCunt++)
					{
						fTemperatureAverage[nTestBoardCunt][nChipCunt] = 0.0f;
					}
				}

				Delay_MS(3000);
				SendStringDataToPC("\t\tWrite data : ");
				SendIntDataToPC(VBEandRotation, 2);
				SendStringDataToPC("\r\n\r\n");

				StartProcess();
				WaitForTrimingProcess(nTestMainBoardIndex); // 等待测试主板写暂存

				for (nTempDataCunt = 0; nTempDataCunt < MAXTEMPDATACUNT; nTempDataCunt++)
				{
					SendStringDataToPC("\t\t");
					//				if(NORMAL_MODE == bRunMode){
					if (RequireCFGuoAllChannelTemperature(fXJYData))
					{
						for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
						{ // 巡检仪温度的平均值
							fXJYDataAverage[nPt100Cunt] += fXJYData[nPt100Cunt] / MAXTEMPDATACUNT;
						}
					}

					else
					{											 // 巡检仪出错
						SetChamberOff(YslChamber, TCXXXChamber); // 关闭温箱
						SetVoltage(0.0f);

						SendStringDataToPC("The number of consecutive errors received by the XJY has reached 10 times. Please check that the XJY has failed!!!\r\n\tand then power it back on.\r\n");
						while (1)
							;
					}
					//					}

					TrimingBoardCollector(); // 启动校准板的温度采集
					Delay_MS(100);			 // 注意这个时间不能太短
					ReadOPA544OutputCurrent(ADS1100_PGA_GAIN_2, 3, &fSupplyCurrent);

					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{
						if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
						{
							memset(nRecieveData, '\0', sizeof(nRecieveData));
							ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);
							for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
							{
								fTemperature[nChipCunt] = (float)nTemperature[nChipCunt] / 16.0;
								fTemperatureAverage[nTestBoardCunt][nChipCunt] += fTemperature[nChipCunt] / MAXTEMPDATACUNT;
							}

							SendFloatDataToPC(fTemperature, nMaxChipCuntOneBoard);
						}
					}

					for (nPt100Cunt = 0; nPt100Cunt < nInputPt100NumberFromPc; nPt100Cunt++)
					{
						SendFloatDataToPC(fXJYData + nPT100Index[nPt100Cunt], 1);
					}

					sprintf(nRecieveData, "%f\r\n", fSupplyCurrent);
					SendStringDataToPC(nRecieveData); // 打印电流值
				}

				SendStringDataToPC("\r\n\t18B20 Temperature Average Value :\r\n\t\t");
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						SendFloatDataToPC(fTemperatureAverage[nTestBoardCunt], nMaxChipCuntOneBoard);
					}
				}

				for (nPt100Cunt = 0; nPt100Cunt < nInputPt100NumberFromPc; nPt100Cunt++)
				{
					SendFloatDataToPC(fXJYDataAverage + nPT100Index[nPt100Cunt], 1);
				}

				SendStringDataToPC("\r\n\r\n");
				Delay_MS(10); // 这个等待时间是有必要的,送PT100的数据给测试主板

				nUseBoardCunt = 0x00;
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					memset(nRecieveData, '\0', sizeof(nRecieveData));
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						nUseBoardCunt++;
						for (nBoardPt100 = 0; nBoardPt100 < 4; nBoardPt100++)
						{ // 每一块板4根铂电阻
							fBoardCfguoData[nBoardPt100] = fXJYDataAverage[nPT100Index[4 * (nUseBoardCunt - 1) + nBoardPt100]];
						}

						SendPt100DataToTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], fBoardCfguoData);
					}
				}

				Delay_MS(3000);

				SendStringDataToPC("\r\nCal Offset data : \t\t");
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{ // 读取testboard计算所得平移熔丝代码
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						memset(nRecieveData, '\0', sizeof(nRecieveData));
						ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);

						SendIntDataToPC(nTemperature, nMaxChipCuntOneBoard);
					}
				}

				if (UserV5V != (nChipPackageType & 0xf0))
				{ // 用5V电源电压写E2PROM
					SetVoltage(5.0);
				}

				Delay_MS(3000);
				StartProcess();
				Delay_MS(100);
				TrimingBoardCollector();					// 启动写修调信息到 E2PROM
				WaitForTrimingProcess(nTestMainBoardIndex); // 等待测试主板写暂存

				SendStringDataToPC("\r\nRead VBE data : \t\t");
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						memset(nRecieveData, '\0', sizeof(nRecieveData));
						ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);

						SendIntDataToPC(nTemperature, nMaxChipCuntOneBoard);
					}
				}

				SendStringDataToPC("\r\nRead Offset data : \t\t");
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						memset(nRecieveData, '\0', sizeof(nRecieveData));
						ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);

						SendIntDataToPC(nTemperature, nMaxChipCuntOneBoard);
					}
				}

				SendStringDataToPC("\r\nRotation Values : \t\t");
				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						memset(nRecieveData, '\0', sizeof(nRecieveData));
						ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);

						SendIntDataToPC(nTemperature, nMaxChipCuntOneBoard);
					}
				}
				Delay_MS(1000);

				for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
				{
					fXJYDataAverage[nPt100Cunt] = 0.0f;
				}

				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					for (nChipCunt = 0; nChipCunt < 48; nChipCunt++)
					{
						fTemperatureAverage[nTestBoardCunt][nChipCunt] = 0.0f;
					}
				}

				for (nTempDataCunt = 0; nTempDataCunt < MAXTEMPDATACUNT; nTempDataCunt++)
				{
					SendStringDataToPC("\r\n\t\t");

					if (NORMAL_MODE == bRunMode)
					{
						if (RequireCFGuoAllChannelTemperature(fXJYData))
						{
							for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
							{ // 巡检仪温度的平均值
								fXJYDataAverage[nPt100Cunt] += fXJYData[nPt100Cunt] / MAXTEMPDATACUNT;
							}
						}

						else
						{											 // 巡检仪出错
							SetChamberOff(YslChamber, TCXXXChamber); // 关闭温箱
							SetVoltage(0.0f);

							SendStringDataToPC("The number of consecutive errors received by the XJY has reached 10 times. Please check that the XJY has failed!!!\r\n\tand then power it back on.\r\n");
							while (1)
								;
						}
					}

					TrimingBoardCollector(); // 启动校准板的温度采集
					Delay_MS(100);			 // 注意这个时间不能太短
					ReadOPA544OutputCurrent(ADS1100_PGA_GAIN_2, 3, &fSupplyCurrent);

					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{
						if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
						{
							memset(nRecieveData, '\0', sizeof(nRecieveData));
							ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);
							for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
							{
								fTemperature[nChipCunt] = (float)nTemperature[nChipCunt] / 16.0;
								fTemperatureAverage[nTestBoardCunt][nChipCunt] += fTemperature[nChipCunt] / MAXTEMPDATACUNT;
							}

							SendFloatDataToPC(fTemperature, nMaxChipCuntOneBoard);
						}
					}

					SendStringDataToPC("\t\t\t\t");
					sprintf(nRecieveData, "%f", fSupplyCurrent);
					SendStringDataToPC(nRecieveData); // 打印电流值
				}

				SendStringDataToPC("\r\n\tTC18B20 Temperature Average Value :\r\n\t\t");

				for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
				{
					if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
					{
						SendFloatDataToPC(fTemperatureAverage[nTestBoardCunt], nMaxChipCuntOneBoard);
					}
				}

				for (nPt100Cunt = 0; nPt100Cunt < nInputPt100NumberFromPc; nPt100Cunt++)
				{
					SendFloatDataToPC(fXJYDataAverage + nPT100Index[nPt100Cunt], 1);
				}

				sprintf(nRecieveData, "\r\n\r\n%.2f\t%.2f\t", fVcc, fTrimingTemperature[0]);
				SendStringDataToPC(nRecieveData);

				if (TO92 == (nChipPackageType & 0x07))
				{ // 计算偏差
					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{
						if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
						{
							for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
							{
								fTemperatureAverage[nTestBoardCunt][nChipCunt] = fTemperatureAverage[nTestBoardCunt][nChipCunt] - fXJYDataAverage[nPT100Index[4 * nTestBoardCunt + nChipCunt / 12]];
							}
							SendFloatDataToPC(fTemperatureAverage[nTestBoardCunt], nMaxChipCuntOneBoard); // 温度误差值送PC显示
						}
					}
				}
				SetVoltage(0.0f); // 芯片断电
				SendStringDataToPC("\r\nTriming Process finished\r\n");
				/*********修调后直接全温区测温请屏蔽此处*********
				SendStringDataToPC("\r\n18B20 Reading Temperature Process finished,Send [NextStart@] again for next starting ...\r\n");
				ES = 0x01;
				EX0 = 0x01;
				while(1);
				/*******************屏蔽结束***********************/
			}

			/********************************************VBE  修调  结束*****************************************/

			////////////////////////////////// 修调完毕，全温区测温 //////////////////////////////////
			ES = 0x01;
			EX0 = 0x01;

			SendStringDataToPC("\r\n Temperature measurement at full voltage in full temperature area start ......\r\n");
			for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
			{
				for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
				{
					nPassFailResult[nTestBoardCunt][nChipCunt] = PASS;
				}
			}

			for (nTemperaturePoint = 0; nTemperaturePoint < nTestTemperaturePoint; nTemperaturePoint++)
			{
				sprintf(nRecieveData, "\r\n\r\n\tSet chamber's temperature to %.2f Celsius.\r\n", fSetChamberTemperature[nTemperaturePoint]);
				SendStringDataToPC(nRecieveData);
				if (bTestFlag == 0)
				{
					if (bRunMode == NORMAL_MODE)
					{
						//						SetChamberTemperature(LabEventChamber,TCXXXChamber,fSetChamberTemperature[nTemperaturePoint]);
						/**************温箱稳定程序**********************/
						//					if(fSetChamberTemperature[nTemperaturePoint] != -55.0){
						//						if(fSetChamberTemperature[nTemperaturePoint] < 25.0 ){
						//							WaitChamberStable(0,fSetChamberTemperature[nTemperaturePoint],0.5,8,0.1,5);
						//						}
						//						else{
						//							WaitChamberStable(1,fSetChamberTemperature[nTemperaturePoint],0.3,8,0.1,3);
						//						}
						//					}
						/*****************************************************************/
						for (k = 0; k < 120; k++) // 1 hour
						{
							Delay_MS(30000);
						}
					}
				}
				for (nVoltagePoint = 0; nVoltagePoint < 3; nVoltagePoint++)
				{
					for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
					{
						fXJYDataAverage[nPt100Cunt] = 0.0f;
					}

					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{
						for (nChipCunt = 0; nChipCunt < 48; nChipCunt++)
						{
							fTemperatureAverage[nTestBoardCunt][nChipCunt] = 0.0f;
						}
					}

					SetVoltage(fVccVoltage[nVoltagePoint]);
					Delay_MS(5000);
					StartProcess();
					Delay_MS(20);
					for (nTempDataCunt = 0; nTempDataCunt < MAXTEMPDATACUNT; nTempDataCunt++)
					{
						SendStringDataToPC("\t\t");
						if (RequireCFGuoAllChannelTemperature(fXJYData))
						{
							for (nPt100Cunt = 0; nPt100Cunt < 32; nPt100Cunt++)
							{ // 巡检仪温度的平均值
								fXJYDataAverage[nPt100Cunt] += fXJYData[nPt100Cunt] / MAXTEMPDATACUNT;
							}
						}

						else
						{ // 巡检仪出错

							//							SetChamberOff(LabEventChamber,TCXXXChamber); // 关闭温箱
							SetVoltage(0.0f);

							SendStringDataToPC("The number of consecutive errors received by the XJY has reached 10 times. Please check that the XJY has failed!!!\r\n\tand then power it back on.\r\n");
							while (1)
								;
						}

						TrimingBoardCollector(); // 启动校准板的温度采集
						Delay_MS(100);			 // 注意这个时间不能太短
						ReadOPA544OutputCurrent(ADS1100_PGA_GAIN_2, 3, &fSupplyCurrent);

						for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
						{
							if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
							{
								memset(nRecieveData, '\0', sizeof(nRecieveData));
								ReadTemperatureDataFromTrimingBoard(nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt], nTemperature);
								for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
								{
									fTemperature[nChipCunt] = (float)nTemperature[nChipCunt] / 16.0;
									fTemperatureAverage[nTestBoardCunt][nChipCunt] += fTemperature[nChipCunt] / MAXTEMPDATACUNT;
								}

								SendFloatDataToPC(fTemperature, nMaxChipCuntOneBoard);
							}
						}
						for (nPt100Cunt = 0; nPt100Cunt < nInputPt100NumberFromPc; nPt100Cunt++)
						{
							SendFloatDataToPC(fXJYData + nPT100Index[nPt100Cunt], 1);
						}

						sprintf(nRecieveData, "%f\r\n", fSupplyCurrent);
						SendStringDataToPC(nRecieveData); // 打印电流值
					}

					SendStringDataToPC("\r\n\tTC18B20 & XJY Temperature Average Value :\r\n\t\t");
					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{
						if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
						{
							SendFloatDataToPC(fTemperatureAverage[nTestBoardCunt], nMaxChipCuntOneBoard);
						}
					}

					for (nPt100Cunt = 0; nPt100Cunt < nInputPt100NumberFromPc; nPt100Cunt++)
					{
						SendFloatDataToPC(fXJYDataAverage + nPT100Index[nPt100Cunt], 1);
					}

					sprintf(nRecieveData, "\r\n\r\n%.2f\t%.2f\t", fVccVoltage[nVoltagePoint], fSetChamberTemperature[nTemperaturePoint]);
					SendStringDataToPC(nRecieveData);

					if (TO92 == (nChipPackageType & 0x07))
					{ // 计算偏差
						for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
						{
							if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
							{
								for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
								{
									fTemperatureAverage[nTestBoardCunt][nChipCunt] = fTemperatureAverage[nTestBoardCunt][nChipCunt] - fXJYDataAverage[nPT100Index[4 * nTestBoardCunt + nChipCunt / 12]];
									if ((fTemperatureAverage[nTestBoardCunt][nChipCunt] > fUpOffset[nTemperaturePoint]) ||
										(fTemperatureAverage[nTestBoardCunt][nChipCunt] < fLoOffset[nTemperaturePoint]))
									{
										if (nPassFailResult[nTestBoardCunt][nChipCunt] == PASS)
										{
											nPassFailResult[nTestBoardCunt][nChipCunt] = FAIL;
										}
									}
								}

								SendFloatDataToPC(fTemperatureAverage[nTestBoardCunt], nMaxChipCuntOneBoard); // 温度误差值送PC显示
							}
						}
					}
					SendStringDataToPC("\r\n\t\t");
					for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
					{ // 打印测温结果到文件之中
						if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
						{
							for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
							{
								if ((fTemperatureAverage[nTestBoardCunt][nChipCunt] > fUpOffset[nTemperaturePoint]) ||
									(fTemperatureAverage[nTestBoardCunt][nChipCunt] < fLoOffset[nTemperaturePoint]))
								{
									SendStringDataToPC("Fail\t");
								}

								else
								{
									SendStringDataToPC("Pass\t");
								}
							}
						}
					}

					SendStringDataToPC("\r\n\r\n");

				} // 4 voltage
				SetVoltage(0.0f);
			}
			////////////////////////////////////////////////////////// 全温区全电压测温结束/////////////////////////////////////////////////////

			EndAllProcess(nTestMainBoardIndex);
			SendStringDataToPC("\r\nFinal Result:\t\t");
			for (nTestBoardCunt = 0; nTestBoardCunt < 8; nTestBoardCunt++)
			{ // 打印测温结果到文件之中
				if (nTestMainBoardIndex & nBoardIsHerePwr[nTestBoardCunt])
				{
					for (nChipCunt = 0; nChipCunt < nMaxChipCuntOneBoard; nChipCunt++)
					{
						if (nPassFailResult[nTestBoardCunt][nChipCunt] != PASS)
						{
							SendStringDataToPC("Fail\t");
						}

						else
						{
							SendStringDataToPC("Pass\t");
						}
					}
				}
			}
			SendStringDataToPC("\r\n\r\n");
			//			SetChamberOff(LabEventChamber,TCXXXChamber); // 关闭温箱

			SendStringDataToPC("\r\n18B20 Reading Temperature Process finished,Send [NextStart@] again for next starting ...\r\n");
			ES = 0x01;
			EX0 = 0x01;
			while (1)
				;
		}
	}
}
