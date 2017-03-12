//#include<reg51.h>
#include <intrins.h>
//#include <STC_NEW_8051.H> //STC����ͷ�ļ�
#include <STC\STC12C5A.H>


/*****************************************************************************
*  ��ʼ������														         *
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
uint Flag;						//�ж����λ
#define AD_SPEED 0x40 //0100,0000   1      0     140 ��ʱ������ת��һ��
uchar get12;


/*****************************************************************************
*  ��ʱ�ӳ���															     *
*																			 *
******************************************************************************/
void delay50us(unsigned int us)
{
	unsigned char j;
	for(;us>0;us--)
	for(j=19;j>0;j--);
}

void delay(uchar delay_time)        // ��ʱ����
{
    uint n;
    while(delay_time--) 
    { 
        n = 6000;
        while(--n);
    }
}
/*****************************************************************************
*  ADת��															         *
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

uchar get_AD_result(uchar channel)  //ADת������
{
    uint AD_finished=0;            //�洢 A/D ת����־

    ADC_RES = 0;                    //�߰�λ
    ADC_RESL = 0;                   //�Ͷ�λ(����δʹ��) 
    channel &= 0x07;                //0000,0111 ��0��5λ
    ADC_CONTR = AD_SPEED;
    _nop_();
    ADC_CONTR |= channel;           //ѡ�� A/D ��ǰͨ��
    _nop_();
    ADC_CONTR |= 0x80;              //���� A/D ��Դ
    delay(1);                       //ʹ�����ѹ�ﵽ�ȶ�
    ADC_CONTR |= 0x08;              //0000,1000 �� ADCS = 1, ����A/Dת��, 
    AD_finished = 0;
    while (AD_finished ==0 )        //�ȴ�A/Dת������
    {
        AD_finished = (ADC_CONTR & 0x10); //0001,0000 ����A/Dת��������
    }
    ADC_CONTR &= 0xE7;              //1111,0111 �� ADC_FLAG λ, �ر�A/Dת��, 
    return (ADC_RES);               //���� A/D �� 8 λת�����
	//Flag=ADC_CONTR&0x7F;			//�������λ���жϵ�ѹ����
}
 
/*****************************************************************************
*  ����������															     *
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
*  ��������															     *
*																			 *
******************************************************************************/
/*if(112<=value) 
	{
	    if(value<122) //��΢
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
			        MildLED=0; 	//һ��
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
						    MildLED=0;	 //������
							GeneralLED=0;
							MoreseriousLED=0;
							SeriousLED=1;
						}
						else
						{				   //������
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
*  ������															         *
*																			 *
******************************************************************************/
void main()
{
	//uint  D_value;//�����źŽ��
	//uchar A_value;//ģ���źŽ��
					 
	MildLED=1;	  //��ʼ���ܽ�
	GeneralLED=1;
	MoreseriousLED=1;
	SeriousLED=1;
	Beep=1;
	
	P1ASF = 0x20;                   //0000,0010, �� P1.1 �ó�ģ���	 ��Ҫ
    AUXR1 &= ~0x04;                 //0000,0100, �� ADRJ=0 : 10 λA/D ת������ĸ�8 λ����ADC_RES �Ĵ���, ��2 λ����ADC_RESL �Ĵ���
    ADC_CONTR |= 0x80;              //1000,0000 �� A/D ת����Դ

	while(1)
	{	
	    delay50us(100); 
	    P2=get_AD_result(5);//P1.6 Ϊ A/D ��ǰͨ��, ���������ͽ��
		Flag=get_AD_result(5);
		get12=Flag&0x80;  //10000000 ȡ���λ
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
        //A_value=D_value/256*5*1000;        //ADԭ��ʽ��ADC_result/256*5V*1000
        
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
