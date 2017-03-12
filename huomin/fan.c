#include<reg52.h>

#define uchar unsigned char
#define uint  unsigned int
 sbit led=P1^4;
 sbit beep=P2^3;
 uint enable,flag = 0;
 uint a, b, c, d, sum;
 unsigned char table[7];
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
void InitUart()
{
    TMOD = 0x20;				// 高4位（定时器1）用于串口
	SCON = 0x50;	
	TH1  = 0xFD;				// 波特率 9600
	TL1  = 0xFD; 
	TR1  = 1;					// 开启定时器1
	ES   = 1; 
	EA   = 1;					// 开启总中断


}
void main()
{	 //uint i;
 	InitUart();
while(1)
		{
					enable = 0;
					SBUF=0x01;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x03;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x00;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x00;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x00;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x01;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x84;//28;	
					while(!TI);	
					TI=0;
					SBUF=0x0A;//28;	
					while(!TI);	
					TI=0;
					//ES=1;
				while(!enable);
	}
}
void in() interrupt 4
{
	if(RI==1)
	{
		RI=0;

		table[flag++] = SBUF;
		if(flag==7){
			flag = 0;
			 a = table[3]/16;
			 b = table[3]%16;
			 c = table[4]/16;
			 d = table[4]%16;
			 sum = a*16*16*16+b*16*16+c*16+d*1;
			SBUF=sum;
			enable=1;
			while(!TI);	
			TI=0;


		}
		
	}
}  