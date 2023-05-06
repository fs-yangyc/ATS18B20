
/*CH395оƬ����ӿ��ļ�,�����ӳ����ⲿ����*/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__

/* TCPģʽ��0Ϊ�ͻ��ˣ�1Ϊ������ */
#define TCP_CLIENT_MODE   0
#define TCP_SERVER_MODE   1

/* ����ʱ�����*/
#define DEF_KEEP_LIVE_IDLE                           15000            /* ����ʱ�� */
#define DEF_KEEP_LIVE_PERIOD                         15000            /* ���Ϊ15�룬����һ��KEEPLIVE���ݰ� */                  
#define DEF_KEEP_LIVE_CNT                            200              /* ���Դ���  */

void CH395CMDReset(void);                                            /* ��λ */

//void CH395CMDSleep(void);                                            /* ˯�� */

//unsigned char CH395CMDGetVer(void);                                          /* ��ȡоƬ���̼��汾�� */

unsigned char CH395CMDCheckExist(unsigned char testdata);                            /* �������� */

//void CH395CMDSetPHY(unsigned char phystat);                                  /* ����PHY״̬  */

unsigned char CH395CMDGetPHYStatus(void);                                    /* ��ȡPHY״̬ */

unsigned char CH395CMDGetGlobIntStatus(void);                                /* ��ȡCH395ȫ���ж�״̬ */

unsigned char CH395CMDInitCH395(void);                                       /* ��ʼ��CH395 */

//void CH395CMDSetUartBaudRate(unsigned long baudrate);                       /* ���ò����� */

unsigned char CH395GetCmdStatus(void);                                       /* ��ȡ����ִ��״̬ */

void CH395CMDSetIPAddr(unsigned char *ipaddr);                               /* ����CH395��IP��ַ */

void CH395CMDSetGWIPAddr(unsigned char *gwipaddr);                           /* ����CH395������IP��ַ */

void CH395CMDSetMASKAddr(unsigned char *maskaddr);                           /* ������������ */

void CH395CMDSetMACAddr(unsigned char *amcaddr);                             /* ����CH395��MAC��ַ */

//void CH395CMDGetMACAddr(unsigned char *amcaddr);                             /* ��ȡMAC��ַ */

//void CH395CMDSetMACFilt(unsigned char filtype,unsigned long table0,unsigned long table1);  /* ����CH395��MAC���� */

void CH395CMDGetUnreachIPPT(unsigned char *list);                            /* ��ȡ���ɴ��ַ�Լ��˿� */

//void CH395CMDGetRemoteIPP(unsigned char sockindex,unsigned char *list);              /* ��ȡԶ��IP�Ͷ˿ڣ�һ����TCP Server��ʹ�� */

void CH395SetSocketDesIP(unsigned char sockindex,unsigned char *ipaddr);             /* ����scoket n��Ŀ��IP��ַ */

void CH395SetSocketProtType(unsigned char sockindex,unsigned char prottype);         /* ����socket n��Э������ */

void CH395SetSocketDesPort(unsigned char sockindex,unsigned short desprot);          /* ����socket n��Ŀ�Ķ˿� */

void CH395SetSocketSourPort(unsigned char sockindex,unsigned short surprot);         /* ����socket n��Դ�˿� */

//void CH395SetSocketIPRAWProto(unsigned char sockindex,unsigned char prototype);      /* ��IPRAWģʽ�£�����socket n��IP��Э���ֶ� */

//void CH395SetRecvThresLen(unsigned char sockindex,unsigned short len);               /* ����socket n�Ľ����жϷ�ֵ */

void CH395SendData(unsigned char sockindex,unsigned char *databuf,unsigned short len);       /* ��socket n�ķ��ͻ�����д���� */

unsigned short CH395GetRecvLength(unsigned char sockindex);                          /* ��ȡsocket n�Ľ��ճ��� */

//void CH395ClearRecvBuf(unsigned char sockindex);                             /* ���socket n�Ľ��ջ����� */

void CH395GetRecvData(unsigned char sockindex,unsigned short len,unsigned char *pbuf);       /* ��ȡ�������� */

//void CH395CMDSetRetryCount(unsigned char count);                             /* ����������Դ��� */

//void CH395CMDSetRetryPeriod(unsigned short period);                          /* ��������������� ��λ ����*/

//void CH395CMDGetSocketStatus(unsigned char sockindex,unsigned char *status) ;        /* ��ȡsocket n��״̬ */

unsigned char  CH395OpenSocket(unsigned char sockindex);                             /* ��socket n*/

//unsigned char  CH395CloseSocket(unsigned char sockindex);                             /* �ر�socket n*/

unsigned char CH395TCPConnect(unsigned char sockindex);                               /* TCP���� */

//unsigned char CH395TCPListen(unsigned char sockindex);                                /* TCP���� */

//unsigned char CH395TCPDisconnect(unsigned char sockindex);                            /* TCP�Ͽ����� */

unsigned char CH395GetSocketInt(unsigned char sockindex);                            /* ��ȡsocket n���ж�״̬ */
                                                                     
//unsigned char CH395CRCRet6Bit(unsigned char *mac_addr);                              /* �ಥ��ַCRC32������HASH���� */
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

void CH395SetKeepLive(unsigned char sockindex,unsigned char cmd);                                 /* keepalive ʹ�� */

void CH395KeepLiveCNT(unsigned char cnt);                                                /* keepalive �������� */

void CH395KeepLiveIDLE(unsigned long idle);                                             /* keepalive �������� */

void CH395KeepLiveINTVL(unsigned long intvl);                                           /* keepalive �������� */

//void CH395SetTTLNum(unsigned char sockindex,unsigned char TTLnum);                               /* TTL�������� */
 
//void CH395EnablePing(unsigned char enable);                                              /* PING ʹ�� */

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