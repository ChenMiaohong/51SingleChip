/*******************************************************************************
* 实 验 名		 : 步进电机实验
* 使用的IO	     : 电机用P1口,键盘使用P3.0、P3.1、P3.2、P3.3
* 实验效果       : 按下K1键，顺时针转，按下K2键，逆时针转，按下K3键，低速，
*按下K4键，高速。
* 注    意		 ：由于P3.2口跟红外线共用，所以做按键实验时为了不让红外线影响实验
*效果，最好把红外线先取下来。
*******************************************************************************/ 
#include "reg52.h"
#include<intrins.h>

//电机IO
#define MotorData P1 
#define uchar unsigned char
#define uint  unsigned int
//sbit F1 = P1^0;
//sbit F2 = P1^1;
//sbit F3 = P1^2;
//sbit F4 = P1^3;
//按键IO
sbit S1=P3^4;
sbit S2=P3^5;
sbit S3=P3^6;
sbit S4=P3^7;
sbit dula=P2^6;
sbit wela=P2^7;
void delay(uint x);
uchar speed=0;
uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
uchar code tableCCW[8]={0x80,0x40,0x20,0x10};   //逆时针旋转相序表  
uchar code tableCW[8]={0x10,0x20,0x40,0x80};    //顺时针旋转相序表  
uint keyscan()
{		 uchar temp,num;
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
			case 0xee:num=1;
				break;
			case 0xde:num=2;
				break;
			case 0xbe:num=3;
				break;
			case 0x7e:num=4;
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
			case 0xed:num=5;
				break;
			case 0xdd:num=6;
				break;
			case 0xbd:num=7;
				break;
			case 0x7d:num=8;
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
			case 0xeb:num=9;
				break;
			case 0xdb:num=10;
				break;
			case 0xbb:num=11;
				break;
			case 0x7b:num=12;
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
			case 0xe7:num=13;
				break;
			case 0xd7:num=14;
				break;
			case 0xb7:num=15;
				break;
			case 0x77:num=16;
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
void display(uchar num)
{	uchar shi,ge;
	 shi=num/10;
	 ge=num%10;
        dula=1;
		P0=table[shi];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfe;
		wela=0;
		delay(5);

		dula=1;
		P0=table[ge];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfd;
		wela=0;
		delay(5);
}

/*void motorTurn_ccw()      //转1圈,逆时针  
{     
    uchar i,j;  
    for(j=0;j<4;j++)       //电机内部的转子旋转一周  
    {  
        if(keyscan()==14)   //按键没处理抖动  
        {  
            break;          //如果停止键按下，退出此循环  
        }  
        for(i=0;i<8;i++)    //旋转45度, 8*5.625°=45°  
        {  
            P1=tableCCW[i];  
            delay(speed);      //调节转速  
        }  
    }  
}  
  
void motorTurn_cw()         //转1圈 ,顺时针  
{  
   uchar i,j;  
    for(j=0;j<4;j++)     //8*45°=360°  
    {  
        if(keyscan()==13)  
        {  
            break;          //如果停止键按下，退出此循环  
        }  
        for(i=0;i<8;i++)    //旋转45度,8*5.625°=45°  
        {  
            P1=tableCW[i];  
            delay(speed);     //调节转速  
        }  
    }  
}*/  
  
  
void main()  
{  
    uint r,enable; 
	uint i,j; 
    uint N=65535; //减速步进电机,减速比1/64,所以N=64时,步进电机外主轴转一圈   
    P1=0xff; 
	_nop_();     
	//ledLock= LOCK;        //我们的P1口接有8个led灯,先锁定,不要让它们亮 ,节能环保   
    while(1)  
    {  
        if(keyscan()==14)                 //按键了  
        {  	speed=3;
			//display(speed); 
			for(r=0;r<N;r++)  
			{	for(j=0;j<4;j++)       //电机内部的转子旋转一周  
    			{  
        
            	P1=tableCCW[i];  
            	delay(speed);      //调节转速  
        		}
				 
			}  
    	}  
        else if(keyscan()==13)                 //按键了  
        {  	speed=3;
			//display(speed); 
			for(r=0;r<N;r++)  
			{	for(j=0;j<4;j++)       //电机内部的转子旋转一周  
    			{  
        
            	P1=tableCW[i];  
            	delay(speed);      //调节转速  
        		}
				if(keyscan()!=13)  
                {  
                   break;         //俺S5键立马停止  
                }  
			}  
    	}
        else if(keyscan()==16) 
        {  
            P1=0xf0;                //平时也是停止的 
			if(keyscan()!=16)  
                {  
                    break;  
                }  	       
        } 
		else if(keyscan()<13)
		{		   speed=3;
			speed=keyscan();
			enable=keyscan();
			
			for(r=0;r<N;r++)  
			{	for(j=0;j<4;j++)       //电机内部的转子旋转一周  
    			{  
        
            	P1=tableCW[i];  
            	display(speed); 
        		}
				if(keyscan()!=enable)
                {  
                   break;         //俺S5键立马停止  
                }  
			}  
        }
	} 
}  

void delay(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
} 