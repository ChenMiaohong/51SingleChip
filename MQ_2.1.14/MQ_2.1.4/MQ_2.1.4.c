//#include<reg51.h>
#include <intrins.h>
//#include <STC_NEW_8051.H> //STC的新头文件
#include <STC\STC12C5A.H>


/*****************************************************************************
*  初始化数据														         *
*																			 *
******************************************************************************/

sbit CS=P0^0;
sbit RDD=P0^1;
sbit WRR=P0^2;
sbit INTR=P0^4;
sbit shi=P0^6;

typedef unsigned char uchar;
typedef unsigned int  uint;

sbit   MildLED=P1^0;
sbit   GeneralLED=P1^1;
sbit   MoreseriousLED=P1^2;
sbit   SeriousLED=P1^3;
sbit   Beep=P3^0;

uchar timer0h,timer0l,time;
uint Flag;						//判断最高位
#define AD_SPEED 0x40 //0100,0000   1      0     140 个时钟周期转换一次
uchar get12;


/*****************************************************************************
*  延时子程序															     *
*																			 *
******************************************************************************/
void delay50us(unsigned int us)
{
	unsigned char j;
	for(;us>0;us--)
	for(j=19;j>0;j--);
}

void delay(uchar delay_time)        // 延时函数
{
    uint n;
    while(delay_time--) 
    { 
        n = 6000;
        while(--n);
    }
}
/*****************************************************************************
*  AD转换															         *
*																			 *
******************************************************************************/

/*void ADchange()
{
        WRR=1;
		CS=1;
		RDD=1;
		CS=0;
		delay50us(1);
		WRR=0;
		delay50us(2);
		WRR=1;
		CS=1;
		delay50us(1);
		//INTR=0;
		delay50us(10);
		CS=0;
		RDD=0;	
		//INTR=1;
		delay50us(1);
		value=P2;
		RDD=1;
    	CS=1;
		delay50us(15000);

}*/

uchar get_AD_result(uchar channel)  //AD转换部分
{
    uint AD_finished=0;            //存储 A/D 转换标志

    ADC_RES = 0;                    //高八位
    ADC_RESL = 0;                   //低二位(本例未使用) 
    channel &= 0x07;                //0000,0111 清0高5位
    ADC_CONTR = AD_SPEED;
    _nop_();
    ADC_CONTR |= channel;           //选择 A/D 当前通道
    _nop_();
    ADC_CONTR |= 0x80;              //启动 A/D 电源
    delay(1);                       //使输入电压达到稳定
    ADC_CONTR |= 0x08;              //0000,1000 令 ADCS = 1, 启动A/D转换, 
    AD_finished = 0;
    while (AD_finished ==0 )        //等待A/D转换结束
    {
        AD_finished = (ADC_CONTR & 0x10); //0001,0000 测试A/D转换结束否
    }
    ADC_CONTR &= 0xE7;              //1111,0111 清 ADC_FLAG 位, 关闭A/D转换, 
    return (ADC_RES);               //返回 A/D 高 8 位转换结果
	//Flag=ADC_CONTR&0x7F;			//返回最高位。判断电压跳变
}
 
/*****************************************************************************
*  蜂鸣器报警															     *
*																			 *
******************************************************************************/
/*void delay(unsigned char t)
   {
   unsigned char t1;
   unsigned long t2;
   for(t1=0;t1<t;t1++)
   {
    for(t2=0;t2<8000;t2++)
       {
        ;
        }
   }
   TR0=0;
 }
void t0int() interrupt 1
{
 TR0=0;
 Beep=!Beep;
 TH0=timer0h;
 TL0=timer0l;
 TR0=1;
 }

void song()
{
 TH0=timer0h;
 TL0=timer0l;
 TR0=1;
 delay(time);                       
 }
 */
 /*****************************************************************************
*  报警程序															     *
*																			 *
******************************************************************************/
/*if(112<=value) 
	{
	    if(value<122) //轻微
		{
			MildLED=0;
			GeneralLED=1;
			MoreseriousLED=1;
			SeriousLED=1;
		}
        else
		{
		    if(122<=value)
			{
			   if(value<133)
			   {
			        MildLED=0; 	//一般
					GeneralLED=0;
					MoreseriousLED=1;
					SeriousLED=1;
			   }
			   else
			   {
			         if(133<=value)
					 {
					    if(value<143)
						{
						    MildLED=0;	 //较严重
							GeneralLED=0;
							MoreseriousLED=0;
							SeriousLED=1;
						}
						else
						{				   //很严重
						      MildLED=0;
							  GeneralLED=0;
							  MoreseriousLED=0;
							  SeriousLED=0;	
						}
					 }
			   }
			}
		}
} */
/*****************************************************************************
*  主程序															         *
*																			 *
******************************************************************************/
void main()
{
	//uint  D_value;//数字信号结果
	//uchar A_value;//模拟信号结果
					 
	MildLED=1;	  //初始化管脚
	GeneralLED=1;
	MoreseriousLED=1;
	SeriousLED=1;
	Beep=1;
	
	P1ASF = 0x20;                   //0000,0010, 将 P1.1 置成模拟口	 重要
    AUXR1 &= ~0x04;                 //0000,0100, 令 ADRJ=0 : 10 位A/D 转换结果的高8 位放在ADC_RES 寄存器, 低2 位放在ADC_RESL 寄存器
    ADC_CONTR |= 0x80;              //1000,0000 打开 A/D 转换电源

	while(1)
	{	
	    delay50us(100); 
	    P2=get_AD_result(5);//P1.6 为 A/D 当前通道, 测量并发送结果
		Flag=get_AD_result(5);
		get12=Flag&0x80;  //10000000 取最高位
		/*delay50us(1000000);
		Flag=get_AD_result(1);
	    P3=get12;*/            
		delay50us(100);
		if(get12==0x00)
		{
		    shi=1;
			MildLED=1;
			GeneralLED=1;
			MoreseriousLED=1;
			SeriousLED=1;
			Beep=1;
		}
		if(get12==0x80)
		{		    
		    shi=0;
			MildLED=0;
			GeneralLED=0;
			MoreseriousLED=0;
			SeriousLED=0;
			Beep=0;
			delay50us(10000);
			shi=1;
			MildLED=1;
			GeneralLED=1;
			MoreseriousLED=1;
			SeriousLED=1;
			Beep=1;
	    }
		delay50us(100);
        //A_value=D_value/256*5*1000;        //AD原公式是ADC_result/256*5V*1000
        
		/*MildLED=0;
		GeneralLED=0;
		MoreseriousLED=0;
		SeriousLED=0;
		Beep=0;*/
		/*if(A_value<2.5)
		{
		    MildLED=1;
			GeneralLED=1;
			MoreseriousLED=1;
			SeriousLED=1;
			Beep=1;
		} */
	    /*if(Flag==1)
		{
		     shi=1;
		}
		 if(Flag==0)
		{
		     shi=0;
		}	*/	
	}
}
