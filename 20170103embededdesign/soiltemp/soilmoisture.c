#include<reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P2^6;
sbit wela=P2^7;       //数码管位选信号，Q7为ADC0804_CS片选信号
sbit adwr=P3^6;        //AD0804 写信号位
sbit adrd=P3^7; 
uint enable,temp,flag;
sbit fan=P2^1;
uint adval,fanstat;
void delay(uint xms)
{
	uint i,j;
	for(i=xms;i>0;i--)
		for(j=110;j>0;j--);
}

void start_ad()
{

	wela=1;
    P0=0;                       //选通ADCS
    adwr=0;			//AD写入（随便写个什么都行，主要是为了启动AD转换）
    _nop_();
    adwr=1;
}
uint readdata()
{   
	uint adval;
	P0=0xff;                    //关闭ADCS
    delay(10);
    wela=0;                     //关闭有AD片选信号锁存器的锁存端以防止在操作数码管时使AD的片选发生变化
    delay(200);
     wela=1;                   	//重新打开有AD片选信号锁存器的锁存端
     P1=0xff;                   //读取P1口之前先给其写全1
     P0=0;                      //选通ADCS
     adrd=0;                    //AD读使能
     adval=P1;					//AD数据读取赋给P1口
     adrd=1;
     P0=0xff;                     //关闭ADCS
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
	value=ad_data*1;
	SBUF=((value/100)+48);		
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

	fan=0;
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
		if(temp==2)
		{
			 fan=1; 
			 fanstat=30;
		}
		if(temp==3)
		{
			 fan=0;
			 fanstat=40;
			 
		
		}
		if(flag==1)
		{
			if(fanstat==30)
			{
				SBUF='-';			
				while(!TI);
				TI=0;
			
			}
			if(fanstat==40)
			{
				SBUF='=';			
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
		if(SBUF=='A')
		{
			enable=1;
		}
		if(SBUF=='B')
		{
			enable=0;
		}
		if(SBUF=='Y')
		{
			temp=2;
		}
		if(SBUF=='Z')
		{
			temp=3;
		}
		 if(SBUF=='+')
		{
			flag=1;
		}
	}
}
