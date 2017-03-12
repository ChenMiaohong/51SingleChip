#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define aa 0x3D
//AD数据口
sbit         cs=P0^7;         //芯片选择信号,控制芯片的启动和结果读取,低电平有效
sbit         rd=P3^7;         //读数据控制,低电平有效
sbit         wr=P3^6;         //AD转换起动控制,上升沿有效
bit enable=0;
sbit dula=P2^6;  //数码管段选,锁存器控制信号
sbit wela=P2^7;  //数码管位选,锁存器控制信号
sfr IAP_CONTR=0xE7;
sbit TRH   = P3^3;					// 温湿度传感器DHT11数据接入

uchar led[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void start_ad(void);
void delay(uint z)
{       uint i,j;
        for(i=z;i>0;i--)
        for(j=210;j>0;j--);
}
void DelayMs(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}
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
//启动AD转换子程序//////////////////////////////////////////////////////////
void start_ad(void)
{       wela=1;
		cs=0;                 //允许进行A/D转换
        wr=0;
        _nop_();
         wr=1; //WR由低变高时,AD开始转换
    	cs=1;                 //停止AD转换
}
//读A/D数据子程序///////////////////////////////////////
       read_ad()
    {   uint ad_data;
       P1=0xff;
		wela=1;
        cs=0;                 //允许读
        rd=0;                 //读取转换数据结果数据结果
        _nop_();
        ad_data=P1;         //把数据存到ad_data中
		wela=1;
        rd=1;
        cs=1;            //停止A/D读取
        return(ad_data);
}
//数据处理与显示子程序//////////////////////////////////////
//说明:当输入电压为5V时,A/D输出为FFH,即输入电压=AD数据*(5/255)=AD数据/(255/5)=AD数据/51
void data_shout(uint ad_data)
{       uint a=50,one,two,three,four;
         four=ad_data/51;                     //第一位
         three=ad_data%51*10/51;              //第二位
         two=ad_data%51*10%51*10/51;          //第三位
         one=ad_data%51*10%51*10%51*10/51; //第四位数码管(最低位)
        while(a--)
        {   
	    	dula=0;		
			P0 = led[one]; //最低位
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xf7;   
			wela=1;
			wela=0;	
			
			delay(1);
		   
			dula=0;		
			P0 = led[two]; //第三位数码管
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfb;   
			wela=1;
			wela=0;	

                        delay(1);


			dula=0;		
			P0 = led[three]; //第二位数码管
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfd;   
			wela=1;
			wela=0;	

                        delay(1);

			dula=0;		
			P0 = led[four];//第一位数码管
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfe;   
			wela=1;
			wela=0;	

            delay(1);
         }
}
void Sendgas(uint ad_data)
{
   SBUF=((ad_data/51)+1)+48;			// 发送 十 位 
	while(!TI);
	TI=0;
	SBUF=((ad_data%51*10/51)+1)+48;			// 发送 个 位 
	while(!TI);
	TI=0;
    SBUF=((ad_data%51*10%51*10/51)+1)+48;			// 发送 十 位 
	while(!TI);
	TI=0;
	SBUF=((ad_data%51*10%51*10%51*10/51)+1)+48;			// 发送 个 位 
	while(!TI);
	TI=0;
}
void Sendtem(uchar dat)
{

	SBUF=((dat/10)+1)+48;			// 发送 十 位 
	while(!TI);
	TI=0;
	SBUF=((dat%10)+1)+48;			// 发送 个 位 
	while(!TI);
	TI=0;
}
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
		Sendtem(TemHig);
		Sendtem(HumiHig);
	}
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

int main(void)
{uint ad_data;
	 Init_Com();
      while(1)
       {    
                //启动AD
   data_shout(read_ad()); 
if(enable==1)
{              start_ad();    
              Sendgas(read_ad());

              ReadTrh();
			DelayMs(1000);

			

//enable=0;
}

}
}
 

void inter() interrupt 4
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

