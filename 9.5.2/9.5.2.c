#include<reg52.h>
#define uchar unsigned char
#define uint unsigned int
uint temp,num;
uint temp1,temp2;
sbit P5=P2^3;
//P0½ÓºìµÆ
//P1½ÓÂÌµÆ
void delay(uint z)
{       uint i,j;
        for(i=z;i>0;i--)
        for(j=210;j>0;j--);
}
void main()
{
	
P5=0;
}

