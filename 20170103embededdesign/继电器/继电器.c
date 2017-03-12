#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit key=P2^1;
bit enable=0;
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
int main(void)
{	uint a=0;
	 Init_Com();
	 enable=0;
     while(1)
    {       
			   
			if(enable==1)
			{
		       
			   key=0;
			   P1=0xff;
			   enable=0;
			}
			if(enable==2)
			{
		       
			   key=1;
			     P1=0;
			   enable=0;
			}
		  
				
		}	 
}

void inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF==0x01)
		{
			enable=1;
		}
		if(SBUF==0x02)
		{
			enable=2;
		}
		if(SBUF==0x03)
		{
			enable=0;
		}
	
	}
}
