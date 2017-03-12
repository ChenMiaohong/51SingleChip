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
sbit S3=P3^5;
sbit S4=P3^4;
sbit S5=P3^6;
sbit S6=P3^7;
void delayms(uint x);
uchar speed;
uchar code tableCCW[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};   //逆时针旋转相序表  
uchar code tableCW[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};    //顺时针旋转相序表  
  
void motorTurn_ccw()      //转1圈,逆时针  
{     
    uchar i,j;  
    for(j=0;j<8;j++)       //电机内部的转子旋转一周  
    {  
        if(S5==0)   //按键没处理抖动  
        {  
            break;          //如果停止键按下，退出此循环  
        }  
        for(i=0;i<8;i++)    //旋转45度, 8*5.625°=45°  
        {  
            P1=tableCCW[i];  
            delayms(speed);      //调节转速  
        }  
    }  
}  
  
void motorTurn_cw()         //转1圈 ,顺时针  
{  
   uchar i,j;  
    for(j=0;j<8;j++)     //8*45°=360°  
    {  
        if(S5==0)  
        {  
            break;          //如果停止键按下，退出此循环  
        }  
        for(i=0;i<8;i++)    //旋转45度,8*5.625°=45°  
        {  
            P1=tableCW[i];  
            delayms(speed);     //调节转速  
        }  
    }  
}  
  
  
void main()  
{  
    uint r;  
    uint N=65535; //减速步进电机,减速比1/64,所以N=64时,步进电机外主轴转一圈   
    P1=0xff; _nop_();     
    speed=2;
	//ledLock= LOCK;        //我们的P1口接有8个led灯,先锁定,不要让它们亮 ,节能环保   
    while(1)  
    {  
        if(S3==0)                 //按键了  
        {  
            for(r=0;r<N;r++)  
            {  
                motorTurn_ccw();  //电机逆时针转  
                if(S5==0)  
                {  
                   break;         //俺S5键立马停止  
                }  
				 if(S4==0)  
                {  
                   break;         //俺S5键立马停止  
                }
            }  
        }  
        else if(S4==0)  
        {  
            for(r=0;r<N;r++)  
            {  
                motorTurn_cw();    //电机顺时针转  
                if(S5==0)  
                {  
                    break;  
                }  
				if(S3==0)  
                {  
                    break;  
                }  
            }  
        } 
		 else if(S6==0) 
		 {
		 	for(r=0;r<N;r++)  
            {  
                motorTurn_cw();    //电机顺时针转 
				speed--; 
                if(S5==0)  
                {  
                    break;  
                }  
				if(S3==0)  
                {  
                    break;  
                }
				if(S4==0)  
                {  
                    break;  
                }   
				if(speed<0)  
  				{
   					speed=2;    //重新开始减速运动 
            	}  
		 
		 }
		}
        else if(S5==0) 
        {  
            P1=0xf0;                //平时也是停止的 
			if(S3==0)  
                {  
                    break;  
                }  
				if(S4==0)  
                {  
                    break;  
                } 
				if(S6==0)  
                {  
                    break;  
                }  
				
			       
        }  
    }  
} 
void delayms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}