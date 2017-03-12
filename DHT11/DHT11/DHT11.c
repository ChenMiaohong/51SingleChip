/***************************************** 
 *    项目： 单片VB上位机视频教程        *
 *  单片机： STC89C52					 *
 *    晶振： 11.0592M					 *
 *    开发： E动力电子工作室			 *
 *    作者： LinHill					 *
 *    邮箱： power_e@126.com			 *
******************************************/

#include <reg52.h>


#define uchar unsigned char
#define uint unsigned int


sbit TRH   = P3^3;					// 温湿度传感器DHT11数据接入


bit enable=0;


/***********************************
   函数：void Delay5us()
 ----------------------
   说明：5微秒延时函数
   参数：无
 返回值：无
***********************************/
void Delay5us()
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}


/***********************************
   函数：DelayMs(uint z)
 ----------------------
   说明：毫秒级的延时
   参数：z 代表要延时的毫秒数
 返回值：无
***********************************/ 
void DelayMs(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}


/************************
   函数：void InitUart()
 ----------------------
   说明：对串口进行初始化
   参数：无
 返回值：无
************************/
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


/*****************************************
   函数：void send(uint dat)
 ---------------------------
   说明：将测得的温度/湿度通过串口发送出去
   参数：dat是测得的温度或湿度
 返回值：无
*****************************************/	
void Send(uchar dat)
{
	SBUF=(dat/10)+48;			// 发送 十 位 
	while(!TI);
	TI=0;
	SBUF=(dat%10)+48;			// 发送 个 位 
	while(!TI);
	TI=0;
}



/*****************************************
   函数：uchar DhtReadByte(void)
 ---------------------------
   说明：读取DHT11的一个字节
   参数：
 返回值：读取到的字节内容
*****************************************/
uchar DhtReadByte(void)
{   
	bit bit_i; 
	uchar j;
	uchar dat=0;
	for(j=0;j<8;j++)    
	{
		while(!TRH);			// 等待低电平结束	
		Delay5us();				// 延时
		Delay5us();
		Delay5us();
		if(TRH)					// 检测数据线是高电平还是低电平
		{
			bit_i=1; 
			while(TRH);
		} 
		else
		{
			bit_i=0;
		}
		dat<<=1;		   		// 将该位移位保存到dat变量中
		dat|=bit_i;    
	}
    return(dat);  
}


/****************************************
   函数：void ReadTrh(void)
 ---------------------------
   说明：将读取到的温湿度通过串口发送出去
   参数：
 返回值：读取到的字节内容
****************************************/
void ReadTrh(void)
{    
	uchar check;				// 校验字节
	uchar TemHig,TemLow,HumiHig,HumiLow;       
	TRH=0;						// 主机拉低18ms
	DelayMs(18);
	TRH=1;						// DATA总线由上拉电阻拉高 主机延时20us
	Delay5us();		
	Delay5us();
	Delay5us();
	Delay5us();					// 主机设为输入 判断从机响应信号
	while(!TRH);				// 等待DHT 80us的低电平结束
	while(TRH);					// 等待DHT 80us的高电平结束
	HumiHig = DhtReadByte(); 	// 湿度高8位
	HumiLow = DhtReadByte(); 	// 湿度低8为，总为0
	TemHig  = DhtReadByte(); 	// 温度高8位 
	TemLow  = DhtReadByte(); 	// 温度低8为，总为0 
	check   = DhtReadByte();	// 8位校验码，其值等于读出的四个字节相加之和的低8位
	TRH=1;						// 释放总线
	if(check==HumiHig + HumiLow + TemHig + TemLow) 			// 如果收到的数据无误
	{
		Send(TemHig);
		Send(HumiHig);
	}
}		


/*******************************
            主函数
*******************************/
void main(void)
{
	InitUart();
	DelayMs(1500);
	while(1)
	{
		if(enable==1)
		{
			ReadTrh();
			DelayMs(1000);
			DelayMs(1000);
		}		
	}
} 




/***********************************
 函数：void Inter() interrupt 4
 ------------------------------
   说明：串口中断处理
   参数：无
 返回值：无
***********************************/
void in() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF==0x01)
		{
			enable=1;
		}
		if(SBUF==0x02)
		{
			enable=0;
		}
	}
}
