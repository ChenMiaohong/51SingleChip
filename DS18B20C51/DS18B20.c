#include<reg52.h>

#define uint  unsigned int
#define uchar unsigned char

sbit DQ=P2^4;    			// ��DS18B20�����ݶ�

bit enable=0;

/***********************************
   ������DelayMs(uint z)
 ----------------------
   ˵�������뼶����ʱ
   ������z ����Ҫ��ʱ�ĺ�����
 ����ֵ����
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
   ˵������λ����DS18B20
   ��������
 ����ֵ����
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
   ˵������ȡDS18B20��һ���ֽ�
   ��������
 ����ֵ�����ض�ȡ�����ֽ�
***********************************/
uchar ReadByte(void)
{
	uchar i,j,b,dat=0;
	for(j=0;j<8;j++)
	{						
		DQ=0;
		i++;
		DQ=1;	
		i=3;		// ��ʱ15us
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
   ˵����д���ݵ�һ���ֽڣ�����д1��д0��ʱ϶Ҫ��
   ������b����Ҫд�뵽DS18B20������
 ����ֵ����
************************************************/
void WriteByte(uchar b)
{
	uint i;
	uchar j;
	bit btmp;
	for(j=0;j<8;j++)
	{
		btmp=b&0x01;
		b=b>>1; 			// ȡ��һλ���ɵ�λ���λ��
		if(btmp)
		{
			DQ=0;
			i++;
			i++;
			DQ=1;
			i=10;
			while(i--);		// ����д1ʱ϶������60us
		}
		else
		{
			DQ=0;
			i=10;
			while(i--); 	// ���ֵ���60us��120us֮��
			DQ=1;
			i++;
			i++;
		}
	}
}




/************************************************
   uint ReadTemp(void)
 ------------------
   ˵������ȡ�¶�ֵ
   ��������
 ����ֵ�����ض�ȡ�����¶�
************************************************/
uint ReadTemp(void)
{
	uchar TempLow,TempHig;	 // �¶�ֵ��λ����λ�ֽ�
	float tt;
	uint temp;
	ReSet();				// ������λ���壬��ʼ��DS18B20
	WriteByte(0xcc);		// skip rom ����
	WriteByte(0x44);		// convert T ����
	ReSet();
	WriteByte(0xcc);		// skip rom ����
	WriteByte(0xbe);		// read �¶�����
	TempLow=ReadByte();		// �¶�ֵ��λ�ֽڣ����е�4λΪ�����Ƶ�"С��"���֣�
	TempHig=ReadByte();		// ��λֵ��λ�ֽڣ����и�5λΪ����λ��
	temp=TempHig;
	temp<<=8;
	temp=temp|TempLow;
	tt=temp*0.0625;
	temp=tt*10+0.5;
	return (temp);
}



/***********************************
   ������void send(uint dat)
 ---------------------------
   ˵��������õľ���ͨ�����ڷ��ͳ�ȥ
   ������dat�ǲ�õľ���
 ����ֵ����
***********************************/	
void Send(uint dat)
{
	SBUF=(dat/1000)+48;		// ���� ǧ λ 
	while(!TI);
	TI=0;
	SBUF=(dat%1000/100)+48;	// ���� �� λ 
	while(!TI);
	TI=0;
	SBUF=(dat%100/10)+48;		// ���� ʮ λ 
	while(!TI);
	TI=0;
	SBUF=(dat%10)+48;			// ���� �� λ 
	while(!TI);
	TI=0;
}

/***********************************
   ������void InitUart()
 ----------------------
   ˵�����Դ��ڽ��г�ʼ��
   ��������
 ����ֵ����
***********************************/
void InitUart()
{
    TMOD = 0x20;				// ��4λ����ʱ��1�����ڴ���
	SCON = 0x50;	
	TH1  = 0xFD;				// ������ 9600
	TL1  = 0xFD; 
	TR1  = 1;					// ������ʱ��1
	EA   = 1;					// �������ж�
	ES   = 1; 
}



/***********************
   ������void main(void)
 ----------------------
   ˵����������
   ��������
 ����ֵ����
***********************/
void main()
{
	uint temp;					// ���������ȡ�����¶�ֵ
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
 ������void Inter() interrupt 4
 ------------------------------
   ˵���������жϴ���
   ��������
 ����ֵ����
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