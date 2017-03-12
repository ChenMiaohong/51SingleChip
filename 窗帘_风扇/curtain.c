#include<reg52.h>
#define uchar unsigned char
#define uint  unsigned int
#define MotorData P1  
sbit beep=P3^3;               //����������ƽӿڶ���
sbit dudu=P2^3;//������
uchar phasecw[4] ={0x08,0x04,0x02,0x01};//��ת �����ͨ���� D-C-B-A
uchar phaseccw[4]={0x01,0x02,0x04,0x08};//��ת �����ͨ���� A-B-C-D
uchar enable,flag,status,stat,atta,stta,temp,enabletemp,enablefan,fan;
//ms��ʱ����
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//˳ʱ��ת��
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(3);//ת�ٵ���
  }
}
//��ʱ��ת��
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(3);//ת�ٵ���
  }
}
//ֹͣת��
void MotorStop(void)
{
 MotorData=0x00;
}
void InitUart()
{
    TMOD = 0x20;				// ��4λ����ʱ��1�����ڴ���
	SCON = 0x50;	
	TH1  = 0xFD;				// ������ 9600
	TL1  = 0xFD; 
	TR1  = 1;					// ������ʱ��1
	ES   = 1; 
	EA   = 1;					// �������ж�


}
void main()
{	uint i;
 	InitUart();
	atta=6;//����״̬��־
	stta=8;//������״̬��־
	enablefan=9;//����״̬��־
	while(1)
	{
		if(enable==1)
 		{
  			for(i=0;i<1500;i++)
  			{
  				MotorCCW();//��ʱ��ת��
  				//Delay_xms(500);
  			} 
  		MotorStop();//ֹͣת��
  		enable=0;
		atta=5;
 		}
		if(enable==2)
		{
		  //Delay_xms(50);
		  for(i=0;i<1630;i++)
		  {	   
		 	MotorCW();   //˳ʱ��ת��
		  } 
		  MotorStop();  //ֹͣת��
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
			  	SBUF='X';			// ���ͷ��ȹرձ�־λ 
				while(!TI);
				TI=0;
				//atta=0;
			}
	
			if(enablefan==8)
			{
			  	SBUF='Y';			// ���ͷ��ȴ򿪱�־ 
				while(!TI);
				TI=0;
				
			}
			fan=0;
		}
		if(status==5)
		{
		  	if(atta==6)
			{
			  	SBUF='D';			// ���ʹ����رձ�־λ 
				while(!TI);
				TI=0;
				//atta=0;
			}
	
			if(atta==5)
			{
			  	SBUF='E';			// ���ʹ����򿪱�־ 
				while(!TI);
				TI=0;
				
			}
			status=0;
		}
		if(temp==6)
		{
			if(stta==8)
			{
				SBUF='P';			// ���� ������״̬��λ 
				while(!TI);
				TI=0;
			}
			if(stta==6)
			{
				SBUF='Q';			// ���� ������״̬�ر�λ 
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
		if(SBUF=='C')//�򿪴���
		{
			enable=1;
		}
		if(SBUF=='T')//�رմ���
		{
			enable=2;
		}
		if(SBUF=='Z')//�򿪷���
		{
			flag=3;
		}
		if(SBUF=='M')//�رշ���
		{
			flag=4;
		}
		if(SBUF=='A')//��ȡ����״̬
		{
			status=5;
		}
		if(SBUF=='B')//��ȡ������״̬
		{
			temp=6;
		}
		if(SBUF=='K')//�رշ�����
		{
			enabletemp=7;
		}
		if(SBUF=='L')//�򿪷�����
		{
			enabletemp=8;
		}
		if(SBUF=='R')//��ȡ����״̬
		{
			fan=9;
		}
	}
}