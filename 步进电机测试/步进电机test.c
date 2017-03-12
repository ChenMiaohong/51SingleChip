#include "reg52.h"
#include<intrins.h>

//���IO
#define MotorData P1 
#define uchar unsigned char
#define uint  unsigned int
//sbit F1 = P1^0;
//sbit F2 = P1^1;
//sbit F3 = P1^2;
//sbit F4 = P1^3;
//����IO
sbit S3=P3^5;
sbit S4=P3^4;
sbit S5=P3^6;
sbit S6=P3^7;
void delayms(uint x);
uchar speed;
uchar code tableCCW[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};   //��ʱ����ת�����  
uchar code tableCW[8]={0x09,0x01,0x03,0x02,0x06,0x04,0x0c,0x08};    //˳ʱ����ת�����  
  
void motorTurn_ccw()      //ת1Ȧ,��ʱ��  
{     
    uchar i,j;  
    for(j=0;j<8;j++)       //����ڲ���ת����תһ��  
    {  
        if(S5==0)   //����û������  
        {  
            break;          //���ֹͣ�����£��˳���ѭ��  
        }  
        for(i=0;i<8;i++)    //��ת45��, 8*5.625��=45��  
        {  
            P1=tableCCW[i];  
            delayms(speed);      //����ת��  
        }  
    }  
}  
  
void motorTurn_cw()         //ת1Ȧ ,˳ʱ��  
{  
   uchar i,j;  
    for(j=0;j<8;j++)     //8*45��=360��  
    {  
        if(S5==0)  
        {  
            break;          //���ֹͣ�����£��˳���ѭ��  
        }  
        for(i=0;i<8;i++)    //��ת45��,8*5.625��=45��  
        {  
            P1=tableCW[i];  
            delayms(speed);     //����ת��  
        }  
    }  
}  
  
  
void main()  
{  
    uint r;  
    uint N=65535; //���ٲ������,���ٱ�1/64,����N=64ʱ,�������������תһȦ   
    P1=0xff; _nop_();     
    speed=2;
	//ledLock= LOCK;        //���ǵ�P1�ڽ���8��led��,������,��Ҫ�������� ,���ܻ���   
    while(1)  
    {  
        if(S3==0)                 //������  
        {  
            for(r=0;r<N;r++)  
            {  
                motorTurn_ccw();  //�����ʱ��ת  
                if(S5==0)  
                {  
                   break;         //��S5������ֹͣ  
                }  
				 if(S4==0)  
                {  
                   break;         //��S5������ֹͣ  
                }
            }  
        }  
        else if(S4==0)  
        {  
            for(r=0;r<N;r++)  
            {  
                motorTurn_cw();    //���˳ʱ��ת  
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
                motorTurn_cw();    //���˳ʱ��ת 
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
   					speed=2;    //���¿�ʼ�����˶� 
            	}  
		 
		 }
		}
        else if(S5==0) 
        {  
            P1=0xf0;                //ƽʱҲ��ֹͣ�� 
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