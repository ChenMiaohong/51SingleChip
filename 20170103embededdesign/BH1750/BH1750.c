#include  <reg52.h>	
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#define   uchar unsigned char
#define   uint unsigned int	
sbit	  SCL=P1^6;      //IICʱ�����Ŷ���
sbit  	  SDA=P1^7;      //IIC�������Ŷ���
sbit      water=P1^0;
uint enable;
uint temp,flag;
uint waterstat;
#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT   ADDRESS���Žӵ�ʱ��ַΪ0x46���ӵ�Դʱ��ַΪ0xB8
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //�������ݻ�����      	
int     dis_data;                       //����

void delay_nms(unsigned int k);
void InitLcd();
void Init_BH1750(void);

void WriteDataLCM(uchar dataW);
void WriteCommandLCM(uchar CMD,uchar Attribc);
void DisplayOneChar(uchar X,uchar Y,uchar DData);
void conversion(uint temp_data);

void  Single_Write_BH1750(uchar REG_Address);               //����д������
uchar Single_Read_BH1750(uchar REG_Address);                //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_BH1750();                               //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
void Delay5us();
void Delay5ms();
void BH1750_Start();                    //��ʼ�ź�
void BH1750_Stop();                     //ֹͣ�ź�
void BH1750_SendACK(bit ack);           //Ӧ��ACK
bit  BH1750_RecvACK();                  //��ack
void BH1750_SendByte(BYTE dat);         //IIC�����ֽ�д
BYTE BH1750_RecvByte();                 //IIC�����ֽڶ�

//-----------------------------------
void Senddata(uint temp_data)
{
	SBUF=temp_data/10000+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%10000;   //ȡ������
	SBUF=temp_data/1000+48 ;
	while(!TI);
	TI=0;
    temp_data=temp_data%1000;    //ȡ������

    SBUF=temp_data/100+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%100;     //ȡ������
    SBUF=temp_data/10+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%10;      //ȡ������
    SBUF=temp_data+48; 
	while(!TI);
	TI=0;	
}
void uartInit()
{
	TMOD=0x20;
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TR1=1;
	EA=1;
	ES=1;
}
//������ʱ**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}


/***********************************/
		
/***********************************/					

/**************************************
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}

/**************************************
��ʼ�ź�
**************************************/
void BH1750_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void BH1750_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void BH1750_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    BH1750_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
  //  BH1750_SendByte(REG_data);       //�ڲ��Ĵ������ݣ�
    BH1750_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);                   //���ʹ洢��Ԫ��ַ����0��ʼ	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=BH1750_RecvByte();              //�����Ĵ�������
	BH1750_SendACK(1);   
	BH1750_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
*/
//*********************************************************
//
//��������BH1750�ڲ�����
//
//*********************************************************
void Multiple_read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
	 for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 3)
        {

           BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //��ӦACK
       }
   }

    BH1750_Stop();                          //ֹͣ�ź�
    Delay5ms();
}


//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}
//*********************************************************
//������********
//*********************************************************
void main()
{  
   float temp;
   delay_nms(100);	    //��ʱ100ms	
   //InitLcd();           //��ʼ��LCD
   Init_BH1750();       //��ʼ��BH1750
   uartInit();
 
  while(1)              //ѭ��
  { 

    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms(180);              //��ʱ180ms

    Multiple_Read_BH1750();       //�����������ݣ��洢��BUF��

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
    
    temp=(float)dis_data/1.2;

    //conversion(temp);

	if(enable==1)
	{
		Senddata(temp);
		delay_nms(100);
		enable=0;
	}
	if(enable==2)
	{
		 water=0; //����ˮ��
		 waterstat=30;
	}
	if(enable==3)
	{
		 water=1;//�ر�ˮ��
		 waterstat=40;
		 
	
	}
	if(flag==1)
	{
		if( waterstat==30)
		{
			SBUF='P';			
			while(!TI);
			TI=0;
		
		}
		if( waterstat==40)
		{
			SBUF='Q';			
			while(!TI);
			TI=0;
		
		}
	   flag=0;
	}
	
            
  }
} 
void Inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF=='I')
		{
			enable=1;	
		}
		if(SBUF=='J')
		{
			enable=0;
		
		}
		if(SBUF=='M')
		{
			enable=2;	
		}
		if(SBUF=='N')
		{
			enable=3;
		
		}
		if(SBUF=='O')
		{
			flag=1;	
		}
		
	
	}


}
