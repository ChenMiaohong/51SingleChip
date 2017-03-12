/*******************************************************************/
/* TX-1C单片机实验板实验例程        							   */
/* 2007年8月第二期培训班课后习题详解							   */
/* 作者：郭天祥      											   */
/* 邮箱：txmcu@163.com 										  	   */
/* 网站：www.txmcu.com(天祥电子)    							   */
/*【版权】Copyright(C)天祥电子 www.txmcu.com  All Rights Reserved  */
/*【声明】此程序仅用于学习与参考，引用请注明版权和作者信息！       */
/*                                                                 */
/*******************************************************************/
/*                                                          
同时用两个定时器控制蜂鸣器发声，
定时器0控制频率，定时器1控制同个
频率持续的时间，间隔2s依次输出
1，10，50,100，200,400,800,
1k（hz）的方波?
设晶振频率为12MHz。
*/
/*********************************************************/
#include<reg52.h>  //52单片机头文件
#include <intrins.h> //包含有左右循环移位子函数的库
#define uint unsigned int    //宏定义
#define uchar unsigned char  //宏定义
sbit beep=P2^3;
uchar tt;
uint fre,flag;

void main()		    //主函数
{
	fre=50000;
	beep=0;
	TMOD=0x11;//设置定时器0,定时器1为工作方式1
	TH0=(65536-fre)/256;
	TL0=(65536-fre)%256;
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	EA=1;//开总中断
	ET0=1;//开定时器0中断
	ET1=1;
	TR1=1;
	TR0=1;//启动定时器0
	while(1);//等待中断产生
	
}

void timer0() interrupt 1   //定时器0中断
{
	TR0=0;    //进中断后先把定时器0中断关闭，防止内部程序过多而造成中断丢失
	TH0=(65536-fre)/256;
	TL0=(65536-fre)%256;
	tt++;
	if(flag<40)    //以下几个if分别用来选取不同的频率
		if(tt==10)
			{
				tt=0;
				fre=50000;
				beep=~beep;
			}
	if(flag>=40&&flag<80)
		{
			tt=0;
			fre=50000;
			beep=~beep;	
		}
	if(flag>=80&&flag<120)
		{
			tt=0;
			fre=10000;
			beep=~beep;	
		}
	if(flag>=120&&flag<160)
		{
			tt=0;
			fre=5000;
			beep=~beep;	
		}
	if(flag>=160&&flag<200)
		{
			tt=0;
			fre=2500;
			beep=~beep;	
		}
	if(flag>=200&&flag<240)
		{
			tt=0;
			fre=1250;
			beep=~beep;	
		}
	if(flag>=240&&flag<280)
		{
			tt=0;
			fre=625;
			beep=~beep;	
		}
	if(flag>=280&&flag<320)
		{
			tt=0;
			fre=312;
			beep=~beep;	
		}
	if(flag>=320&&flag<360)
		{
			tt=0;
			fre=156;
			beep=~beep;	
		}
	TR0=1;
}
void timer1() interrupt 3  //定时器1中断用来产生2秒时间定时
{
	TH1=(65536-50000)/256;
	TL1=(65536-50000)%256;
	flag++;
	if(flag==360)
	  {
	    flag=0;
	    fre=50000;
	  }
}
