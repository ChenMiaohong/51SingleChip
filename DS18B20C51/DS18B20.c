#include<reg52.h>

#define uint  unsigned int
#define uchar unsigned char

sbit DQ=P2^4;    			// 接DS18B20的数据端

bit enable=0;

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


/***********************************
   void ReSet(void)
 ------------------
   说明：复位启动DS18B20
   参数：无
 返回值：无
***********************************/
void ReSet(void)
{
	uint i;
	DQ=0;
	i=100;
	while(i--);
	DQ=1;
	i=4;
	while(i--);
	while(DQ);
	while(~DQ);
	i=4;
	while(i--);
}



/***********************************
   uchar ReadByte(void)
 ------------------
   说明：读取DS18B20的一个字节
   参数：无
 返回值：返回读取到的字节
***********************************/
uchar ReadByte(void)
{
	uchar i,j,b,dat=0;
	for(j=0;j<8;j++)
	{						
		DQ=0;
		i++;
		DQ=1;	
		i=3;		// 延时15us
		while(--i);
		b=DQ;
		i=10;
		while(i--);
		dat=(b<<7)|(dat>>1);
	}
	return(dat);
}


/************************************************
   void WriteByte(uchar b)
 ------------------
   说明：写数据的一个字节，满足写1和写0的时隙要求
   参数：b代表要写入到DS18B20的内容
 返回值：无
************************************************/
void WriteByte(uchar b)
{
	uint i;
	uchar j;
	bit btmp;
	for(j=0;j<8;j++)
	{
		btmp=b&0x01;
		b=b>>1; 			// 取下一位（由低位向高位）
		if(btmp)
		{
			DQ=0;
			i++;
			i++;
			DQ=1;
			i=10;
			while(i--);		// 整个写1时隙不低于60us
		}
		else
		{
			DQ=0;
			i=10;
			while(i--); 	// 保持低在60us到120us之间
			DQ=1;
			i++;
			i++;
		}
	}
}




/************************************************
   uint ReadTemp(void)
 ------------------
   说明：读取温度值
   参数：无
 返回值：返回读取到的温度
************************************************/
uint ReadTemp(void)
{
	uchar TempLow,TempHig;	 // 温度值低位、高位字节
	float tt;
	uint temp;
	ReSet();				// 产生复位脉冲，初始化DS18B20
	WriteByte(0xcc);		// skip rom 命令
	WriteByte(0x44);		// convert T 命令
	ReSet();
	WriteByte(0xcc);		// skip rom 命令
	WriteByte(0xbe);		// read 温度命令
	TempLow=ReadByte();		// 温度值低位字节（其中低4位为二进制的"小数"部分）
	TempHig=ReadByte();		// 高位值高位字节（其中高5位为符号位）
	temp=TempHig;
	temp<<=8;
	temp=temp|TempLow;
	tt=temp*0.0625;
	temp=tt*10+0.5;
	return (temp);
}



/***********************************
   函数：void send(uint dat)
 ---------------------------
   说明：将测得的距离通过串口发送出去
   参数：dat是测得的距离
 返回值：无
***********************************/	
void Send(uint dat)
{
	SBUF=(dat/1000)+48;		// 发送 千 位 
	while(!TI);
	TI=0;
	SBUF=(dat%1000/100)+48;	// 发送 百 位 
	while(!TI);
	TI=0;
	SBUF=(dat%100/10)+48;		// 发送 十 位 
	while(!TI);
	TI=0;
	SBUF=(dat%10)+48;			// 发送 个 位 
	while(!TI);
	TI=0;
}

/***********************************
   函数：void InitUart()
 ----------------------
   说明：对串口进行初始化
   参数：无
 返回值：无
***********************************/
void InitUart()
{
    TMOD = 0x20;				// 高4位（定时器1）用于串口
	SCON = 0x50;	
	TH1  = 0xFD;				// 波特率 9600
	TL1  = 0xFD; 
	TR1  = 1;					// 开启定时器1
	EA   = 1;					// 开启总中断
	ES   = 1; 
}



/***********************
   函数：void main(void)
 ----------------------
   说明：主函数
   参数：无
 返回值：无
***********************/
void main()
{
	uint temp;					// 用来保存读取到的温度值
	InitUart();
	while(1)
	{
		if(enable==1)
		{		
			temp=ReadTemp();
			Send(temp);
			DelayMs(200);
			//enable=0;

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
void Inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF =='A')
		{
			enable = 1;
		}
		if(SBUF =='B')
		{
			enable = 0;
		}
	}	
}