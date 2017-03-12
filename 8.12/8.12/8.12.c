#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uint num,temp;
sbit a1=P2^1;
sbit a2=P2^3;
void wu();
void wufeng();
void beifeng();
void dongbeifeng();
void dongfeng();
void dongnanfeng();
void nanfeng();
void xinanfeng();
void xifeng();
void xibeifeng();
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
num=temp;    
switch(num)
{
case 10:wu();break;
case 9:wufeng();break;
case 1:beifeng();break;
case 2:dongbeifeng();break;
case 3:dongfeng();break;
case 4:dongnanfeng();break;
case 5:nanfeng();break;
case 6:xinanfeng();break;
case 7:xifeng();break;
case 8:xibeifeng();break;
}
         
            }
 
}
void inter() interrupt 4
{
if(RI==1)
{
   RI=0;
  temp=SBUF; 
switch(temp)
{

case 0x20:temp=10;break;
case 0x10:temp=9;break;
case 0x11:temp=1;break;
case 0x12:temp=2;break;
case 0x13:temp=3;break;
case 0x14:temp=4;break;
case 0x15:temp=5;break;
case 0x16:temp=6;break;
case 0x17:temp=7;break;
case 0x18:temp=8;break;
}
}
}
void wu()
{
P1=0x00;
P0=0x00;
a1=1;
a2=1;


}
void wufeng()
{P1=0xFF;
P0=0x00;
a1=0;
a2=0;
}
void beifeng()
{P0=0x7C;
P1=0x83;
a1=0;
a2=0;

}
void dongbeifeng()
{P0=0xF8;
P1=0x07;
a1=0;
a2=0;
}
void dongfeng()
{P0=0xF1;
P1=0x0E;
a1=0;
a2=0;

}
void dongnanfeng()
{ P0=0xE3;
P1=0x1C;
a1=0;
a2=0;
}
void nanfeng()
{P0=0xC7;
P1=0x38;
a1=0;
a2=0;
}
void xinanfeng()
{P0=0x8F;
P1=0x70;
a1=0;
a2=0;

}
void xifeng()
{P0=0x1F;
P1=0xE0;
a1=0;
a2=0;

}
void xibeifeng()
{P0=0x3E;
P1=0xC1;
a1=0;
a2=0;
}
