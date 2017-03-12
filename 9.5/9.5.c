#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit a0=P0^0;
uint a=0;
void main()
{   TMOD=0x20;//设置定时器1为工作方式2
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	REN=1;
	SM0=0;
	SM1=1;
	EA=1;
	ES=1;
          while(1)
		{
		
		
			if(a==1)
				{
SBUF='1';
while(!TI);	
					TI=0;
                a0=1;
					}

          }
}

void ser() interrupt 4
{
	RI=0;
	if(SBUF==0x01)
{

a=1;


}
	
	
}




