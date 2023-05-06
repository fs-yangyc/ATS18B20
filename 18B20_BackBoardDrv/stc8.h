//(171009����\WEB_STCISP2\header_file\STC8.H Ϊ���հ汾)

#ifndef __STC8F_H_
#define __STC8F_H_

/////////////////////////////////////////////////
//������ͷ�ļ���,���������ٰ���"REG51.H"
//STC8ϵ�е�Ƭ����-1T8051/8K RAM/64K ROM/12λADC/8��PWM/I2C/SPI/4����
//��ϸ�����ֲ����������STC����: www.STCMCU.com��www.GXWMCU.com����.
//��STC8ϵ�е�Ƭ��ͷ�ļ���STC-ISP V6.85o�������ɵ�����STCISP.COM����STC8-160506�����ֲ�����@2016.07.12;13//11.04

//�ں����⹦�ܼĴ���
sfr ACC         =   0xe0;                                       //�ۼ���
sfr B           =   0xf0;                                       //B�Ĵ���
sfr PSW         =   0xd0;                                       //����״̬�ּĴ���
sbit CY         =   PSW^7;
sbit AC         =   PSW^6;
sbit F0         =   PSW^5;
sbit RS1        =   PSW^4;                                      //-�����Ĵ���ѡ��λ1
sbit RS0        =   PSW^3;                                      //-�����Ĵ���ѡ��λ0
sbit OV         =   PSW^2;
sbit P          =   PSW^0;
sfr SP          =   0x81;                                       //��ջָ��
sfr DPL         =   0x82;                                       //����ָ��(���ֽ�)
sfr DPH         =   0x83;                                       //����ָ��(���ֽ�)
sfr TA          =   0xae;                                       //DPTRʱ����ƼĴ���
sfr DPS         =   0xe3;                                       //DPTRָ��ѡ����
sfr DPL1        =   0xe4;                                       //�ڶ�������ָ��(���ֽ�)
sfr DPH1        =   0xe5;                                       //�ڶ�������ָ��(���ֽ�)


//I/O �����⹦�ܼĴ���
sfr P0          =   0x80;                                       //P0�˿�
sfr P1          =   0x90;                                       //P1�˿�
sfr P2          =   0xa0;                                       //P2�˿�
sfr P3          =   0xb0;                                       //P3�˿�
sfr P4          =   0xc0;                                       //P4�˿�
sfr P5          =   0xc8;                                       //P5�˿�
sfr P6          =   0xe8;                                       //P6�˿�
sfr P7          =   0xf8;                                       //P7�˿�
sfr P0M0        =   0x94;                                       //P0�����üĴ���0
sfr P0M1        =   0x93;                                       //P0�����üĴ���1
sfr P1M0        =   0x92;                                       //P1�����üĴ���0
sfr P1M1        =   0x91;                                       //P1�����üĴ���1
sfr P2M0        =   0x96;                                       //P2�����üĴ���0
sfr P2M1        =   0x95;                                       //P2�����üĴ���1
sfr P3M0        =   0xb2;                                       //P3�����üĴ���0
sfr P3M1        =   0xb1;                                       //P3�����üĴ���1
sfr P4M0        =   0xb4;                                       //P4�����üĴ���0
sfr P4M1        =   0xb3;                                       //P4�����üĴ���1
sfr P5M0        =   0xca;                                       //P5�����üĴ���0
sfr P5M1        =   0xc9;                                       //P5�����üĴ���1
sfr P6M0        =   0xcc;                                       //P6�����üĴ���0
sfr P6M1        =   0xcb;                                       //P6�����üĴ���1
sfr P7M0        =   0xe2;                                       //P7�����üĴ���0
sfr P7M1        =   0xe1;                                       //P7�����üĴ���1

sbit P00        =   P0^0;
sbit P01        =   P0^1;
sbit P02        =   P0^2;
sbit P03        =   P0^3;
sbit P04        =   P0^4;
sbit P05        =   P0^5;
sbit P06        =   P0^6;
sbit P07        =   P0^7;
sbit P10        =   P1^0;
sbit P11        =   P1^1;
sbit P12        =   P1^2;
sbit P13        =   P1^3;
sbit P14        =   P1^4;
sbit P15        =   P1^5;
sbit P16        =   P1^6;
sbit P17        =   P1^7;
sbit P20        =   P2^0;
sbit P21        =   P2^1;
sbit P22        =   P2^2;
sbit P23        =   P2^3;
sbit P24        =   P2^4;
sbit P25        =   P2^5;
sbit P26        =   P2^6;
sbit P27        =   P2^7;
sbit P30        =   P3^0;
sbit P31        =   P3^1;
sbit P32        =   P3^2;
sbit P33        =   P3^3;
sbit P34        =   P3^4;
sbit P35        =   P3^5;
sbit P36        =   P3^6;
sbit P37        =   P3^7;
sbit P40        =   P4^0;
sbit P41        =   P4^1;
sbit P42        =   P4^2;
sbit P43        =   P4^3;
sbit P44        =   P4^4;
sbit P45        =   P4^5;
sbit P46        =   P4^6;
sbit P47        =   P4^7;
sbit P50        =   P5^0;
sbit P51        =   P5^1;
sbit P52        =   P5^2;
sbit P53        =   P5^3;
sbit P54        =   P5^4;
sbit P55        =   P5^5;
sbit P56        =   P5^6;
sbit P57        =   P5^7;
sbit P60        =   P6^0;
sbit P61        =   P6^1;
sbit P62        =   P6^2;
sbit P63        =   P6^3;
sbit P64        =   P6^4;
sbit P65        =   P6^5;
sbit P66        =   P6^6;
sbit P67        =   P6^7;
sbit P70        =   P7^0;
sbit P71        =   P7^1;
sbit P72        =   P7^2;
sbit P73        =   P7^3;
sbit P74        =   P7^4;
sbit P75        =   P7^5;
sbit P76        =   P7^6;
sbit P77        =   P7^7;

//�������⹦�ܼĴ���λ����չRAM����
//������Щ�Ĵ���,���Ƚ�P_SW2��BIT7����Ϊ1,�ſ�������д
#define P0PU        (*(unsigned char volatile xdata *)0xfe10)   //P0������������ƼĴ���
#define P1PU        (*(unsigned char volatile xdata *)0xfe11)   //P1������������ƼĴ���
#define P2PU        (*(unsigned char volatile xdata *)0xfe12)   //P2������������ƼĴ���
#define P3PU        (*(unsigned char volatile xdata *)0xfe13)   //P3������������ƼĴ���
#define P4PU        (*(unsigned char volatile xdata *)0xfe14)   //P4������������ƼĴ���
#define P5PU        (*(unsigned char volatile xdata *)0xfe15)   //P5������������ƼĴ���
#define P6PU        (*(unsigned char volatile xdata *)0xfe16)   //P6������������ƼĴ���
#define P7PU        (*(unsigned char volatile xdata *)0xfe17)   //P7������������ƼĴ���
#define P0NCS       (*(unsigned char volatile xdata *)0xfe18)   //P0��ʩ���ش������ƼĴ���
#define P1NCS       (*(unsigned char volatile xdata *)0xfe19)   //P1��ʩ���ش������ƼĴ���
#define P2NCS       (*(unsigned char volatile xdata *)0xfe1a)   //P2��ʩ���ش������ƼĴ���
#define P3NCS       (*(unsigned char volatile xdata *)0xfe1b)   //P3��ʩ���ش������ƼĴ���
#define P4NCS       (*(unsigned char volatile xdata *)0xfe1c)   //P4��ʩ���ش������ƼĴ���
#define P5NCS       (*(unsigned char volatile xdata *)0xfe1d)   //P5��ʩ���ش������ƼĴ���
#define P6NCS       (*(unsigned char volatile xdata *)0xfe1e)   //P6��ʩ���ش������ƼĴ���
#define P7NCS       (*(unsigned char volatile xdata *)0xfe1f)   //P7��ʩ���ش������ƼĴ���

//ϵͳ�������⹦�ܼĴ���
sfr PCON        =   0x87;                                       //��Դ���ƼĴ���
#define SMOD        0x80                                        //-����1�����ʿ���λ
#define SMOD0       0x40                                        //-֡���������λ
#define LVDF        0x20                                        //-��ѹ����־λ
#define POF         0x10                                        //-�ϵ��־λ
#define GF1         0x08
#define GF0         0x04
#define PD          0x02                                        //-����ģʽ����λ
#define IDL         0x01                                        //-IDLE�����У�ģʽ����λ
sfr AUXR        =   0x8e;                                       //�����Ĵ���
#define T0x12       0x80                                        //-��ʱ��0�ٶȿ���λ
#define T1x12       0x40                                        //��ʱ��1�ٶȿ���λ
#define UART_M0x6   0x20                                        //-����1ģʽ0��ͨѶ�ٶȿ���λ
#define T2R         0x10                                        //-��ʱ��2�����п���λ
#define T2_CT       0x08                                        //-��ʱ��0������ʱ�������������λ
#define T2x12       0x04                                        //-��ʱ��2�ٶȿ���λ
#define EXTRAM      0x02                                        //-��չRAM���ʿ���λ
#define S1ST2       0x01                                        //-����1�����ʷ�����ѡ��λ
sfr AUXR2       =   0x97;                                       //�����Ĵ���2
#define TXLNRX      0x10                                        //-����1�м̹㲥��ʽ����λ
sfr BUS_SPEED   =   0xa1;                                       //�����ٶȿ��ƼĴ���
sfr P_SW1       =   0xa2;                                       //����˿��л��Ĵ���1
sfr P_SW2       =   0xba;                                       //����˿��л��Ĵ���2
#define EAXFR       0x80
sfr VOCTRL      =   0xbb;                                       //��ѹ���ƼĴ���
sfr RSTCFG      =   0xff;                                       //��λ���üĴ���

//�������⹦�ܼĴ���λ����չRAM����
//������Щ�Ĵ���,���Ƚ�P_SW2��BIT7����Ϊ1,�ſ�������д
#define CKSEL       (*(unsigned char volatile xdata *)0xfe00)   //ʱ��ѡ��Ĵ���
#define CKDIV       (*(unsigned char volatile xdata *)0xfe01)   //ʱ�ӷ�Ƶ�Ĵ���(DUP)
#define CLKDIV      (*(unsigned char volatile xdata *)0xfe01)   //ʱ�ӷ�Ƶ�Ĵ���
#define IRC24MCR    (*(unsigned char volatile xdata *)0xfe02)   //�ڲ�24M�������ƼĴ���
#define XOSCCR      (*(unsigned char volatile xdata *)0xfe03)   //�ⲿ������ƼĴ���
#define IRC32KCR    (*(unsigned char volatile xdata *)0xfe04)   //�ڲ�32K�������ƼĴ���

//�ж����⹦�ܼĴ���
sfr IE          =   0xa8;                                       //�ж�����Ĵ���
sbit EA         =   IE^7;                                       //-���ж��������λ
sbit ELVD       =   IE^6;                                       //-��ѹ����ж�����λ
sbit EADC       =   IE^5;                                       //-A/Dת���ж�����λ
sbit ES         =   IE^4;                                       //-���п�1�ж�����λ
sbit ET1        =   IE^3;                                       //-��ʱ/������T1������ж�����λ
sbit EX1        =   IE^2;                                       //-�ⲿ�ж�1�ж�����λ
sbit ET0        =   IE^1;                                       //-��ʱ/������T0������ж�����λ
sbit EX0        =   IE^0;                                       //-�ⲿ�ж�0�ж�����λ
sfr IE2         =   0xaf;                                       //�ж�����Ĵ���2
#define ET4         0x40                                        //-��ʱ/������T4������ж�����λ
#define ET3         0x20                                        //-��ʱ/������T3������ж�����λ
#define ES4         0x10                                        //-���п�4�ж�����λ
#define ES3         0x08                                        //-���п�3�ж�����λ
#define ET2         0x04                                        //��ʱ/������T2������ж�����λ
#define ESPI        0x02                                        //SPI�ж�����λ
#define ES2         0x01                                        //���п�2�ж�����λ
sfr IP          =   0xb8;                                       //�ж����ȼ����ƼĴ���
sbit PPCA       =   IP^7;                                       //-CCP/PCA�ж����ȼ�����λ
sbit PLVD       =   IP^6;                                       //-��ѹ����ж����ȼ�����λ
sbit PADC       =   IP^5;                                       //-ADC�ж����ȼ�����λ
sbit PS         =   IP^4;                                       //-����1�ж����ȼ�����λ
sbit PT1        =   IP^3;                                       //-��ʱ��1�ж����ȼ�����λ
sbit PX1        =   IP^2;                                       //-�ⲿ�ж�1�ж����ȼ�����λ
sbit PT0        =   IP^1;                                       //-��ʱ��0�ж����ȼ�����λ
sbit PX0        =   IP^0;                                       //-�ⲿ�ж�0�ж����ȼ�����λ
sfr IP2         =   0xb5;                                       //�ж����ȼ����ƼĴ���2
#define PI2C        0x40                                        //-I2C�ж����ȼ�����λ
#define PCMP        0x20                                        //-�Ƚ����ж����ȼ�����λ
#define PX4         0x10                                        //-�ⲿ�ж�4�ж����ȼ�����λ
#define PPWMFD      0x08                                        //-ǿ��PWM�쳣����ж����ȼ�����λ
#define PPWM        0x04                                        //-��ǿ��PWM�ж����ȼ�����λ
#define PSPI        0x02                                        //-SPI�ж����ȼ�����λ
#define PS2         0x01                                        //-����2�ж����ȼ�����λ
sfr IPH         =   0xb7;                                       //���ж����ȼ����ƼĴ���
#define PPCAH       0x80                                        //-CCP/PCA�ж����ȼ�����λ
#define PLVDH       0x40                                        //-��ѹ����ж����ȼ�����λ
#define PADCH       0x20                                        //-ADC�ж����ȼ�����λ
#define PSH         0x10                                        //-����1�ж����ȼ�����λ
#define PT1H        0x08                                        //-��ʱ��1�ж����ȼ�����λ
#define PX1H        0x04                                        //-�ⲿ�ж�1�ж����ȼ�����λ
#define PT0H        0x02                                        //-��ʱ��0�ж����ȼ�����λ
#define PX0H        0x01                                        //-�ⲿ�ж�0�ж����ȼ�����λ
sfr IP2H        =   0xb6;                                       //���ж����ȼ����ƼĴ���2
#define PI2CH       0x40                                        //-I2C�ж����ȼ�����λ
#define PCMPH       0x20                                        //-�Ƚ����ж����ȼ�����λ
#define PX4H        0x10                                        //-�ⲿ�ж�4�ж����ȼ�����λ
#define PPWMFDH     0x08                                        //-��ǿ��PWM�쳣����ж����ȼ�����λ
#define PPWMH       0x04                                        //-��ǿ��PWM�ж����ȼ�����λ
#define PSPIH       0x02                                        //-SPI�ж����ȼ�����λ
#define PS2H        0x01                                        //-����2�ж����ȼ�����λ
sfr INTCLKO     =   0x8f;                                       //�ж���ʱ��������ƼĴ���
#define EX4         0x40                                        //-�ⲿ�ж�4�ж�����λ
#define EX3         0x20                                        //-�ⲿ�ж�3�ж�����λ
#define EX2         0x10                                        //-�ⲿ�ж�2�ж�����λ
#define T2CLKO      0x04                                        //-��ʱ��2ʱ���������
#define T1CLKO      0x02                                        //-��ʱ��1ʱ���������
#define T0CLKO      0x01                                        //-��ʱ��0ʱ���������
sfr AUXINTIF    =   0xef;                                       //��չ�ⲿ�жϱ�־�Ĵ���
#define INT4IF      0x40                                        //-�ⲿ�ж�4�ж������־
#define INT3IF      0x20                                        //�ⲿ�ж�3�ж������־
#define INT2IF      0x10                                        //�ⲿ�ж�2�ж������־
#define T4IF        0x04                                        //-��ʱ��4����жϱ�־
#define T3IF        0x02                                        //-��ʱ��3����жϱ�־
#define T2IF        0x01                                        //-��ʱ��2����жϱ�־

//��ʱ�����⹦�ܼĴ���
sfr TCON        =   0x88;                                       //��ʱ�����ƼĴ���
sbit TF1        =   TCON^7;                                     //-T1����жϱ�־
sbit TR1        =   TCON^6;                                     //-��ʱ��T1�����п���λ
sbit TF0        =   TCON^5;                                     //-T0����жϱ�־
sbit TR0        =   TCON^4;                                     //-��ʱ��T0�����п���λ
sbit IE1        =   TCON^3;                                     //-�ⲿ�ж�1����Դ��INT1/P3.3����־
sbit IT1        =   TCON^2;                                     //-�ⲿ�ж�Դ1��������λ
sbit IE0        =   TCON^1;                                     //-�ⲿ�ж�0����Դ��INT0/P3.2����־
sbit IT0        =   TCON^0;                                     //-�ⲿ�ж�Դ0��������λ
sfr TMOD        =   0x89;                                       //��ʱ��ģʽ�Ĵ���
#define T1_GATE     0x80                                        //-���ƶ�ʱ��
#define T1_CT       0x40                                        //-���ƶ�ʱ��1������ʱ���������
#define T1_M1       0x20                                        //��ʱ����ʱ��/������1ģʽѡ��1
#define T1_M0       0x10                                        //��ʱ����ʱ��/������1ģʽѡ��0
#define T0_GATE     0x08                                        //-���ƶ�ʱ��0
#define T0_CT       0x04                                        //-���ƶ�ʱ��0������ʱ���������
#define T0_M1       0x02                                        //��ʱ����ʱ��/������0ģʽѡ��1
#define T0_M0       0x01                                        //��ʱ����ʱ��/������0ģʽѡ��0
sfr TL0         =   0x8a;                                       //��ʱ��0��8λ�Ĵ���
sfr TL1         =   0x8b;                                       //��ʱ��1��8λ�Ĵ���
sfr TH0         =   0x8c;                                       //��ʱ��0��8λ�Ĵ���
sfr TH1         =   0x8d;                                       //��ʱ��1��8λ�Ĵ���
sfr T4T3M       =   0xd1;                                       //��ʱ��4/3���ƼĴ���
#define T4R         0x80                                        //-��ʱ��4�����п���λ
#define T4_CT       0x40                                        //-���ƶ�ʱ��4������ʱ���������
#define T4x12       0x20                                        //-��ʱ��4�ٶȿ���λ
#define T4CLKO      0x10                                        //-��ʱ��4ʱ���������
#define T3R         0x08                                        //-��ʱ��3�����п���λ
#define T3_CT       0x04                                        //-���ƶ�ʱ��3������ʱ���������
#define T3x12       0x02                                        //-��ʱ��3�ٶȿ���λ
#define T3CLKO      0x01                                        //-��ʱ��3ʱ���������
sfr T4H         =   0xd2;                                       //��ʱ��4���ֽ�
sfr T4L         =   0xd3;                                       //��ʱ��4���ֽ�
sfr T3H         =   0xd4;                                       //��ʱ��3���ֽ�
sfr T3L         =   0xd5;                                       //��ʱ��3���ֽ�
sfr T2H         =   0xd6;                                       //��ʱ��2���ֽ�
sfr T2L         =   0xd7;                                       //��ʱ��2���ֽ�
sfr TH4         =   0xd2;
sfr TL4         =   0xd3;
sfr TH3         =   0xd4;
sfr TL3         =   0xd5;
sfr TH2         =   0xd6;
sfr TL2         =   0xd7;
sfr WKTCL       =   0xaa;                                       //���绽�Ѷ�ʱ�����ֽ�
sfr WKTCH       =   0xab;                                       //���绽�Ѷ�ʱ�����ֽ�
#define WKTEN       0x80                                        //-���绽�Ѷ�ʱ����ʹ�ܿ���λ
sfr WDT_CONTR   =   0xc1;                                       //���Ź����ƼĴ���
#define WDT_FLAG    0x80                                        //-���Ź������־
#define EN_WDT      0x20                                        //-���Ź�ʹ��λ
#define CLR_WDT     0x10                                        //-���Ź���ʱ������
#define IDL_WDT     0x08                                        //-IDLEģʽʱ�Ŀ��Ź�����λ

//���п����⹦�ܼĴ���
sfr SCON        =   0x98;                                       //����1���ƼĴ���
sbit SM0        =   SCON^7;                                     //-����1��ͨ�Ź���ģʽ����0
sbit SM1        =   SCON^6;                                     //-����1��ͨ�Ź���ģʽ����1
sbit SM2        =   SCON^5;                                     //-����ģʽ2��ģʽ3���ͨ�ſ���λ
sbit REN        =   SCON^4;                                     //-����/��ֹ���ڽ��տ���λ
sbit TB8        =   SCON^3;                                     //-������1ʹ��ģʽ2��ģʽ3ʱ��TB8ΪҪ���͵ĵ�9λ����
sbit RB8        =   SCON^2;                                     //-������1ʹ��ģʽ2��ģʽ3ʱ��RB8Ϊ���յ��ĵ�9λ����
sbit TI         =   SCON^1;                                     //-����1�����ж������־λ
sbit RI         =   SCON^0;                                     //-����1�����ж������־λ
sfr SBUF        =   0x99;                                       //����1���ݼĴ���
sfr S2CON       =   0x9a;                                       //����2���ƼĴ���
#define S2SM0       0x80                                        //-����2��ͨ�Ź���ģʽ����λ
#define S2ST4       0x40
#define S2SM2       0x20                                        //-������2��ģʽ1ʱ������ͨ�ſ���λ
#define S2REN       0x10                                        //-����2����/��ֹ���ڽ��տ���λ
#define S2TB8       0x08                                        //-������2ʹ��ģʽ1ʱ��S2TB8ΪҪ���͵ĵ�9λ����
#define S2RB8       0x04                                        //-������2ʹ��ģʽ1ʱ��S2RB8Ϊ���յ��ĵ�9λ����
#define S2TI        0x02                                        //-����2�����ж������־λ
#define S2RI        0x01                                        //-����2�����ж������־λ
sfr S2BUF       =   0x9b;                                       //����2���ݼĴ���
sfr S3CON       =   0xac;                                       //����3���ƼĴ���
#define S3SM0       0x80                                        //-����3��ͨ�Ź���ģʽ����λ
#define S3ST4       0x40
#define S3SM2       0x20                                        //-������3��ģʽ1ʱ������ͨ�ſ���λ
#define S3REN       0x10                                        //-����3����/��ֹ���ڽ��տ���λ
#define S3TB8       0x08                                        //-������3ʹ��ģʽ1ʱ��S3TB8ΪҪ���͵ĵ�9λ����
#define S3RB8       0x04                                        //-������3ʹ��ģʽ1ʱ��S3RB8Ϊ���յ��ĵ�9λ����
#define S3TI        0x02                                        //-����3�����ж������־λ
#define S3RI        0x01                                        //-����3�����ж������־λ
sfr S3BUF       =   0xad;                                       //����3���ݼĴ���
sfr S4CON       =   0x84;                                       //����4���ƼĴ���
#define S4SM0       0x80                                        //-����4��ͨ�Ź���ģʽ����λ
#define S4ST4       0x40                                        //-ѡ�񴮿�4�Ĳ����ʷ�����
#define S4SM2       0x20                                        //-������4��ģʽ1ʱ������ͨ�ſ���λ
#define S4REN       0x10                                        //-����4����/��ֹ���ڽ��տ���λ
#define S4TB8       0x08                                        //-������4ʹ��ģʽ1ʱ��S3TB8ΪҪ���͵ĵ�9λ����
#define S4RB8       0x04                                        //-������4ʹ��ģʽ1ʱ��S3RB8Ϊ���յ��ĵ�9λ����
#define S4TI        0x02                                        //-����4�����ж������־λ
#define S4RI        0x01                                        //-����4�����ж������־λ
sfr S4BUF       =   0x85;                                       //����4���ݼĴ���
sfr SADDR       =   0xa9;                                       //����1�ӻ���ַ�Ĵ���
sfr SADEN       =   0xb9;                                       //����1�ӻ���ַ���μĴ���

//ADC ���⹦�ܼĴ���
sfr ADC_CONTR   =   0xbc;                                       //ADC���ƼĴ���
#define ADC_POWER   0x80                                        //-ADC��Դ����λ
#define ADC_START   0x40                                        //-ADCת����������λ
#define ADC_FLAG    0x20                                        //-ADCת������ж������־
sfr ADC_RES     =   0xbd;                                       //ADCת�������λ�Ĵ���
sfr ADC_RESL    =   0xbe;                                       //ADCת�������λ�Ĵ���
sfr ADCCFG      =   0xde;                                       //ADC���üĴ���
#define ADC_RESFMT  0x20                                        //-ADCת�������ʽ����λ

//SPI ���⹦�ܼĴ���
sfr SPSTAT      =   0xcd;                                       //SPI״̬�Ĵ���
#define SPIF        0x80                                        //-SPI�жϱ�־λ
#define WCOL        0x40                                        //-SPIд��ͻ��־λ
sfr SPCTL       =   0xce;                                       //SPI���ƼĴ���
#define SSIG        0x80                                        //-SS���Ź��ܿ���λ
#define SPEN        0x40                                        //-SPIʹ�ܿ���λ
#define DORD        0x20                                        //-SPI����λ����/���յ�˳��
#define MSTR        0x10                                        //-������/��ģʽѡ��λ
#define CPOL        0x08                                        //-SPIʱ�Ӽ��Կ���
#define CPHA        0x04                                        //-SPIʱ����λ����
sfr SPDAT       =   0xcf;                                       //���ݼĴ���

//IAP/ISP ���⹦�ܼĴ���
sfr IAP_DATA    =   0xc2;                                       //IAP���ݼĴ���
sfr IAP_ADDRH   =   0xc3;                                       //IAP�ߵ�ַ�Ĵ���
sfr IAP_ADDRL   =   0xc4;                                       //IAP�͵�ַ�Ĵ���
sfr IAP_CMD     =   0xc5;                                       //IAP����Ĵ���
#define IAP_IDL     0x00                                        //IAP_�ղ���
#define IAP_READ    0x01                                        //IAP_��
#define IAP_WRITE   0x02                                        //IAP_д
#define IAP_ERASE   0x03                                        //IAP_����
sfr IAP_TRIG    =   0xc6;                                       //IAP�����Ĵ���
sfr IAP_CONTR   =   0xc7;                                       //IAP���ƼĴ���
#define IAPEN       0x80                                        //-EEPROM����ʹ�ܿ���λ
#define SWBS        0x40                                        //-�����λѡ�����λ
#define SWRST       0x20                                        //-�����λ����λ
#define CMD_FAIL    0x10                                        //-EEPROM����ʧ��״̬λ
sfr ISP_DATA    =   0xc2;                                       //ISP���ݼĴ���
sfr ISP_ADDRH   =   0xc3;                                       //ISP�ߵ�ַ�Ĵ���
sfr ISP_ADDRL   =   0xc4;                                       //ISP�͵�ַ�Ĵ���
sfr ISP_CMD     =   0xc5;                                       //ISP����Ĵ���
sfr ISP_TRIG    =   0xc6;                                       //ISP�����Ĵ���
sfr ISP_CONTR   =   0xc7;                                       //ISP���ƼĴ���

//�Ƚ������⹦�ܼĴ���
sfr CMPCR1      =   0xe6;                                       //�Ƚ������ƼĴ���1
#define CMPEN       0x80                                        //-�Ƚ���ģ��ʹ��λ
#define CMPIF       0x40                                        //-�Ƚ����жϱ�־λ
#define PIE         0x20                                        //-�Ƚ����������ж�ʹ��λ
#define NIE         0x10                                        //-�Ƚ����½����ж�ʹ��λ
#define PIS         0x08                                        //-�Ƚ���������ѡ��λ
#define NIS         0x04                                        //-�Ƚ����ĸ���ѡ��λ
#define CMPOE       0x02                                        //-�Ƚ�������������λ
#define CMPRES      0x01                                        //-�Ƚ����ıȽϽ��
sfr CMPCR2      =   0xe7;                                       //�Ƚ������ƼĴ���2
#define INVCMPO     0x80                                        //-�Ƚ�������������
#define DISFLT      0x40                                        //-ģ���˲����ܿ���

//PCA/PWM ���⹦�ܼĴ���
sfr CCON        =   0xd8;                                       //PCA���ƼĴ���
sbit CF         =   CCON^7;                                     //-PCA����������жϱ�־
sbit CR         =   CCON^6;                                     //-PCA�������������λ
sbit CCF3       =   CCON^3;                                     //-PCAģ��3�ж������־
sbit CCF2       =   CCON^2;                                     //-PCAģ��2�ж������־
sbit CCF1       =   CCON^1;                                     //-PCAģ��1�ж������־
sbit CCF0       =   CCON^0;                                     //-PCAģ��0�ж������־
sfr CMOD        =   0xd9;                                       //PCAģʽ�Ĵ���
#define CIDL        0x80                                        //-����ģʽ���Ƿ�ֹͣPCA����
#define ECF         0x01                                        //PCA����������ж�����λ
sfr CL          =   0xe9;                                       //PCA���������ֽ�
sfr CH          =   0xf9;                                       //PCA���������ֽ�
sfr CCAPM0      =   0xda;                                       //PCAģ��0ģʽ���ƼĴ���
#define ECOM0       0x40                                        //����PCAģ��0�ıȽϹ���
#define CCAPP0      0x20                                        //����PCAģ��0���������ز���
#define CCAPN0      0x10                                        //����PCAģ��0�����½��ز���
#define MAT0        0x08                                        //����PCAģ��0��ƥ�书��
#define TOG0        0x04                                        //����PCAģ��0�ĸ��������������
#define PWM0        0x02                                        //����PCAģ��0����������������
#define ECCF0       0x01                                        //����PCAģ��0��ƥ��/�����ж�
sfr CCAPM1      =   0xdb;                                       //PCAģ��1ģʽ���ƼĴ���
#define ECOM1       0x40                                        //����PCAģ��1�ıȽϹ���
#define CCAPP1      0x20                                        //����PCAģ��1���������ز���
#define CCAPN1      0x10                                        //����PCAģ��1�����½��ز���
#define MAT1        0x08                                        //����PCAģ��1��ƥ�书��
#define TOG1        0x04                                        //����PCAģ��1�ĸ��������������
#define PWM1        0x02                                        //����PCAģ��1����������������
#define ECCF1       0x01                                        //����PCAģ��1��ƥ��/�����ж�
sfr CCAPM2      =   0xdc;                                       //PCAģ��2ģʽ���ƼĴ���
#define ECOM2       0x40                                        //����PCAģ��2�ıȽϹ���
#define CCAPP2      0x20                                        //����PCAģ��2���������ز���
#define CCAPN2      0x10                                        //����PCAģ��2�����½��ز���
#define MAT2        0x08                                        //����PCAģ��2��ƥ�书��
#define TOG2        0x04                                        //����PCAģ��2�ĸ��������������
#define PWM2        0x02                                        //����PCAģ��2����������������
#define ECCF2       0x01                                        //����PCAģ��2��ƥ��/�����ж�
sfr CCAPM3      =   0xdd;                                       //PCAģ��3ģʽ���ƼĴ���
#define ECOM3       0x40                                        //-����PCAģ��3�ıȽϹ���
#define CCAPP3      0x20                                        //-����PCAģ��3���������ز���
#define CCAPN3      0x10                                        //-����PCAģ��3�����½��ز���
#define MAT3        0x08                                        //-����PCAģ��3��ƥ�书��
#define TOG3        0x04                                        //-����PCAģ��3�ĸ��������������
#define PWM3        0x02                                        //-����PCAģ��3����������������
#define ECCF3       0x01                                        //-����PCAģ��3��ƥ��/�����ж�
sfr CCAP0L      =   0xea;                                       //PCAģ��0���ֽ�
sfr CCAP1L      =   0xeb;                                       //PCAģ��1���ֽ�
sfr CCAP2L      =   0xec;                                       //PCAģ��2���ֽ�
sfr CCAP3L      =   0xed;                                       //PCAģ��3���ֽ�
sfr CCAP0H      =   0xfa;                                       //PCAģ��0���ֽ�
sfr CCAP1H      =   0xfb;                                       //PCAģ��1���ֽ�
sfr CCAP2H      =   0xfc;                                       //PCAģ��2���ֽ�
sfr CCAP3H      =   0xfd;                                       //PCAģ��3���ֽ�
sfr PCA_PWM0    =   0xf2;                                       //PCA0��PWMģʽ�Ĵ���
sfr PCA_PWM1    =   0xf3;                                       //PCA1��PWMģʽ�Ĵ���
sfr PCA_PWM2    =   0xf4;                                       //PCA2��PWMģʽ�Ĵ���
sfr PCA_PWM3    =   0xf5;                                       //PCA3��PWMģʽ�Ĵ���

//��ǿ��PWM���η��������⹦�ܼĴ���
sfr PWMCFG      =   0xf1;                                       //PWM������
#define CBIF        0x80                                        //-��ǿ��PWM�������ж������־
#define ETADC       0x40                                        //-PWM�Ƿ���ADC����
sfr PWMIF       =   0xf6;                                       //��ǿ��PWM�жϱ�־�Ĵ���
#define C7IF        0x80                                        //��ǿ��PWMͨ��7�ж������־(��Ҫ�������)
#define C6IF        0x40                                        //��ǿ��PWMͨ��6�ж������־(��Ҫ�������)
#define C5IF        0x20                                        //��ǿ��PWMͨ��5�ж������־(��Ҫ�������)
#define C4IF        0x10                                        //��ǿ��PWMͨ��4�ж������־(��Ҫ�������)
#define C3IF        0x08                                        //��ǿ��PWMͨ��3�ж������־(��Ҫ�������)
#define C2IF        0x04                                        //��ǿ��PWMͨ��2�ж������־(��Ҫ�������)
#define C1IF        0x02                                        //��ǿ��PWMͨ��1�ж������־(��Ҫ�������)
#define C0IF        0x01                                        //��ǿ��PWMͨ��0�ж������־(��Ҫ�������)
sfr PWMFDCR     =   0xf7;                                       //PWM�쳣�����ƼĴ���
#define INVCMP      0x80                                        //-�Ƚ���������쳣�źŴ���
#define INVIO       0x40                                        //-�ⲿ�˿�P3.5�쳣�źŴ���
#define ENFD        0x20                                        //-PWM�ⲿ�쳣������λ
#define FLTFLIO     0x10                                        //-����PWM�ⲿ�쳣ʱ��PWM����ڿ���λ
#define EFDI        0x08                                        //-PWM�쳣����ж�ʹ��λ
#define FDCMP       0x04                                        //-�Ƚ�������쳣���ʹ��λ
#define FDIO        0x02                                        //-P3.5�ڵ�ƽ�쳣���ʹ��λ
#define FDIF        0x01                                        //-��ǿ��PWM�쳣����ж������־
sfr PWMCR       =   0xfe;                                       //PWM���ƼĴ���
#define ENPWM       0x80                                        //-��ǿ��PWM���η�����ʹ��λ
#define ECBI        0x40                                        //-PWM�����������ж�ʹ��λ

//�������⹦�ܼĴ���λ����չRAM����
//������Щ�Ĵ���,���Ƚ�P_SW2��BIT7����Ϊ1,�ſ�������д
#define PWMC        (*(unsigned int  volatile xdata *)0xfff0)   //PWM������
#define PWMCH       (*(unsigned char volatile xdata *)0xfff0)   //PWM���������ֽ�
#define PWMCL       (*(unsigned char volatile xdata *)0xfff1)   //PWM���������ֽ�
#define PWMCKS      (*(unsigned char volatile xdata *)0xfff2)   //PWMʱ��ѡ��
#define TADCP       (*(unsigned char volatile xdata *)0xfff3)   //����ADC����ֵ
#define TADCPH      (*(unsigned char volatile xdata *)0xfff3)   //����ADC����ֵ���ֽ�
#define TADCPL      (*(unsigned char volatile xdata *)0xfff4)   //����ADC����ֵ���ֽ�
#define PWM0T1      (*(unsigned int  volatile xdata *)0xff00)   //PWM0T1����ֵ
#define PWM0T1H     (*(unsigned char volatile xdata *)0xff00)   //PWM0T1����ֵ���ֽ�
#define PWM0T1L     (*(unsigned char volatile xdata *)0xff01)   //PWM0T1����ֵ���ֽ�
#define PWM0T2      (*(unsigned int  volatile xdata *)0xff02)   //PWM0T2��ֵ
#define PWM0T2H     (*(unsigned char volatile xdata *)0xff02)   //PWM0T2��ֵ���ֽ�
#define PWM0T2L     (*(unsigned char volatile xdata *)0xff03)   //PWM0T2��ֵ���ֽ�
#define PWM0CR      (*(unsigned char volatile xdata *)0xff04)   //PWM0���ƼĴ���
#define PWM0HLD     (*(unsigned char volatile xdata *)0xff05)   //PWM0��ƽ���ֿ��ƼĴ���
#define PWM1T1      (*(unsigned int  volatile xdata *)0xff10)   //PWM1T1����ֵ
#define PWM1T1H     (*(unsigned char volatile xdata *)0xff10)   //PWM1T1����ֵ���ֽ�
#define PWM1T1L     (*(unsigned char volatile xdata *)0xff11)   //PWM1T1����ֵ���ֽ�
#define PWM1T2      (*(unsigned int  volatile xdata *)0xff12)   //PWM1T2��ֵ
#define PWM1T2H     (*(unsigned char volatile xdata *)0xff12)   //PWM1T2��ֵ���ֽ�
#define PWM1T2L     (*(unsigned char volatile xdata *)0xff13)   //PWM1T2��ֵ���ֽ�
#define PWM1CR      (*(unsigned char volatile xdata *)0xff14)   //PWM1���ƼĴ���
#define PWM1HLD     (*(unsigned char volatile xdata *)0xff15)   //PWM1��ƽ���ֿ��ƼĴ���
#define PWM2T1      (*(unsigned int  volatile xdata *)0xff20)   //PWM2T1����ֵ
#define PWM2T1H     (*(unsigned char volatile xdata *)0xff20)   //PWM2T1����ֵ���ֽ�
#define PWM2T1L     (*(unsigned char volatile xdata *)0xff21)   //PWM2T1����ֵ���ֽ�
#define PWM2T2      (*(unsigned int  volatile xdata *)0xff22)   //PWM2T2��ֵ
#define PWM2T2H     (*(unsigned char volatile xdata *)0xff22)   //PWM2T2��ֵ���ֽ�
#define PWM2T2L     (*(unsigned char volatile xdata *)0xff23)   //PWM2T2��ֵ���ֽ�
#define PWM2CR      (*(unsigned char volatile xdata *)0xff24)   //PWM2���ƼĴ���
#define PWM2HLD     (*(unsigned char volatile xdata *)0xff25)   //PWM2��ƽ���ֿ��ƼĴ���
#define PWM3T1      (*(unsigned int  volatile xdata *)0xff30)   //PWM3T1����ֵ
#define PWM3T1H     (*(unsigned char volatile xdata *)0xff30)   //PWM3T1����ֵ���ֽ�
#define PWM3T1L     (*(unsigned char volatile xdata *)0xff31)   //PWM3T1����ֵ���ֽ�
#define PWM3T2      (*(unsigned int  volatile xdata *)0xff32)   //PWM3T2��ֵ
#define PWM3T2H     (*(unsigned char volatile xdata *)0xff32)   //PWM3T2��ֵ���ֽ�
#define PWM3T2L     (*(unsigned char volatile xdata *)0xff33)   //PWM3T2��ֵ���ֽ�
#define PWM3CR      (*(unsigned char volatile xdata *)0xff34)   //PWM3���ƼĴ���
#define PWM3HLD     (*(unsigned char volatile xdata *)0xff35)   //PWM3��ƽ���ֿ��ƼĴ���
#define PWM4T1      (*(unsigned int  volatile xdata *)0xff40)   //PWM4T1����ֵ
#define PWM4T1H     (*(unsigned char volatile xdata *)0xff40)   //PWM4T1����ֵ���ֽ�
#define PWM4T1L     (*(unsigned char volatile xdata *)0xff41)   //PWM4T1����ֵ���ֽ�
#define PWM4T2      (*(unsigned int  volatile xdata *)0xff42)   //PWM4T2��ֵ
#define PWM4T2H     (*(unsigned char volatile xdata *)0xff42)   //PWM4T2��ֵ���ֽ�
#define PWM4T2L     (*(unsigned char volatile xdata *)0xff43)   //PWM4T2��ֵ���ֽ�
#define PWM4CR      (*(unsigned char volatile xdata *)0xff44)   //PWM4���ƼĴ���
#define PWM4HLD     (*(unsigned char volatile xdata *)0xff45)   //PWM4��ƽ���ֿ��ƼĴ���
#define PWM5T1      (*(unsigned int  volatile xdata *)0xff50)   //PWM5T1����ֵ
#define PWM5T1H     (*(unsigned char volatile xdata *)0xff50)   //PWM5T1����ֵ���ֽ�
#define PWM5T1L     (*(unsigned char volatile xdata *)0xff51)   //PWM5T1����ֵ���ֽ�
#define PWM5T2      (*(unsigned int  volatile xdata *)0xff52)   //PWM5T2��ֵ
#define PWM5T2H     (*(unsigned char volatile xdata *)0xff52)   //PWM5T2��ֵ���ֽ�
#define PWM5T2L     (*(unsigned char volatile xdata *)0xff53)   //PWM5T2��ֵ���ֽ�
#define PWM5CR      (*(unsigned char volatile xdata *)0xff54)   //PWM5���ƼĴ���
#define PWM5HLD     (*(unsigned char volatile xdata *)0xff55)   //PWM5��ƽ���ֿ��ƼĴ���
#define PWM6T1      (*(unsigned int  volatile xdata *)0xff60)   //PWM6T1����ֵ
#define PWM6T1H     (*(unsigned char volatile xdata *)0xff60)   //PWM6T1����ֵ���ֽ�
#define PWM6T1L     (*(unsigned char volatile xdata *)0xff61)   //PWM6T1����ֵ���ֽ�
#define PWM6T2      (*(unsigned int  volatile xdata *)0xff62)   //PWM6T2��ֵ
#define PWM6T2H     (*(unsigned char volatile xdata *)0xff62)   //PWM6T2��ֵ���ֽ�
#define PWM6T2L     (*(unsigned char volatile xdata *)0xff63)   //PWM6T2��ֵ���ֽ�
#define PWM6CR      (*(unsigned char volatile xdata *)0xff64)   //PWM6���ƼĴ���
#define PWM6HLD     (*(unsigned char volatile xdata *)0xff65)   //PWM6��ƽ���ֿ��ƼĴ���
#define PWM7T1      (*(unsigned int  volatile xdata *)0xff70)   //PWM7T1����ֵ
#define PWM7T1H     (*(unsigned char volatile xdata *)0xff70)   //PWM7T1����ֵ���ֽ�
#define PWM7T1L     (*(unsigned char volatile xdata *)0xff71)   //PWM7T1����ֵ���ֽ�
#define PWM7T2      (*(unsigned int  volatile xdata *)0xff72)   //PWM7T2��ֵ
#define PWM7T2H     (*(unsigned char volatile xdata *)0xff72)   //PWM7T2��ֵ���ֽ�
#define PWM7T2L     (*(unsigned char volatile xdata *)0xff73)   //PWM7T2��ֵ���ֽ�
#define PWM7CR      (*(unsigned char volatile xdata *)0xff74)   //PWM7���ƼĴ���
#define PWM7HLD     (*(unsigned char volatile xdata *)0xff75)   //PWM7��ƽ���ֿ��ƼĴ���

//I2C���⹦�ܼĴ���
//�������⹦�ܼĴ���λ����չRAM����
//������Щ�Ĵ���,���Ƚ�P_SW2��BIT7����Ϊ1,�ſ�������д
#define I2CCFG      (*(unsigned char volatile xdata *)0xfe80)   //I2C���üĴ���
#define ENI2C       0x80                                        //-I2C����ʹ�ܿ���λ
#define MSSL        0x40                                        //-I2C����ģʽѡ��λ
#define I2CMSCR     (*(unsigned char volatile xdata *)0xfe81)   //I2C�������ƼĴ���
#define EMSI        0x80                                        //-I2C����ģʽ�ж�ʹ�ܿ���λ
#define I2CMSST     (*(unsigned char volatile xdata *)0xfe82)   //I2C����״̬�Ĵ���
#define MSBUSY      0x80                                        //-����ģʽʱI2C������״̬λ��ֻ��λ��
#define MSIF        0x40                                        //-����ģʽ���ж�����λ���жϱ�־λ��
#define MSACKI      0x02                                        //-����ģʽʱ�����͡�011�����I2CMSCR��MSCMDλ�������յ���ACK����
#define MSACKO      0x01                                        //-����ģʽʱ��׼����Ҫ���ͳ�ȥ��ACK�ź�
#define I2CSLCR     (*(unsigned char volatile xdata *)0xfe83)   //I2C�ӻ����ƼĴ���
#define ESTAI       0x40                                        //-I2C�ӻ�����START�¼��ж�����λ
#define ERXI        0x20                                        //-I2C�ӻ�������������¼��ж�����λ
#define ETXI        0x10                                        //-I2C�ӻ�������������¼��ж�����λ
#define ESTOI       0x08                                        //-�ӻ�ģʽʱ���յ�STOP�ź��ж�����λ
#define SLRST       0x01                                        //-��λ�ӻ�ģʽ
#define I2CSLST     (*(unsigned char volatile xdata *)0xfe84)   //I2C�ӻ�״̬�Ĵ���
#define SLBUSY      0x80                                        //-�ӻ�ģʽʱI2C������״̬λ��ֻ��λ��
#define STAIF       0x40                                        //-�ӻ�ģʽʱ���յ�START�źź���ж�����
#define RXIF        0x20                                        //-�ӻ�ģʽʱ���յ�1�ֽڵ����ݺ���ж���
#define TXIF        0x10                                        //-�ӻ�ģʽʱ�������1�ֽڵ����ݺ���ж�
#define STOIF       0x08                                        //-�ӻ�ģʽʱ���յ�STOP�źź���ж�����
#define TXING       0x04
#define SLACKI      0x02                                        //-�ӻ�ģʽʱ�����յ���ACK����
#define SLACKO      0x01                                        //-�ӻ�ģʽʱ��׼����Ҫ���ͳ�ȥ��ACK�ź�
#define I2CSLADR    (*(unsigned char volatile xdata *)0xfe85)   //I2C�ӻ���ַ�Ĵ���
#define I2CTXD      (*(unsigned char volatile xdata *)0xfe86)   //I2C��123E-005
