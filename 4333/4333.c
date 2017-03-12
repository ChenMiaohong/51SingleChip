#include<reg52.h>
#define uint unsigned int
#define uchar unsigned char
uchar aa,temp,bai,shi,ge;
sbit dula=P2^6;
sbit wela=P2^7;
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

void delay(uint z);
void display(uchar bai,uchar shi,uchar ge);
void init();
void main()
{
	/*temp=152;

	bai=temp/100;
	shi=temp%100/10;
	ge=temp%10;*/
	
	init();//初始化子函数
	while(1)
	{
		if(aa==20)
		{
			aa=0;
			temp++;
			if(temp==100)
			{
				temp=0;
			}
			bai=temp/100;
			shi=temp%100/10;
			ge=temp%10;
		}
		display(bai,shi,ge);
	}
}
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
void display(uchar bai,uchar shi,uchar ge)
{
		dula=1;
		P0=table[bai];
		dula=0;//送百位数

		P0=0xff;//消应
		wela=1;
		P0=0xfe;
		wela=0;
		delay(1);
		


		dula=1;
		P0=table[shi];
		dula=0;
		P0=0xff;//消应
		wela=1;
		P0=0xfd;
		wela=0;
		delay(1);


		dula=1;
		P0=table[ge];
		dula=0;
		P0=0xff;//消应
		wela=1;
		P0=0xfb;
		wela=0;
		delay(1);
}
void init()
{
	wela=0;
	dula=0;
	temp=0;
	TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	ET0=1;
	TR0=1;
}
void timer0() interrupt 1
{
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	aa++;
}
