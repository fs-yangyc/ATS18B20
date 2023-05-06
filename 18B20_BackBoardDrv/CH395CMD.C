/******************************************************************************
* Description: CH395芯片命令接口及与LabEvent温箱通讯文件                      
*******************************************************************************/

/* 头文件包含*/
#include "CH395INC.H"
#include "ch395cmd.h"
#include <pc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <DelayTime.h>
#include <SystemInit.h>

/* 常用变量定义 */      
xdata struct CH395_SYS CH395Inf;                 //CH395的配置信息
xdata struct SOCK_INF  ScokInf[1];               //Socket 0 的配置信息 

extern unsigned char xdata print_data[80];       //from cfguo.c,发送到桌面的字符串
unsigned char xdata MyBuffer[50];                //接收数据缓冲区   
unsigned char xdata ChamberCMDData[53]; 			   //发给温箱的字符命令
unsigned char xdata TCP_connect_falg;		         //TCP连接标志，1表示TCP连接，0表示TCP断开
unsigned char xdata TCP_timeout_flag;			       //TCP连接超时标志
unsigned char xdata rev_over_flag;				       //接收数据完成标志
//unsigned char xdata socket_state[2];			     //Socket的状态
unsigned char xdata TCP_timeout_cunt;			       //TCP超时次数
unsigned long xdata func_para;					         //CH395功能参数

/* CH395相关定义 */
//unsigned char code CH395MACAddr[6] = {0x02,0x03,0x04,0x05,0x06,0x07}; /* CH395MAC地址 */
unsigned char code CH395IPAddr[4]   = {192,168,1,100};                    /* CH395IP地址 */
unsigned char code CH395GWIPAddr[4] = {192,168,1,1};                    /* CH395网关 */
unsigned char code CH395IPMask[4]   = {255,255,255,0};                  /* CH395子网掩码 */

/* socket 相关定义*/
unsigned char  code Socket0DesIP[4] = {192,168,1,64};                   /* Socket 0目的IP地址 */
unsigned short code Socket0DesPort  = 2049;                             /* Socket 0目的端口 */
unsigned short code Socket0SourPort = 8000;                             /* Socket 0源端口 */

/*======全局变量初始化===========================================================*/
void CH395_LabCham_Vari_Init()
{	
	TCP_connect_falg = 0;
	TCP_timeout_flag = 0;
	rev_over_flag    = 0;
	TCP_timeout_cunt = 0;
	func_para        = 0x00000004;			   //bit2为1，表示CH395低能耗模式使能
	memset(print_data,'\0',sizeof(print_data));
	memset(ChamberCMDData,'\0',sizeof(ChamberCMDData));
}
/*==============================================================================
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : iError   错误代码
* Output         : None
* Return         : None
==============================================================================*/
void mStopIfError(unsigned char iError)
{
    if (iError == CMD_ERR_SUCCESS) return;              /* 操作成功 */
    sprintf(print_data,"Error: 0x%02x\r\n", (unsigned short)iError);     /* 打印错误 */
	  SendStringDataToPC(print_data);
    while ( 1 ) 
    {
      Delay_MS(500);
    }
}
/*********************************************************************************
* Function Name  : keeplive_set
* Description    : 保活定时器参数设置
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void keeplive_set(void)
{
	CH395SetKeepLive(0,1); //Socket 0 开启KeepLive功能
	CH395KeepLiveCNT(DEF_KEEP_LIVE_CNT);
	CH395KeepLiveIDLE(DEF_KEEP_LIVE_IDLE);
	CH395KeepLiveINTVL(DEF_KEEP_LIVE_PERIOD);
}
/*==============================================================================
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
==============================================================================*/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
//  memcpy(CH395Inf.MacAddr,CH395MACAddr,sizeof(CH395MACAddr));
}

/*==============================================================================
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
==============================================================================*/
void InitSocketParam(void)
{
    memset(&ScokInf[0],0,sizeof(ScokInf[0]));                        /* 将Socket0全部清零*/
    memcpy(ScokInf[0].IPAddr,Socket0DesIP,sizeof(Socket0DesIP));     /* 将目的IP地址写入 */
    ScokInf[0].DesPort = Socket0DesPort;                             /* 目的端口 */
    ScokInf[0].SourPort = Socket0SourPort;                           /* 源端口 */
    ScokInf[0].ProtoType = PROTO_TYPE_TCP;                           /* TCP模式 */
	  ScokInf[0].TcpMode = TCP_CLIENT_MODE;                            /* TCP客户端模式 */
}

/*==============================================================================
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
==============================================================================*/
unsigned char  CH395Init(void)
{
    unsigned char i;
    
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
//	  SendStringDataToPC("Initializing CH395 IP,GWIP,MASK...\r\n");
//  CH395CMDSetMACAddr(CH395Inf.MacAddr);                            /* 设置CH395的MAC地址 */
	  CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* 设置CH395的IP地址 */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* 设置网关地址 */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* 设置子网掩码，默认为255.255.255.0*/   
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

/*==============================================================================
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
==============================================================================*/
void CH395SocketInitOpen(void)
{
    unsigned char i;

    /* socket 0为TCP Client模式 */
	  CH395SetSocketProtType(0,PROTO_TYPE_TCP);                        /* 设置socket 0协议类型 */
    CH395SetSocketDesIP(0,ScokInf[0].IPAddr);                        /* 设置socket 0目的IP地址 */
    CH395SetSocketDesPort(0,ScokInf[0].DesPort);                     /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(0,ScokInf[0].SourPort);                   /* 设置socket 0源端口 */
//	  SendStringDataToPC("Open Socket0...\r\n");
    i = CH395OpenSocket(0);                                          /* 打开socket 0 */
    mStopIfError(i);                                                 /* 检查是否成功 */
//    SendStringDataToPC("TCP Connecting...\r\n");
  	i = CH395TCPConnect(0);                                          /* 开始连接 */
    mStopIfError(i);                                                 /* 检测命令是否执行成功,如果失败，则socket会关闭 */
}

/*==============================================================================
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket 中断,在全局中断中被调用
* Input          : sockindex  Socket索引
* Output         : None
* Return         : None
==============================================================================*/
void CH395SocketInterrupt(unsigned char sockindex)
{
    unsigned char  sock_int_socket;
    unsigned char  i;
    unsigned short len;

    sock_int_socket = CH395GetSocketInt(sockindex);                  /* 获取socket 的中断状态 */
    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                      /* 发送缓冲区空闲，可以继续写入要发送的数据 */
    {
			/********2021.05.29屏蔽显示********/
//			  SendStringDataToPC("Sending Buffer is Empty!\r\n");
    }
    if(sock_int_socket & SINT_STAT_SEND_OK)                          /* 发送完成中断 */
    {
		/********2021.05.29屏蔽显示********/
//			  SendStringDataToPC("Sending CMD Successful!\r\n");
    }
    if(sock_int_socket & SINT_STAT_RECV)                             /* 接收中断 */
    {
        len = CH395GetRecvLength(sockindex);                         /* 获取当前缓冲区内数据长度 */
        if(len == 0)
				{
					SendStringDataToPC("Socket0 RECV Interrupt,But Data Length is 0!!!\r\n");
					return;
				}
		    memset(MyBuffer,'\0',sizeof(MyBuffer));	
        CH395GetRecvData(sockindex,len,MyBuffer);         /* 读取数据 */
		    if(MyBuffer[len-1]==0x0D)  rev_over_flag=1;
//				sprintf(print_data,"Receiving Data,Lenth = %dBytes,and Data list:\r\n",len);
				/********2021.05.29屏蔽显示********/
//			  SendStringDataToPC(print_data);
//			  SendStringDataToPC(MyBuffer);  
				
   }
   if(sock_int_socket & SINT_STAT_CONNECT)                            /* 连接中断，仅在TCP模式下有效*/
   {
       SendStringDataToPC("Chamber is Connected!\r\n");
	     TCP_connect_falg=1;
//       if(ScokInf[sockindex].TcpMode == TCP_SERVER_MODE)              /* 如果socket 为服务器模式，用户可以获取远端的IP和端口*/
//       {
//           CH395CMDGetRemoteIPP(sockindex,buf);
//           sprintf(print_data,"IP address = %d.%d.%d.%d\r\n",(unsigned short)buf[0],(unsigned short)buf[1],(unsigned short)buf[2],(unsigned short)buf[3]);    
//           SendStringDataToPC(print_data);
//				   tmp = (unsigned short)(buf[5]<<8) + buf[4];
//           sprintf(print_data,"Port = %d\r\n",tmp);
//           SendStringDataToPC(print_data);				 
//       }
   }
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
   {
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断，仅在TCP模式下有效 */
   {
       SendStringDataToPC("Tcp Time out!!!\r\n");
 
/* 产生超时中断表示连接/发送/接收数据超时或者失败，产生超时时CH395芯片内部将会将此    */
/* socket关闭，在某些情况下CH395并不会重试连接，例如远端端口未打开，如果CH395连接，则 */
/* 远端设备可能会发出RST强制将此连接复位，此时CH395仍然会产生超时中断。本程序仅作演示 */
/* 实际应用中不推荐产生产生超时中断后立即连接，可以间隔一定时间内重新打开socket进行连 */
/* 即可。*/        
       if(ScokInf[sockindex].TcpMode == TCP_CLIENT_MODE)             /* 本程序实际只用了一个socket */
       {
           Delay_MS(1000);  
//		       SendStringDataToPC("Open Socket0 Once Again...\r\n");  /* 延时1S后再次重试，没有必要过于频繁连接 */
           i = CH395OpenSocket(sockindex);
           mStopIfError(i);
		       SendStringDataToPC("TCP Connecting Once Again...\r\n");
           CH395TCPConnect(sockindex);                               /* 开始连接 */
           mStopIfError(i);
		       TCP_timeout_cunt++;
		       if(TCP_timeout_cunt>=10) TCP_timeout_flag = 1;
       }
   }
}

/*==============================================================================
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
==============================================================================*/
void CH395GlobalInterrupt(void)
{
    unsigned char  init_status;
    unsigned short i;
    unsigned char  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus();
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
		SendStringDataToPC("Init status : GINT_STAT_UNREACH, the unreach info:\r\n"); /* UDP模式下可能会产生不可达中断 */
        CH395CMDGetUnreachIPPT(buf); 
        SendStringDataToPC(buf);			
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
//			  SendStringDataToPC("GINT_STAT_PHY_CHANGE:\r\n");
        i = CH395CMDGetPHYStatus();                                  /* 获取PHY状态 */
        if(i == PHY_DISCONN)
				{
					SendStringDataToPC("Ethernet Disconnect!!!\r\n");/* 如果是PHY_DISCONN，CH395内部会自动关闭所有的socket*/
//					while(1)
//					{
//						LED_3 = 0;
//						Delay_MS(500);
//						LED_3 = 1;
//						Delay_MS(500);
//					}
				}
				if(i == PHY_100M_FLL)
				{
//					SendStringDataToPC("Ethernet Connect! Communication Mode is: 100M Full duplex.\r\n");
				}
			   
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)                                /* 本程序实际只用了socket0,该中断不会产生 */
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)                                /* 本程序实际只用了socket0,该中断不会产生 */
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)                                /* 本程序实际只用了socket0,该中断不会产生 */
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
}

//==========设置LabEvent温箱命令格式==================================================
void Set_LabCMD_format(float ftemp)
{
  unsigned char j;
  unsigned char Cham_neg_data[5];            //存储温箱负温度数据 

  if(ftemp>=100)
    sprintf(ChamberCMDData,"$01E 0%.1f 01000000000000000000000000000000\r",ftemp);
  else if(ftemp>=10)
    sprintf(ChamberCMDData,"$01E 00%.1f 01000000000000000000000000000000\r",ftemp);
  else if(ftemp>=0)
    sprintf(ChamberCMDData,"$01E 000%.1f 01000000000000000000000000000000\r",ftemp);
  else
  {
    sprintf(ChamberCMDData,"$01E -000.0 01000000000000000000000000000000\r");
    sprintf(Cham_neg_data,"%.1f",ftemp);
	if(ftemp>-10)
	{
	  for(j=1;j<4;j++)
	  {
	    ChamberCMDData[7+j]=Cham_neg_data[j];
	  }
	}
	else
	{
	  for(j=1;j<5;j++)
	  {
	    ChamberCMDData[6+j]=Cham_neg_data[j];
	  }
	}
  }
}
//==========设置LabEvent温箱温度==================================================
void Set_LabChamber_Temp(float set_temp)
{
    unsigned int ij=0,time_cunt = 0;

 	Set_LabCMD_format(set_temp);         //设置命令格式
	SendCMDToLabChamber(ChamberCMDData); //发送命令
	while(1)                             //等待命令执行并返回数据
	{          
    if(!CH395INT) CH395GlobalInterrupt();
		if(rev_over_flag==1)
		{
		  rev_over_flag=0;
			if(MyBuffer[0]=='0')             //返回0表示温箱执行命令成功
			{
		    SendStringDataToPC("Set Temperature Successful!\r\n");
	      break;
			}
		}
		//超时输出
		ij++;					                 //一条语句耗时2个时钟周期
		if(ij==0)  time_cunt++;
		if(time_cunt>=5000)		 
		{		    
		   SendStringDataToPC("Time out!Set Temperature Failed!!!\r\n");
			 while(1);
		}
	}
	Delay_MS(1000);
}
//==========关闭温箱==================================================
void Set_LabChamber_Off(void)
{   
    unsigned int ij=0,time_cunt = 0;

    sprintf(ChamberCMDData,"$01E 0023.0 0000.0 00000000000000000000000000000000\r");
    SendCMDToLabChamber(ChamberCMDData);
    while(1)
    {          
      if(!CH395INT) CH395GlobalInterrupt();
		  if(rev_over_flag==1)
		  {
		    rev_over_flag=0;
			  if(MyBuffer[0]=='0')
			  {
		      SendStringDataToPC("Stop LabEvent Chamber Successful!\r\n");
	        break;
			  }
		  }
		  //超时输出
		  ij++;					     //一条语句耗时2个时钟周期
		  if(ij==0)  time_cunt++;
		  if(time_cunt>=5000)		 
		  {		    
		    SendStringDataToPC("Time out!Stop LabEvent Chamber Failed!!!\r\n");
			  while(1);
		  }
	  }
}
//==========获取温箱状态==================================================
void Request_LabChamber_State(void)
{
  unsigned int ij=0,time_cunt = 0;

	sprintf(ChamberCMDData,"$01I\r");
	SendCMDToLabChamber(ChamberCMDData);
	while(1)
	{          
		if(!CH395INT) CH395GlobalInterrupt();
		if(rev_over_flag==1)
		{
		  rev_over_flag=0;
			/********2021.05.29屏蔽显示********/
//			SendStringDataToPC("Request State Successful!\r\n");
			break;
		}
		//超时输出
		ij++;					     //一条语句耗时2个时钟周期
		if(ij==0)  time_cunt++;
		if(time_cunt>=10000)		 
		{		    
		  SendStringDataToPC("Time out!Request State Failed!!!\r\n");
			while(1);
		}
	}
}
//==========获取温箱当前温度==================================================
void Acqurie_LabCham_ActualTemp(float *pData)
{
   unsigned char i,actual_temp[6];

   Request_LabChamber_State();  //获取温箱状态
   for(i=0;i<6;i++)             //接收数据的第7位到第12位表示的是当前温箱实际温度
   {
	  actual_temp[i]=MyBuffer[7+i];
   }
   pData[0] = atof(actual_temp);
}
//==========初始化CH395以及与LabEvent温箱建立连接==================================================
void Init_CH395_LabCham_Connect(void)
{
	  unsigned char i;
	
		CH395CMDReset(); //CH395硬件复位
		Delay_MS(400);
	
/* 初始化CH395相关变量 */
    InitCH395InfParam();                                             
    i = CH395Init();                                       //初始化CH395芯片   
    mStopIfError(i);                                       //检查初始化是否成功

   //CH395初始化后，会改变PHY的状态并产生PHY中断
    while(1)
    {                                                                 /* 等待以太网连接成功*/
      if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* 查询CH395是否连接 */
      {
        Delay_MS(200);                                            /* 未连接则等待200MS后再次查询 */
      }
      else 
      {
		    CH395GlobalInterrupt();
        break;
      }
    }

/* 初始化Socket相关变量 */                                                                    
    InitSocketParam();                                                
    CH395SocketInitOpen();
	//Socket 0初始化后，如果TCP连接成功，会产生Socket 0中断
	  while(1)
	  {  
	    if(!CH395INT) CH395GlobalInterrupt();
	    if(TCP_connect_falg) break;		//如果TCP正确连接，就退出循环
	    if(TCP_timeout_flag) 		    //连接超时
	    {
		    SendStringDataToPC("Sorry,TCP Connecting Failed!!! Please Check!\r\n");
		    while(1)
		    {
			    Delay_MS(500);
		    }
	   }		  
	}
	Delay_MS(500);
	//保活定时器参数设置
	keeplive_set();   
}

/********************************************************************************
* Function Name  : ChamberSendCMD
* Description    : 向温箱发送命令
*******************************************************************************/
void SendCMDToLabChamber(unsigned char *cmdbuf)
{
    unsigned short cmd_len;

	cmd_len = (unsigned short)strlen(cmdbuf);
	CH395SendData(0,cmdbuf,cmd_len);
}

/********************************************************************************
* Function Name  : CH395CMDReset
* Description    : 复位CH395芯片
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDReset(void)
{
    xWriteCH395Cmd(CMD00_RESET_ALL);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 使CH395进入睡眠状态
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSleep(void)
//{
//    xWriteCH395Cmd(CMD00_ENTER_SLEEP);
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395CMDSleep
* Description    : 获取芯片以及固件版本号，1字节，高四位表示芯片版本，
                   低四位表示固件版本
* Input          : None
* Output         : None
* Return         : 1字节芯片及固件版本号
*******************************************************************************/
//unsigned char CH395CMDGetVer(void)
//{
//    unsigned char i;
//    xWriteCH395Cmd(CMD01_GET_IC_VER);
//    i = xReadCH395Data();
//    xEndCH395Cmd();
//    return i;
//}

/********************************************************************************
* Function Name  : CH395CMDCheckExist
* Description    : 测试命令，用于测试硬件以及接口通讯
* Input          : testdata 1字节测试数据
* Output         : None
* Return         : 硬件OK，返回 testdata按位取反
*******************************************************************************/
unsigned char CH395CMDCheckExist(unsigned char testdata)
{
    unsigned char i;

    xWriteCH395Cmd(CMD11_CHECK_EXIST);
    xWriteCH395Data(testdata);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetPHY
* Description    : 设置PHY，主要设置CH395 PHY为100/10M 或者全双工半双工，CH395默
                    为自动协商。
* Input          : phystat 参考PHY 命令参数/状态
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSetPHY(unsigned char phystat)
//{
//    xWriteCH395Cmd(CMD10_SET_PHY);
//    xWriteCH395Data(phystat);
//    xEndCH395Cmd();
//}

/*******************************************************************************
* Function Name  : CH395CMDGetPHYStatus
* Description    : 获取PHY的状态
* Input          : None
* Output         : None
* Return         : 当前CH395PHY状态，参考PHY参数/状态定义
*******************************************************************************/
unsigned char CH395CMDGetPHYStatus(void)
{
    unsigned char i;

    xWriteCH395Cmd(CMD01_GET_PHY_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断，0x43及以下版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
unsigned char CH395CMDGetGlobIntStatus(void)
{
    unsigned char init_status;

    xWriteCH395Cmd(CMD01_GET_GLOB_INT_STATUS);
    init_status = xReadCH395Data();
    xEndCH395Cmd();
    return  init_status;
}

/********************************************************************************
* Function Name  : CH395CMDInitCH395
* Description    : 初始化CH395芯片。
* Input          : None
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
unsigned char CH395CMDInitCH395(void)
{
    unsigned char i = 0;
    unsigned char s = 0;

    xWriteCH395Cmd(CMD0W_INIT_CH395);
    xEndCH395Cmd();
    while(1)
    {
        Delay_MS(10);                                                /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出,本函数需要500MS以上执行完毕 */
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395CMDSetUartBaudRate
* Description    : 设置CH395串口波特率，仅在串口模式下有效
* Input          : baudrate 串口波特率
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSetUartBaudRate(unsigned long baudrate)
//{
//	  unsigned char i;
//    xWriteCH395Cmd(CMD31_SET_BAUDRATE);
//    xWriteCH395Data((unsigned char)baudrate);
//    xWriteCH395Data((unsigned char)((unsigned short)baudrate >> 8));
//    xWriteCH395Data((unsigned char)(baudrate >> 16));
//	  i = xReadCH395Data();
//    xEndCH395Cmd();
//}

/*******************************************************************************
* Function Name  : CH395GetCmdStatus
* Description    : 获取命令执行状态，某些命令需要等待命令执行结果
* Input          : None
* Output         : None
* Return         : 返回上一条命令执行状态
*******************************************************************************/
unsigned char CH395GetCmdStatus(void)
{
    unsigned char i;

    xWriteCH395Cmd(CMD01_GET_CMD_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetIPAddr
* Description    : 设置CH395的IP地址
* Input          : ipaddr 指IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetIPAddr(unsigned char *ipaddr)
{
    unsigned char i;

    xWriteCH395Cmd(CMD40_SET_IP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetGWIPAddr
* Description    : 设置CH395的网关IP地址
* Input          : ipaddr 指向网关IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetGWIPAddr(unsigned char *gwipaddr)
{
    unsigned char i;

    xWriteCH395Cmd(CMD40_SET_GWIP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*gwipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMASKAddr
* Description    : 设置CH395的子网掩码，默认为255.255.255.0
* Input          : maskaddr 指子网掩码地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMASKAddr(unsigned char *maskaddr)
{
    unsigned char i;

    xWriteCH395Cmd(CMD40_SET_MASK_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*maskaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMACAddr
* Description    : 设置CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSetMACAddr(unsigned char *amcaddr)
//{
//    unsigned char i;

//    xWriteCH395Cmd(CMD60_SET_MAC_ADDR);
//    for(i = 0; i < 6;i++)xWriteCH395Data(*amcaddr++);
//    xEndCH395Cmd();
//    Delay_MS(100); 
//}

/********************************************************************************
* Function Name  : CH395CMDGetMACAddr
* Description    : 获取CH395的MAC地址。
* Input          : amcaddr MAC地址指针
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDGetMACAddr(unsigned char *amcaddr)
//{
//    unsigned char i;

//    xWriteCH395Cmd(CMD06_GET_MAC_ADDR);
//    for(i = 0; i < 6;i++)*amcaddr++ = xReadCH395Data();
//    xEndCH395Cmd();
// }

/*******************************************************************************
* Function Name  : CH395CMDSetMACFilt
* Description    : 设置MAC过滤。
* Input          : filtype 参考 MAC过滤
                   table0 Hash0
                   table1 Hash1
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSetMACFilt(unsigned char filtype,unsigned long table0,unsigned long table1)
//{
//    xWriteCH395Cmd(CMD90_SET_MAC_FILT);
//    xWriteCH395Data(filtype);
//    xWriteCH395Data((unsigned char)table0);
//    xWriteCH395Data((unsigned char)((unsigned short)table0 >> 8));
//    xWriteCH395Data((unsigned char)(table0 >> 16));
//    xWriteCH395Data((unsigned char)(table0 >> 24));

//    xWriteCH395Data((unsigned char)table1);
//    xWriteCH395Data((unsigned char)((unsigned short)table1 >> 8));
//    xWriteCH395Data((unsigned char)(table1 >> 16));
//    xWriteCH395Data((unsigned char)(table1 >> 24));
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395CMDGetUnreachIPPT
* Description    : 获取不可达信息 (IP,Port,Protocol Type)
* Input          : list 保存获取到的不可达
                        第1个字节为不可达代码，请参考 不可达代码(CH395INC.H)
                        第2个字节为IP包协议类型
                        第3-4字节为端口号
                        第4-8字节为IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetUnreachIPPT(unsigned char *list)
{
    unsigned char i;

    xWriteCH395Cmd(CMD08_GET_UNREACH_IPPORT);
    for(i = 0; i < 8; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetRemoteIPP
* Description    : 获取远端的IP和端口地址，一般在TCP Server模式下使用
* Input          : sockindex Socket索引
                   list 保存IP和端口
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDGetRemoteIPP(unsigned char sockindex,unsigned char *list)
//{
//    unsigned char i;

//    xWriteCH395Cmd(CMD06_GET_REMOT_IPP_SN);
//    xWriteCH395Data(sockindex);
//    for(i = 0; i < 6; i++)
//    {
//        *list++ = xReadCH395Data();
//    }   
//    xEndCH395Cmd();
//}

/*******************************************************************************
* Function Name  : CH395SetSocketDesIP
* Description    : 设置socket n的目的IP地址
* Input          : sockindex Socket索引
                   ipaddr 指向IP地址
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesIP(unsigned char sockindex,unsigned char *ipaddr)
{
    xWriteCH395Cmd(CMD50_SET_IP_ADDR_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketProtType
* Description    : 设置socket 的协议类型
* Input          : sockindex Socket索引
                   prottype 协议类型，请参考 socket协议类型定义(CH395INC.H)
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketProtType(unsigned char sockindex,unsigned char prottype)
{
    xWriteCH395Cmd(CMD20_SET_PROTO_TYPE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prottype);
    xEndCH395Cmd();
}

/*******************************************************************************

* Function Name  : CH395SetSocketDesPort
* Description    : 设置socket n的目的端口地址
* Input          : sockindex Socket索引
                   desprot 2字节目的端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesPort(unsigned char sockindex,unsigned short desprot)
{
    xWriteCH395Cmd(CMD30_SET_DES_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((unsigned char)desprot);
    xWriteCH395Data((unsigned char)(desprot >> 8));
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSourPort
* Description    : 设置socket n的源端口地址
* Input          : sockindex Socket索引
                   desprot 2字节源端口
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSourPort(unsigned char sockindex,unsigned short surprot)
{
    xWriteCH395Cmd(CMD30_SET_SOUR_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((unsigned char)surprot);
    xWriteCH395Data((unsigned char)(surprot>>8));
    xEndCH395Cmd();
}

/******************************************************************************
* Function Name  : CH395SetSocketIPRAWProto
* Description    : IP模式下，socket IP包协议字段
* Input          : sockindex Socket索引
                   prototype IPRAW模式1字节协议字段
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395SetSocketIPRAWProto(unsigned char sockindex,unsigned char prototype)
//{
//    xWriteCH395Cmd(CMD20_SET_IPRAW_PRO_SN);
//    xWriteCH395Data(sockindex);
//    xWriteCH395Data(prototype);
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395EnablePing
* Description    : 开启/关闭 PING
* Input          : enable : 1  开启PING
                          ：0  关闭PING
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395EnablePing(unsigned char enable)
//{
//    xWriteCH395Cmd(CMD01_PING_ENABLE);
//    xWriteCH395Data(enable);
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395SendData
* Description    : 向发送缓冲区写数据
* Input          : sockindex Socket索引
                   databuf  数据缓冲区
                   len   长度
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SendData(unsigned char sockindex,unsigned char *databuf,unsigned short len)
{
    unsigned short i;

    xWriteCH395Cmd(CMD30_WRITE_SEND_BUF_SN);
    xWriteCH395Data((unsigned char)sockindex);
    xWriteCH395Data((unsigned char)len);
    xWriteCH395Data((unsigned char)(len>>8));
   
    for(i = 0; i < len; i++)
    {
        xWriteCH395Data(*databuf++);
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 获取接收缓冲区长度
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回接收缓冲区有效长度
*******************************************************************************/
unsigned short CH395GetRecvLength(unsigned char sockindex)
{
    unsigned short i;

    xWriteCH395Cmd(CMD12_GET_RECV_LEN_SN);
    xWriteCH395Data((unsigned char)sockindex);
    i = xReadCH395Data();
    i = (unsigned short)(xReadCH395Data()<<8) + i;
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395ClearRecvBuf
* Description    : 清除接收缓冲区
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395ClearRecvBuf(unsigned char sockindex)
//{
//    xWriteCH395Cmd(CMD10_CLEAR_RECV_BUF_SN);
//    xWriteCH395Data((unsigned char)sockindex);
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : 读取接收缓冲区数据
* Input          : sockindex Socket索引
                   len   长度
                   pbuf  缓冲区
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GetRecvData(unsigned char sockindex,unsigned short len,unsigned char *pbuf)
{
    unsigned short i;
    if(!len)return;
    xWriteCH395Cmd(CMD30_READ_RECV_BUF_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((unsigned char)len);
    xWriteCH395Data((unsigned char)(len>>8));
    Delay_5US();
    for(i = 0; i < len; i++)
    {
       *pbuf = xReadCH395Data();
       pbuf++;
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryCount
* Description    : 设置重试次数
* Input          : count 重试值，最大为20次
* Output         : None
* Return         : None
********************************************************************************/
//void CH395CMDSetRetryCount(unsigned char count)
//{
//    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
//    xWriteCH395Data(count);
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395CMDSetRetryPeriod
* Description    : 设置重试周期
* Input          : period 重试周期单位为毫秒，最大1000ms
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395CMDSetRetryPeriod(unsigned short period)
//{
//    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
//    xWriteCH395Data((unsigned char)period);
//    xWriteCH395Data((unsigned char)(period>>8));
//    xEndCH395Cmd();
//}

/********************************************************************************
* Function Name  : CH395CMDGetSocketStatus
* Description    : 获取socket
* Input          : None
* Output         : socket n的状态信息，第1字节为socket 打开或者关闭
                   第2字节为TCP状态
* Return         : None
*******************************************************************************/
//void CH395CMDGetSocketStatus(unsigned char sockindex,unsigned char *status)
//{
//    xWriteCH395Cmd(CMD12_GET_SOCKET_STATUS_SN);
//    xWriteCH395Data(sockindex);
//    *status++ = xReadCH395Data();
//    *status++ = xReadCH395Data();
//    xEndCH395Cmd();
//}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : 打开socket，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
unsigned char  CH395OpenSocket(unsigned char sockindex)
{
    unsigned char i = 0;
    unsigned char s = 0;
    xWriteCH395Cmd(CMD1W_OPEN_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        Delay_MS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395CloseSocket
* Description    : 关闭socket，
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
//unsigned char  CH395CloseSocket(unsigned char sockindex)
//{
//    unsigned char i = 0;
//    unsigned char s = 0;
//    xWriteCH395Cmd(CMD1W_CLOSE_SOCKET_SN);
//    xWriteCH395Data(sockindex);
//    xEndCH395Cmd();
//    while(1)
//    {
//        Delay_MS(5);                                                 /* 延时查询，建议2MS以上*/
//        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
//        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
//        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
//    }
//    return s;
//}

/********************************************************************************
* Function Name  : CH395TCPConnect
* Description    : TCP连接，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
unsigned char CH395TCPConnect(unsigned char sockindex)
{
    unsigned char i = 0;
    unsigned char s = 0;
    xWriteCH395Cmd(CMD1W_TCP_CONNECT_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        Delay_MS(5);                                                 /* 延时查询，建议2MS以上*/
        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395TCPListen
* Description    : TCP监听，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : 返回执行结果
*******************************************************************************/
//unsigned char CH395TCPListen(unsigned char sockindex)
//{
//    unsigned char i = 0;
//    unsigned char s = 0;
//    xWriteCH395Cmd(CMD1W_TCP_LISTEN_SN);
//    xWriteCH395Data(sockindex);
//    xEndCH395Cmd();
//    while(1)
//    {
//        Delay_MS(5);                                                 /* 延时查询，建议2MS以上*/
//        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
//        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
//        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
//    }
//    return s;
//}

/********************************************************************************
* Function Name  : CH395TCPDisconnect
* Description    : TCP断开，仅在TCP模式下有效，此命令需要等待执行成功
* Input          : sockindex Socket索引
* Output         : None
* Return         : None
*******************************************************************************/
//unsigned char CH395TCPDisconnect(unsigned char sockindex)
//{
//    unsigned char i = 0;
//    unsigned char s = 0;
//    xWriteCH395Cmd(CMD1W_TCP_DISNCONNECT_SN);
//    xWriteCH395Data(sockindex);
//    xEndCH395Cmd();
//    while(1)
//    {
//        Delay_MS(5);                                                 /* 延时查询，建议2MS以上*/
//        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
//        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
//        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
//    }
//    return s;
//}

/*******************************************************************************
* Function Name  : CH395GetSocketInt
* Description    : 获取socket n的中断状态
* Input          : sockindex   socket索引
* Output         : None
* Return         : 中断状态
*******************************************************************************/
unsigned char CH395GetSocketInt(unsigned char sockindex)
{
    unsigned char intstatus;
    xWriteCH395Cmd(CMD11_GET_INT_STATUS_SN);
    xWriteCH395Data(sockindex);
    Delay_5US();
    intstatus = xReadCH395Data();
    xEndCH395Cmd();
    return intstatus;
}

/*******************************************************************************
* Function Name  : CH395CRCRet6Bit
* Description    : 对多播地址进行CRC运算，并取高6位。
* Input          : mac_addr   MAC地址
* Output         : None
* Return         : 返回CRC32的高6位
*******************************************************************************/
//unsigned char CH395CRCRet6Bit(unsigned char *mac_addr)
//{
//    long perByte;
//    long perBit;
//    const unsigned long poly = 0x04C11DB7;
//    unsigned long crc_value = 0xFFFFFFFF;
//    unsigned char c;
//    for ( perByte = 0; perByte < 6; perByte ++ ) 
//    {
//        c = *(mac_addr++);
//        for ( perBit = 0; perBit < 8; perBit++ ) 
//        {
//            crc_value = (crc_value<<1)^((((crc_value>>31)^c)&0x01)?poly:0);
//            c >>= 1;
//        }
//    }
//    crc_value=crc_value>>26;                                      
//    return ((unsigned char)crc_value);
//}

/******************************************************************************
* Function Name  : CH395DHCPEnable
* Description    : 启动/停止DHCP
* Input          : flag   1:启动DHCP;0：停止DHCP
* Output         : None
* Return         : 执行状态
*******************************************************************************/
//unsigned char  CH395DHCPEnable(unsigned char flag)
//{
//    unsigned char i = 0;
//    unsigned char s;
//    xWriteCH395Cmd(CMD10_DHCP_ENABLE);
//    xWriteCH395Data(flag);
//    xEndCH395Cmd();
//    while(1)
//    {
//        Delay_MS(20);
//        s = CH395GetCmdStatus();                                     /* 不能过于频繁查询*/
//        if(s !=CH395_ERR_BUSY)break;                                 /* 如果CH395芯片返回忙状态*/
//        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* 超时退出*/
//    }
//    return s;
//}

/******************************************************************************
* Function Name  : CH395GetDHCPStatus
* Description    : 获取DHCP状态
* Input          : None
* Output         : None
* Return         : DHCP状态，0为成功，其他值表示错误
*******************************************************************************/
//unsigned char CH395GetDHCPStatus(void)
//{
//    unsigned char status;
//    xWriteCH395Cmd(CMD01_GET_DHCP_STATUS);
//    status = xReadCH395Data();
//    xEndCH395Cmd();
//    return status;
//}

/*******************************************************************************
* Function Name  : CH395GetIPInf
* Description    : 获取IP，子网掩码和网关地址
* Input          : None
* Output         : 12个字节的IP,子网掩码和网关地址
* Return         : None
*******************************************************************************/
//void CH395GetIPInf(unsigned char *addr)
//{
//    unsigned char i;
//    xWriteCH395Cmd(CMD014_GET_IP_INF);
//    for(i = 0; i < 20; i++)
//    {
//     *addr++ = xReadCH395Data();
//    }
//    xEndCH395Cmd();
//}

/*******************************************************************************
* Function Name  : CH395WriteGPIOAddr
* Description    : 写GPIO寄存器
* Input          : regadd   寄存器地址
*                ：regval   寄存器值
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395WriteGPIOAddr(unsigned char regadd,unsigned char regval)
//{
//    xWriteCH395Cmd(CMD20_WRITE_GPIO_REG);
//    xWriteCH395Data(regadd);
//    xWriteCH395Data(regval);
//}

/*******************************************************************************
* Function Name  : CH395ReadGPIOAddr
* Description    : 读GPIO寄存器
* Input          : regadd   寄存器地址
* Output         : None
* Return         : 寄存器的值
*******************************************************************************/
//unsigned char CH395ReadGPIOAddr(unsigned char regadd)
//{
//    unsigned char i;
//    xWriteCH395Cmd(CMD10_READ_GPIO_REG);
//    xWriteCH395Data(regadd);
//    Delay_MS(1);
//    i = xReadCH395Data();
//    return i;
//}

/*******************************************************************************
* Function Name  : CH395EEPROMErase
* Description    : 擦除EEPROM
* Input          : None
* Output         : None
* Return         : 执行状态
*******************************************************************************/
//unsigned char CH395EEPROMErase(void)
//{
//    unsigned char i;    
//    xWriteCH395Cmd(CMD00_EEPROM_ERASE);
//    while(1)
//    {
//       Delay_MS(20);
//       i = CH395GetCmdStatus();
//       if(i == CH395_ERR_BUSY)continue;
//       break;
//    }
//    return i;
//}

/*******************************************************************************
* Function Name  : CH395EEPROMWrite
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : 执行状态
*******************************************************************************/
//unsigned char CH395EEPROMWrite(unsigned short eepaddr,unsigned char *buf,unsigned char len)
//{
//    unsigned char i;
//    xWriteCH395Cmd(CMD30_EEPROM_WRITE);
//    xWriteCH395Data((unsigned char)(eepaddr));
//    xWriteCH395Data((unsigned char)(eepaddr >> 8));
//    xWriteCH395Data(len);  
//    while(len--)xWriteCH395Data(*buf++);
//    while(1)
//    {
//       Delay_MS(20);
//       i = CH395GetCmdStatus();
//       if(i == CH395_ERR_BUSY)continue;
//       break;
//    }
//    return i;
//}
  
/*******************************************************************************
* Function Name  : CH395EEPROMRead
* Description    : 写EEPROM
* Input          : eepaddr  EEPROM地址
*                ：buf      缓冲区地址
*                ：len      长度
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395EEPROMRead(unsigned short eepaddr,unsigned char *buf,unsigned char len)
//{
//    xWriteCH395Cmd(CMD30_EEPROM_READ);
//    xWriteCH395Data((unsigned char)(eepaddr));
//    xWriteCH395Data((unsigned char)(eepaddr >> 8));
//    xWriteCH395Data(len);  
//    Delay_MS(1);
//    while(len--)*buf++ = xReadCH395Data();
//}

/*******************************************************************************
* Function Name  : CH395SetTCPMss
* Description    : 设置TCP MSS值
* Input          : tcpmss 
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395SetTCPMss(unsigned short tcpmss)
//{
//    xWriteCH395Cmd(CMD20_SET_TCP_MSS);
//    xWriteCH395Data((unsigned char)(tcpmss));
//    xWriteCH395Data((unsigned char)(tcpmss >> 8));
//}

/*******************************************************************************
* Function Name  : CH395SetSocketRecvBuf
* Description    : 设置Socket接收缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数 ，单位为512字节
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395SetSocketRecvBuf(unsigned char sockindex,unsigned char startblk,unsigned char blknum)
//{
//    xWriteCH395Cmd(CMD30_SET_RECV_BUF);
//    xWriteCH395Data(sockindex);
//    xWriteCH395Data(startblk);
//    xWriteCH395Data(blknum);
//}

/*******************************************************************************
* Function Name  : CH395SetSocketSendBuf
* Description    : 设置Socket发送缓冲区
* Input          : sockindex  socket索引
                 ：startblk   起始地址
                 ：blknum     单位缓冲区个数
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395SetSocketSendBuf(unsigned char sockindex,unsigned char startblk,unsigned char blknum)
//{
//    xWriteCH395Cmd(CMD30_SET_SEND_BUF);
//    xWriteCH395Data(sockindex);
//    xWriteCH395Data(startblk);
//    xWriteCH395Data(blknum);
//}

/*******************************************************************************
* Function Name  : CH395UDPSendTo
* Description    : UDP向指定的IP和端口发送数据
* Input          : buf     : 发送数据缓冲区
                   len     : 发送数据长度
				   ip      : 目标IP
				   port    : 目标端口
				   sockeid : socket索引值
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395UDPSendTo(unsigned char *buf,unsigned long len,unsigned char *ip,unsigned short port,unsigned char sockindex)
//{
//    CH395SetSocketDesIP(sockindex,ip);                            /* 设置socket 0目标IP地址 */         
//    CH395SetSocketDesPort(sockindex,port);
//    CH395SendData(sockindex,buf,len);    
//}

/*******************************************************************************
* Function Name  : CH395SetStartPara
* Description    : 设置CH395启动参数
* Input          : mdata
* Output         : None
* Return         : None
*******************************************************************************
void CH395SetStartPara(unsigned long mdata)
{
    xWriteCH395Cmd(CMD40_SET_FUN_PARA);
    xWriteCH395Data((unsigned char)mdata);
    xWriteCH395Data((unsigned char)((unsigned short)mdata>>8));
    xWriteCH395Data((unsigned char)(mdata >> 16));
    xWriteCH395Data((unsigned char)(mdata >> 24));
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : 获取全局中断状态，收到此命令CH395自动取消中断,0x44及以上版本使用
* Input          : None
* Output         : None
* Return         : 返回当前的全局中断状态
*******************************************************************************/
//unsigned short CH395CMDGetGlobIntStatus_ALL(void)
//{
//		unsigned short init_status;
//		xWriteCH395Cmd(CMD02_GET_GLOB_INT_STATUS_ALL);
//		Delay_5US();
//		init_status = xReadCH395Data();
//		init_status = (unsigned short)(xReadCH395Data()<<8) + init_status;
//		xEndCH395Cmd();
//		return 	init_status;
//}

/*******************************************************************************
* Function Name  : CH395SetKeepLive
* Description    : 设置keepalive功能
* Input          : sockindex Socket号
*                  cmd 0：关闭 1：开启
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetKeepLive(unsigned char sockindex,unsigned char cmd)
{
    xWriteCH395Cmd(CMD20_SET_KEEP_LIVE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(cmd);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveCNT
* Description    : 设置keepalive重试次数
* Input          : cnt 重试次数（）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveCNT(unsigned char cnt)
{
    xWriteCH395Cmd(CMD10_SET_KEEP_LIVE_CNT);
    xWriteCH395Data(cnt);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveIDLE
* Description    : 设置KEEPLIVE空闲
* Input          : idle 空闲时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveIDLE(unsigned long idle)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_IDLE);
    xWriteCH395Data((unsigned char)idle);
    xWriteCH395Data((unsigned char)((unsigned short)idle>>8));
    xWriteCH395Data((unsigned char)(idle >> 16));
    xWriteCH395Data((unsigned char)(idle >> 24));
}

/*******************************************************************************
* Function Name  : CH395KeepLiveINTVL
* Description    : 设置KeepLive间隔时间 
* Input          : intvl 间隔时间（单位：ms）
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveINTVL(unsigned long intvl)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_INTVL);
    xWriteCH395Data((unsigned char)intvl);
    xWriteCH395Data((unsigned char)((unsigned short)intvl>>8));
    xWriteCH395Data((unsigned char)(intvl >> 16));
    xWriteCH395Data((unsigned char)(intvl >> 24));
}

/*******************************************************************************
* Function Name  : CH395SetTTLNum
* Description    : 设置TTL
* Input          : sockindex Socket号
*                  TTLnum:TTL数
* Output         : None
* Return         : None
*******************************************************************************/
//void CH395SetTTLNum(unsigned char sockindex,unsigned char TTLnum)
//{
//    xWriteCH395Cmd(CMD20_SET_TTL);
//    xWriteCH395Data(sockindex);
//    xWriteCH395Data(TTLnum);
//}


