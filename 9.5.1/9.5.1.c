#include<reg52.h>
unsigned char flag,a;
sbit a0=P1^0;
void main()
{

	TMOD=0x20;//���ö�ʱ��1Ϊ������ʽ2
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
		
			if(flag==1)
				{
					ES=0;
					flag=0;
					SBUF='1';
a0=1;	
					while(!TI);	
					TI=0;
					
					ES=1;
				}
		}
}

void ser() interrupt 4
{
if(RI==1)
{
	RI=0;
if(SBUF==0x01)
{
	flag=1;
}



}

}
