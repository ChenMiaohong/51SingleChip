#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P1  
sbit beep=P3^3;               //步进电机控制接口定义
sbit dudu=P2^3;//蜂鸣器
uchar phasecw[4] ={0x08,0x04,0x02,0x01};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0x01,0x02,0x04,0x08};//反转 电机导通相序 A-B-C-D
uchar enable,flag,status,stat,atta,stta,temp,enabletemp,enablefan,fan;
//ms延时函数
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//顺时针转动
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(3);//转速调节
  }
}
//逆时针转动
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(3);//转速调节
  }
}
//停止转动
void MotorStop(void)
{
 MotorData=0x00;
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
void main()
{	uint i;
 	InitUart();
	atta=6;//窗帘状态标志
	stta=8;//蜂鸣器状态标志
	enablefan=9;//风扇状态标志
	while(1)
	{
		if(enable==1)
 		{
  			for(i=0;i<1500;i++)
  			{
  				MotorCCW();//逆时针转动
  				//Delay_xms(500);
  			} 
  		MotorStop();//停止转动
  		enable=0;
		atta=5;
 		}
		if(enable==2)
		{
		  //Delay_xms(50);
		  for(i=0;i<1630;i++)
		  {	   
		 	MotorCW();   //顺时针转动
		  } 
		  MotorStop();  //停止转动
		  enable=0;
		  atta=6;
		  //Delay_xms(500);
		} 
		if(flag==3)
		{
		  	beep=0;
			enablefan=8;
		} 
		if(flag==4)
		{
		  	beep=1;
			enablefan=9;
			
		}
		if(fan==9)
		{
			if(enablefan==9)
			{
			  	SBUF='X';			// 发送风扇关闭标志位 
				while(!TI);
				TI=0;
				//atta=0;
			}
	
			if(enablefan==8)
			{
			  	SBUF='Y';			// 发送风扇打开标志 
				while(!TI);
				TI=0;
				
			}
			fan=0;
		}
		if(status==5)
		{
		  	if(atta==6)
			{
			  	SBUF='D';			// 发送窗帘关闭标志位 
				while(!TI);
				TI=0;
				//atta=0;
			}
	
			if(atta==5)
			{
			  	SBUF='E';			// 发送窗帘打开标志 
				while(!TI);
				TI=0;
				
			}
			status=0;
		}
		if(temp==6)
		{
			if(stta==8)
			{
				SBUF='P';			// 发送 蜂鸣器状态开位 
				while(!TI);
				TI=0;
			}
			if(stta==6)
			{
				SBUF='Q';			// 发送 蜂鸣器状态关闭位 
				while(!TI);
				TI=0;
			}
			
			temp=0;
		
		}
		if(enabletemp==7)
		{
			dudu=1;
			stta=6;
				
		}
		if(enabletemp==8)
		{
			dudu=0;
			stta=8;
				
		}


 	}
}
void inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF=='C')//打开窗帘
		{
			enable=1;
		}
		if(SBUF=='T')//关闭窗帘
		{
			enable=2;
		}
		if(SBUF=='Z')//打开风扇
		{
			flag=3;
		}
		if(SBUF=='M')//关闭风扇
		{
			flag=4;
		}
		if(SBUF=='A')//获取窗帘状态
		{
			status=5;
		}
		if(SBUF=='B')//获取蜂鸣器状态
		{
			temp=6;
		}
		if(SBUF=='K')//关闭蜂鸣器
		{
			enabletemp=7;
		}
		if(SBUF=='L')//打开蜂鸣器
		{
			enabletemp=8;
		}
		if(SBUF=='R')//获取风扇状态
		{
			fan=9;
		}
	}
}