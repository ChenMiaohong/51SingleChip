#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P2^6;
sbit wela=P2^7;       //�����λѡ�źţ�Q7ΪADC0804_CSƬѡ�ź�
sbit adwr=P3^6;        //AD0804 д�ź�λ
sbit adrd=P3^7; 
sbit fan=P2^1;
uint enable,flag,fanstat;
uint adval;
void delay(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void start_ad()
{

	wela=1;
    P0=0;                       //ѡͨADCS
    adwr=0;			//ADд�루���д��ʲô���У���Ҫ��Ϊ������ADת����
    _nop_();
    adwr=1;
}
uint readdata()
{   
	uint adval;
	P0=0xff;                    //�ر�ADCS
    delay(10);
    wela=0;                     //�ر���ADƬѡ�ź���������������Է�ֹ�ڲ��������ʱʹAD��Ƭѡ�����仯
    delay(200);
     wela=1;                   	//���´���ADƬѡ�ź��������������
     P1=0xff;                   //��ȡP1��֮ǰ�ȸ���дȫ1
     P0=0;                      //ѡͨADCS
     adrd=0;                    //AD��ʹ��
     adval=P1;					//AD���ݶ�ȡ����P1��
     adrd=1;
     P0=0xff;                     //�ر�ADCS
     adwr=0;	 
	return adval;
} 
void Init_Com()
{
	TMOD=0x20;
	SCON=0x50;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
    REN=1;
    SM0=0;
    SM1=1;
	EA=1;
	ES=1;
	
}
void Send(uint ad_data)
{
	uint fdata,value;
	value=ad_data*4;
	SBUF=((value/1000)+48);			
	while(!TI);
	TI=0;
	SBUF=(((value/100)%10)+48);		
	while(!TI);
	TI=0;
	SBUF=(((value/10)%10)+48);		
	while(!TI);
	TI=0;
	SBUF=((value%10)+48);		
	while(!TI);
	TI=0;
}
void main()
{

	Init_Com();
	while(1)
    {       
		start_ad();  
		if(enable==1)
		{
	       Send(readdata());
		   delay(200);
		   enable=0;
		}
		if(enable==2)
		{
			 fan=0; //��������
			 fanstat=30;
		}
		if(enable==3)
		{
			 fan=1;//�رշ���
			 fanstat=40;
			 
		
		}
		if(flag==1)
		{
			if(fanstat==30)
			{
				SBUF='W';			
				while(!TI);
				TI=0;
			
			}
			if(fanstat==40)
			{
				SBUF='X';			
				while(!TI);
				TI=0;
			
			}
		   flag=0;
		}
			
	}	
}
void inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF=='C')
		{
			enable=1;
		}
		if(SBUF=='D')
		{
			enable=0;
		}
		if(SBUF=='T')
		{
			enable=2;
		}
		if(SBUF=='U')
		{
			enable=3;
		}
		if(SBUF=='V')
		{
			flag=1;
		}

	}
}
