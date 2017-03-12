#include<reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define aa 0x3D
//AD���ݿ�
sbit         cs=P0^7;         //оƬѡ���ź�,����оƬ�������ͽ����ȡ,�͵�ƽ��Ч
sbit         rd=P3^7;         //�����ݿ���,�͵�ƽ��Ч
sbit         wr=P3^6;         //ADת���𶯿���,��������Ч
bit enable=0;
sbit dula=P2^6;  //����ܶ�ѡ,�����������ź�
sbit wela=P2^7;  //�����λѡ,�����������ź�
sfr IAP_CONTR=0xE7;
sbit TRH   = P3^3;					// ��ʪ�ȴ�����DHT11���ݽ���

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
void Sendgas(uint ad_data)
{
   SBUF=((ad_data/51)+1)+48;			// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=((ad_data%51*10/51)+1)+48;			// ���� �� λ 
	while(!TI);
	TI=0;
    SBUF=((ad_data%51*10%51*10/51)+1)+48;			// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=((ad_data%51*10%51*10%51*10/51)+1)+48;			// ���� �� λ 
	while(!TI);
	TI=0;
}
void Sendtem(uchar dat)
{

	SBUF=((dat/10)+1)+48;			// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=((dat%10)+1)+48;			// ���� �� λ 
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
		while(!TRH);			// �ȴ��͵�ƽ����	
		Delay5us();				// ��ʱ
		Delay5us();
		Delay5us();
		if(TRH)					// ����������Ǹߵ�ƽ���ǵ͵�ƽ
		{
			bit_i=1; 
			while(TRH);
		} 
		else
		{
			bit_i=0;
		}
		dat<<=1;		   		// ����λ��λ���浽dat������
		dat|=bit_i;    
	}
    return(dat);  
}


/****************************************
   ������void ReadTrh(void)
 ---------------------------
   ˵��������ȡ������ʪ��ͨ�����ڷ��ͳ�ȥ
   ������
 ����ֵ����ȡ�����ֽ�����
****************************************/
void ReadTrh(void)
{    
	uchar check;				// У���ֽ�
	uchar TemHig,TemLow,HumiHig,HumiLow;       
	TRH=0;						// ��������18ms
	DelayMs(18);
	TRH=1;						// DATA������������������ ������ʱ20us
	Delay5us();		
	Delay5us();
	Delay5us();
	Delay5us();					// ������Ϊ���� �жϴӻ���Ӧ�ź�
	while(!TRH);				// �ȴ�DHT 80us�ĵ͵�ƽ����
	while(TRH);					// �ȴ�DHT 80us�ĸߵ�ƽ����
	HumiHig = DhtReadByte(); 	// ʪ�ȸ�8λ
	HumiLow = DhtReadByte(); 	// ʪ�ȵ�8Ϊ����Ϊ0
	TemHig  = DhtReadByte(); 	// �¶ȸ�8λ 
	TemLow  = DhtReadByte(); 	// �¶ȵ�8Ϊ����Ϊ0 
	check   = DhtReadByte();	// 8λУ���룬��ֵ���ڶ������ĸ��ֽ����֮�͵ĵ�8λ
	TRH=1;						// �ͷ�����
	if(check==HumiHig + HumiLow + TemHig + TemLow) 			// ����յ�����������
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
                //����AD
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

