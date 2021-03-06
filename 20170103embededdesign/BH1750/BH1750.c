#include  <reg52.h>	
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>
#define   uchar unsigned char
#define   uint unsigned int	
sbit	  SCL=P1^6;      //IIC时钟引脚定义
sbit  	  SDA=P1^7;      //IIC数据引脚定义
sbit      water=P1^0;
uint enable;
uint temp,flag;
uint waterstat;
#define	  SlaveAddress   0x46 //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
                              //ALT   ADDRESS引脚接地时地址为0x46，接电源时地址为0xB8
typedef   unsigned char BYTE;
typedef   unsigned short WORD;

BYTE    BUF[8];                         //接收数据缓存区      	
int     dis_data;                       //变量

void delay_nms(unsigned int k);
void InitLcd();
void Init_BH1750(void);

void WriteDataLCM(uchar dataW);
void WriteCommandLCM(uchar CMD,uchar Attribc);
void DisplayOneChar(uchar X,uchar Y,uchar DData);
void conversion(uint temp_data);

void  Single_Write_BH1750(uchar REG_Address);               //单个写入数据
uchar Single_Read_BH1750(uchar REG_Address);                //单个读取内部寄存器数据
void  Multiple_Read_BH1750();                               //连续的读取内部寄存器数据
//------------------------------------
void Delay5us();
void Delay5ms();
void BH1750_Start();                    //起始信号
void BH1750_Stop();                     //停止信号
void BH1750_SendACK(bit ack);           //应答ACK
bit  BH1750_RecvACK();                  //读ack
void BH1750_SendByte(BYTE dat);         //IIC单个字节写
BYTE BH1750_RecvByte();                 //IIC单个字节读

//-----------------------------------
void Senddata(uint temp_data)
{
	SBUF=temp_data/10000+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%10000;   //取余运算
	SBUF=temp_data/1000+48 ;
	while(!TI);
	TI=0;
    temp_data=temp_data%1000;    //取余运算

    SBUF=temp_data/100+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%100;     //取余运算
    SBUF=temp_data/10+48;
	while(!TI);
	TI=0;
    temp_data=temp_data%10;      //取余运算
    SBUF=temp_data+48; 
	while(!TI);
	TI=0;	
}
void uartInit()
{
	TMOD=0x20;
	SCON=0x50;
	TH1=0xFD;
	TL1=0xFD;
	TR1=1;
	EA=1;
	ES=1;
}
//毫秒延时**************************
void delay_nms(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}


/***********************************/
		
/***********************************/					

/**************************************
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5us()
{
    _nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

/**************************************
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
**************************************/
void Delay5ms()
{
    WORD n = 560;

    while (n--);
}

/**************************************
起始信号
**************************************/
void BH1750_Start()
{
    SDA = 1;                    //拉高数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 0;                    //产生下降沿
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
    SDA = 0;                    //拉低数据线
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SDA = 1;                    //产生上升沿
    Delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
    SDA = ack;                  //写应答信号
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
    SCL = 1;                    //拉高时钟线
    Delay5us();                 //延时
    CY = SDA;                   //读应答信号
    SCL = 0;                    //拉低时钟线
    Delay5us();                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        SDA = CY;               //送数据口
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    BH1750_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BH1750_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        SCL = 1;                //拉高时钟线
        Delay5us();             //延时
        dat |= SDA;             //读数据               
        SCL = 0;                //拉低时钟线
        Delay5us();             //延时
    }
    return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
  //  BH1750_SendByte(REG_data);       //内部寄存器数据，
    BH1750_Stop();                   //发送停止信号
}

//********单字节读取*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
    BH1750_SendByte(REG_Address);                   //发送存储单元地址，从0开始	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=BH1750_RecvByte();              //读出寄存器数据
	BH1750_SendACK(1);   
	BH1750_Stop();                           //停止信号
    return REG_data; 
}
*/
//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
void Multiple_read_BH1750(void)
{   uchar i;	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	
	 for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 3)
        {

           BH1750_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {		
          BH1750_SendACK(0);                //回应ACK
       }
   }

    BH1750_Stop();                          //停止信号
    Delay5ms();
}


//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750()
{
   Single_Write_BH1750(0x01);  

}
//*********************************************************
//主程序********
//*********************************************************
void main()
{  
   float temp;
   delay_nms(100);	    //延时100ms	
   //InitLcd();           //初始化LCD
   Init_BH1750();       //初始化BH1750
   uartInit();
 
  while(1)              //循环
  { 

    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

     delay_nms(180);              //延时180ms

    Multiple_Read_BH1750();       //连续读出数据，存储在BUF中

    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
    
    temp=(float)dis_data/1.2;

    //conversion(temp);

	if(enable==1)
	{
		Senddata(temp);
		delay_nms(100);
		enable=0;
	}
	if(enable==2)
	{
		 water=0; //启动水泵
		 waterstat=30;
	}
	if(enable==3)
	{
		 water=1;//关闭水泵
		 waterstat=40;
		 
	
	}
	if(flag==1)
	{
		if( waterstat==30)
		{
			SBUF='P';			
			while(!TI);
			TI=0;
		
		}
		if( waterstat==40)
		{
			SBUF='Q';			
			while(!TI);
			TI=0;
		
		}
	   flag=0;
	}
	
            
  }
} 
void Inter() interrupt 4
{
	if(RI==1)
	{
		RI=0;
		if(SBUF=='I')
		{
			enable=1;	
		}
		if(SBUF=='J')
		{
			enable=0;
		
		}
		if(SBUF=='M')
		{
			enable=2;	
		}
		if(SBUF=='N')
		{
			enable=3;
		
		}
		if(SBUF=='O')
		{
			flag=1;	
		}
		
	
	}


}
