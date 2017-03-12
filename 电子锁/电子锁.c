#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int 
sbit beep=P2^3; 
sbit suo=P2^4;
sbit dula=P2^6;
sbit wela=P2^7;
sbit led=P1^0;
uchar enable,flag,cover,fan,tmp,num; 
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};             
void delay(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
void adisplay(uchar wei)
{
        dula=1;
		P0=0x40;
		dula=0;
		P0=0xff;
		wela=1;
		P0=wei;
		wela=0;
		delay(1);

//       	dula=1;
//		P0=table[bb];
//		dula=0;
//		P0=0xff;
//		wela=1;
//		P0=0xfd;
//		wela=0;
//		delay(1);
//
//       dula=1;
//		P0=table[cc];
//		dula=0;
//		P0=0xff;
//		wela=1;
//		P0=0xfb;
//		wela=0;
//		delay(1);
}
void closedisplay()
{	  	
	  
        dula=1;
		P0=0x39;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfe;
		wela=0;
		delay(1);

       	dula=1;
		P0=0x38;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfd;
		wela=0;
		delay(1);

       	dula=1;
		P0=0x3F;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfb;
		wela=0;
		delay(1);

		dula=1;
		P0=0x6D;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xf7;
		wela=0;
		delay(1);

		dula=1;
		P0=0x79;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xef;
		wela=0;
		delay(1);
	}
void opendisplay()
	{
		dula=1;
		P0=0x3F;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfe;
		wela=0;
		delay(1);

       	dula=1;
		P0=0x73;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfd;
		wela=0;
		delay(1);

       	dula=1;
		P0=0x79;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfb;
		wela=0;
		delay(1);

		dula=1;
		P0=0x37;
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xf7;
		wela=0;
		delay(1);
	



}

uint keyscan()
{
	uchar temp,num,count;
	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			temp=P3;
		switch(temp)
		{
			case 0xee:num=0;
				break;
			case 0xde:num=1;
				break;
			case 0xbe:num=2;
				break;
			case 0x7e:num=3;
				break;
		}
		while(temp!=0xf0)
		{
			temp=P3;
			temp=temp&0xf0;
		}
		}
	}

	P3=0xfd;
	temp=P3;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			temp=P3;
		switch(temp)
		{
			case 0xed:num=4;
				break;
			case 0xdd:num=5;
				break;
			case 0xbd:num=6;
				break;
			case 0x7d:num=7;
				break;
		}
		while(temp!=0xf0)
		{
			temp=P3;
			temp=temp&0xf0;
		}
		}
	}


	P3=0xfb;
	temp=P3;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			temp=P3;
		switch(temp)
		{
			case 0xeb:num=8;
				break;
			case 0xdb:num=9;
				break;
			case 0xbb:num=10;
				break;
			case 0x7b:num=11;
				break;
		}
		while(temp!=0xf0)
		{
			temp=P3;
			temp=temp&0xf0;
		}
		}
	}


	P3=0xf7;
	temp=P3;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			temp=P3;
		switch(temp)
		{
			case 0xe7:num=12;
				break;
			case 0xd7:num=13;
				break;
			case 0xb7:num=14;
				break;
			case 0x77:num=15;
				break;
		}
		while(temp!=0xf0)
		{
			temp=P3;
			temp=temp&0xf0;
		}
		}
	}
return num;
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
//void charge()
//{
//  	 while(1)
//	 {
//	  	if((enable==1)&(flag==2)&(temp==3)&(cover==4))
//		{
//		   suo=0;
//		   beep=0;
//		
//		
//		  enable=0;
//		  flag=0;
//		  temp=0;
//		  cover=0;
//		
//		}
//			
//		if(fan==4)
//		{
//				suo=1;
//				beep=1;
//			
//		}
//	}
//}
//void keycharge()
//{
//	uchar first,second,third,furth;
//
//		switch(keyscan())
//		{
//		  case 1:display(0xfe);first=5;break;
//		  case 2:display(0xfc);second=6;break;
//		  case 3:display(0xf8);third=7;break;
//		  case 4:display(0xf0);furth=8;break;
//
//		}
//
//
//}

void main()
{		uchar first,second,third,furth;
	InitUart();
	 num=5;
	  
	while(1)
	{	
		 if(num==5)
		 closedisplay();
		 if(num==6)
		 opendisplay();
		if(enable==1)
		{
			suo=0;
			enable=0;
			num=6;
		}
			
		if(enable==2)
		{
			suo=1;
			num=5;	
		}
		switch(keyscan())
		{
		  case 8:first=5;break;
		  case 9:second=6;break;
		  case 10:third=7;break;
		  case 11:furth=8;break;

		}
		if((first==5)&(second==6)&(third==7)&(furth==8))
		{
			num=6;
		  	suo=0;
			first=0;
		}
		

		 //display();
			
	}
}

void inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF=='*')//打开窗帘
		{
			enable=1;
		}
		if(SBUF=='&')//关闭窗帘
		{
			enable=2;
		}
	
	
	}
}