//���ƿڶ���//////////////////////
#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define aa 0x3D
//#define      ad0_7      P1       //AD���ݿ�
sbit         cs=P0^7;         //оƬѡ���ź�,����оƬ�������ͽ����ȡ,�͵�ƽ��Ч
sbit         rd=P3^7;         //�����ݿ���,�͵�ƽ��Ч
sbit         wr=P3^6;         //ADת���𶯿���,��������Ч
bit enable=0;
sbit dula=P2^6;  //����ܶ�ѡ,�����������ź�
sbit wela=P2^7;  //�����λѡ,�����������ź�
sfr IAP_CONTR=0xE7;

uchar led[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
//5ms��ʱ�ӳ���/////////////////
void delay(uint z)
{       uint i,j;
        for(i=z;i>0;i--)
        for(j=210;j>0;j--);
}
//����ADת���ӳ���//////////////////////////////////////////////////////////
void start_ad(void)
{       wela=1;
		cs=0;                 //�������A/Dת��
        wr=0;
        _nop_();
          wr=1; //WR�ɵͱ��ʱ,AD��ʼת��
    	cs=1;                 //ֹͣADת��
}
//��A/D�����ӳ���///////////////////////////////////////
       read_ad()
    {   uint ad_data;
       P1=0xff;
		wela=1;
        cs=0;                 //�����
        rd=0;                 //��ȡת�����ݽ�����ݽ��
        _nop_();
        ad_data=P1;         //�����ݴ浽ad_data��
		wela=1;
        rd=1;
        cs=1;            //ֹͣA/D��ȡ
        return(ad_data);
}
//���ݴ�������ʾ�ӳ���//////////////////////////////////////
//˵��:�������ѹΪ5Vʱ,A/D���ΪFFH,�������ѹ=AD����*(5/255)=AD����/(255/5)=AD����/51
void data_shout(uint ad_data)
{       uint a=50,one,two,three,four;
         four=ad_data/51;                     //��һλ
         three=ad_data%51*10/51;              //�ڶ�λ
         two=ad_data%51*10%51*10/51;          //����λ
         one=ad_data%51*10%51*10%51*10/51; //����λ�����(���λ)
        while(a--)
        {   
	    	dula=0;		
			P0 = led[one]; //���λ
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xf7;   
			wela=1;
			wela=0;	
			
			delay(1);
		   
			dula=0;		
			P0 = led[two]; //����λ�����
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfb;   
			wela=1;
			wela=0;	

                        delay(1);


			dula=0;		
			P0 = led[three]; //�ڶ�λ�����
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfd;   
			wela=1;
			wela=0;	

                        delay(1);

			dula=0;		
			P0 = led[four];//��һλ�����
			dula=1;
			dula=0;
	
			wela=0;
			P0=0xfe;   
			wela=1;
			wela=0;	

            delay(1);
         }
}
void Send(uint ad_data)
{
SBUF=(ad_data/51);			// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=(ad_data%51*10/51);			// ���� �� λ 
	while(!TI);
	TI=0;
SBUF=(ad_data%51*10%51*10/51);			// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=(ad_data%51*10%51*10%51*10/51);			// ���� �� λ 
	while(!TI);
	TI=0;



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
       {       start_ad();             //����AD
            data_shout(read_ad()); 
 //��AD���ݲ���ʾ
if(enable==1)
{
              Send(read_ad());

enable=0;
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
			enable=1;;
		}
		if(SBUF==0x02)
		{
			enable=0;
		}
	}
}

