#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uint num,temp;

void delay(uint z)
{       uint i,j;
        for(i=z;i>0;i--)
        for(j=210;j>0;j--);
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

void main(void)
{
	 Init_Com();
      while(1)
{
if(num==1)
{
P1=0x00;
delay(100);
num=0;
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
			num=1;
		}
		
	}
}

