#include<reg52.h>
unsigned char flag,a;
void main()
{

	TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;

	
}

void ser() interrupt 4
{  if(RI==1)
{	RI=0;
	
P1=SBUF;
 
	 }
}
