
/*CH395芯片命令接口文件,常用子程序外部调用*/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__

/* TCP模式，0为客户端，1为服务器 */
#define TCP_CLIENT_MODE   0
#define TCP_SERVER_MODE   1

/* 保活时间参数*/
#define DEF_KEEP_LIVE_IDLE                           15000            /* 空闲时间 */
#define DEF_KEEP_LIVE_PERIOD                         15000            /* 间隔为15秒，发送一次KEEPLIVE数据包 */                  
#define DEF_KEEP_LIVE_CNT                            200              /* 重试次数  */

void CH395CMDReset(void);                                            /* 复位 */

//void CH395CMDSleep(void);                                            /* 睡眠 */

//unsigned char CH395CMDGetVer(void);                                          /* 获取芯片及固件版本号 */

unsigned char CH395CMDCheckExist(unsigned char testdata);                            /* 测试命令 */

//void CH395CMDSetPHY(unsigned char phystat);                                  /* 设置PHY状态  */

unsigned char CH395CMDGetPHYStatus(void);                                    /* 获取PHY状态 */

unsigned char CH395CMDGetGlobIntStatus(void);                                /* 获取CH395全局中断状态 */

unsigned char CH395CMDInitCH395(void);                                       /* 初始化CH395 */

//void CH395CMDSetUartBaudRate(unsigned long baudrate);                       /* 设置波特率 */

unsigned char CH395GetCmdStatus(void);                                       /* 获取命令执行状态 */

void CH395CMDSetIPAddr(unsigned char *ipaddr);                               /* 设置CH395的IP地址 */

void CH395CMDSetGWIPAddr(unsigned char *gwipaddr);                           /* 设置CH395的网关IP地址 */

void CH395CMDSetMASKAddr(unsigned char *maskaddr);                           /* 设置子网掩码 */

void CH395CMDSetMACAddr(unsigned char *amcaddr);                             /* 设置CH395的MAC地址 */

//void CH395CMDGetMACAddr(unsigned char *amcaddr);                             /* 获取MAC地址 */

//void CH395CMDSetMACFilt(unsigned char filtype,unsigned long table0,unsigned long table1);  /* 设置CH395的MAC过滤 */

void CH395CMDGetUnreachIPPT(unsigned char *list);                            /* 获取不可达地址以及端口 */

//void CH395CMDGetRemoteIPP(unsigned char sockindex,unsigned char *list);              /* 获取远端IP和端口，一般在TCP Server下使用 */

void CH395SetSocketDesIP(unsigned char sockindex,unsigned char *ipaddr);             /* 设置scoket n的目的IP地址 */

void CH395SetSocketProtType(unsigned char sockindex,unsigned char prottype);         /* 设置socket n的协议类型 */

void CH395SetSocketDesPort(unsigned char sockindex,unsigned short desprot);          /* 设置socket n的目的端口 */

void CH395SetSocketSourPort(unsigned char sockindex,unsigned short surprot);         /* 设置socket n的源端口 */

//void CH395SetSocketIPRAWProto(unsigned char sockindex,unsigned char prototype);      /* 在IPRAW模式下，设置socket n的IP包协议字段 */

//void CH395SetRecvThresLen(unsigned char sockindex,unsigned short len);               /* 设置socket n的接收中断阀值 */

void CH395SendData(unsigned char sockindex,unsigned char *databuf,unsigned short len);       /* 向socket n的发送缓冲区写数据 */

unsigned short CH395GetRecvLength(unsigned char sockindex);                          /* 获取socket n的接收长度 */

//void CH395ClearRecvBuf(unsigned char sockindex);                             /* 清除socket n的接收缓冲区 */

void CH395GetRecvData(unsigned char sockindex,unsigned short len,unsigned char *pbuf);       /* 获取接收数据 */

//void CH395CMDSetRetryCount(unsigned char count);                             /* 设置最大重试次数 */

//void CH395CMDSetRetryPeriod(unsigned short period);                          /* 设置最大重试周期 单位 毫秒*/

//void CH395CMDGetSocketStatus(unsigned char sockindex,unsigned char *status) ;        /* 获取socket n的状态 */

unsigned char  CH395OpenSocket(unsigned char sockindex);                             /* 打开socket n*/

//unsigned char  CH395CloseSocket(unsigned char sockindex);                             /* 关闭socket n*/

unsigned char CH395TCPConnect(unsigned char sockindex);                               /* TCP连接 */

//unsigned char CH395TCPListen(unsigned char sockindex);                                /* TCP监听 */

//unsigned char CH395TCPDisconnect(unsigned char sockindex);                            /* TCP断开连接 */

unsigned char CH395GetSocketInt(unsigned char sockindex);                            /* 获取socket n的中断状态 */
                                                                     
//unsigned char CH395CRCRet6Bit(unsigned char *mac_addr);                              /* 多播地址CRC32，用于HASH过滤 */
                                                                     /*Function Count = 36 */
//void CH395GetIPInf(unsigned char *addr);

//unsigned char  CH395DHCPEnable(unsigned char flag);

//void CH395WriteGPIOAddr(unsigned char regadd,unsigned char regval);

//unsigned char CH395ReadGPIOAddr(unsigned char regadd);

//unsigned char CH395EEPROMErase(void);

//unsigned char CH395EEPROMWrite(unsigned short eepaddr,unsigned char *buf,unsigned char len);

//void CH395EEPROMRead(unsigned short eepaddr,unsigned char *buf,unsigned char len);

//void CH395SetTCPMss(unsigned short tcpmss);

//void CH395SetSocketRecvBuf(unsigned char sockindex,unsigned char startblk,unsigned char blknum);

//void CH395SetSocketSendBuf(unsigned char sockindex,unsigned char startblk,unsigned char blknum);

//void CH395UDPSendTo(unsigned char *buf,unsigned long len,unsigned char *ip,unsigned short port,unsigned char sockindex);

void CH395SetStartPara(unsigned long mdata);

//unsigned short CH395CMDGetGlobIntStatus_ALL(void);

void CH395SetKeepLive(unsigned char sockindex,unsigned char cmd);                                 /* keepalive 使能 */

void CH395KeepLiveCNT(unsigned char cnt);                                                /* keepalive 参数设置 */

void CH395KeepLiveIDLE(unsigned long idle);                                             /* keepalive 参数设置 */

void CH395KeepLiveINTVL(unsigned long intvl);                                           /* keepalive 参数设置 */

//void CH395SetTTLNum(unsigned char sockindex,unsigned char TTLnum);                               /* TTL参数设置 */
 
//void CH395EnablePing(unsigned char enable);                                              /* PING 使能 */

void CH395_LabCham_Vari_Init();
void mStopIfError(unsigned char iError);
void keeplive_set(void);
void InitCH395InfParam(void);
void InitSocketParam(void);
unsigned char  CH395Init(void);
void CH395SocketInitOpen(void);
void CH395SocketInterrupt(unsigned char sockindex);
void CH395GlobalInterrupt(void);
void Set_LabCMD_format(float ftemp);
void Set_LabChamber_Temp(float set_temp);
void Set_LabChamber_Off(void);
void Request_LabChamber_State(void);
void Acqurie_LabCham_ActualTemp(float *pData);
void Init_CH395_LabCham_Connect(void);
void SendCMDToLabChamber(unsigned char *cmdbuf);

#endif
/**************************** endfile *************************************/
