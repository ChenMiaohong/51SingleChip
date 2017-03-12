	#include<reg52.h> 
				#include <intrins.h> 
				#include<stdio.h>
				#define uint unsigned int   
				#define uchar unsigned char  
				uint count;
				uint second;
				uint minute;
				uint hour;
				//uchar in;
				uint shour=10000;
				uint sminute=100;
				uchar beep;
			long int tep;
				uchar a,b,c;
				uchar j;
				void dec();
				void inc();
				void INT0_ISR();
				void timer0_ISR();
				
				sbit P1_4 = P1^4;
				sbit P3_4=P3^4;
				sbit P36 = P3^6;
				sbit P37=P3^7;
				sbit DUAN=P2^6;     
				sbit WEI=P2^7; 
				sbit voice=P2^2;
				
				uchar Temp[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x80};  
				uchar keyboard_Scan[]={0xef,0xdf,0xbf,0x7f};
				//?車那㊣o‘那y

	void delay(uint z)            
				{
					uint x,y;
					for(x=z;x>0;x--)
						for(y=110;y>0;y--);
					
				}
				//???車?邦?足
uchar Keyboard(void)
{	
	uchar i,j,pc,in;
	in=0;
	for(i=0;i<4;i++)
	{
		P3=keyboard_Scan[i];
		pc=P3;
		pc=pc<<4|0x0f;           //????????
		for(j=0;j<4;j++)         //????????????
		{
			if(keyboard_Scan[j]==pc)
			{
				in=i*4+j;
				return in;
			}
			//if(in>=10)in=15-in;
			while(keyboard_Scan[i]!=P3)            //????
			{
				P3=keyboard_Scan[i];
			}
		}	
	}
	return in;
}
		//那y??1邦??那?o‘那y
void display_8seg( unsigned long dis_number)
				{
						uchar i;
						uchar number[8];
					uchar temp=0xfe;
					
						for(i=0;i<8;i++)
						{
							if(i==2||i==5)
								 {
							number[i]=Temp[10];
								 }
								 else
								 {
							number[i] = Temp[dis_number%10];
							dis_number /= 10;
								 }
								 
						}
						
						for(i=0;i<8;i++)
						{  
						
							P0=temp;     						 
							WEI=1;       
							WEI=0;       
											
							P0=number[7-i];  
							DUAN=1;          
							DUAN=0;      
							delay(1);            
							temp=_crol_(temp,1); 
							
							P0=0x00;    
							DUAN=1;                   
							DUAN=0;     
							delay(15); 
						}
					}
void ss()
{if(a==0xff)
	{		
		 TR0=0;
				
	if(Keyboard()==8)
	{
		delay(110);
											//voice=0;
		second++;	
		if(second==60)
		second=0;	

	}	
								
	   delay(50);
								
	if(Keyboard()==5)
	{//delay(100);
											//voice=1;
		if(second==0)
		   second=60;
			second--;	
	}
	}						
}
void mm()
{
	if(b==0xff)
	    {		
		  	TR0=0;
				
		if(Keyboard()==8)
			{delay(110);
					minute++;	
					if(minute==60)
					minute=0;	
							//voice=0								
			}	
								
	        	delay(50);
								
			if(Keyboard()==5)
			{//delay(100);
											//voice=1;
				if(minute==0)
		          minute=60;
					minute--;
			}
	}
}
void hh()
{
	if(c==0xff)
	    {		
		  	TR0=0;
				
		if(Keyboard()==8)
		{   
				delay(0);
											//voice=0;
				hour++;	
				if(hour==24)
				hour=0;	

		}	
								
	    delay(50);
								
		if(Keyboard()==5)
		{//delay(100);
											//voice=1;
			if(hour==0)
		     hour=24;
			hour--;
										

		}						
	}
}

					
void main()
{ 
	tep=0;	
	beep=1;
	second = 0;
	minute=0;
	hour=0;
	count=0;
	a=0;b=0;c=0;
	P1_4 = 0;
	P36=1;
	P37=1;
	P0=0XFF;
	EA=0;
	
 TMOD=0x01;
 TH0=(65536-10000)/256;
 TL0=(65536-10000)%256;					
				 // EX0=1;
				 //IT0=1;
					ET0=0;						
					TR0=0;
	while(1)  
{		
	j=Keyboard();
	if(j==1)//second
		a=~a;
	if(j==2)//min
		b=~b;
	if(j==3)//hour
		c=~c;
	if(j==4)
		TR0=1;
	ss();
	mm();
	hh();
				
		
			tep=hour*shour+sminute*minute+second;
			display_8seg(90000);
	
					 
}
}
void inc()
{
	if(second==60)	
	{ 
		minute++;
		second=0;
		if(minute==60)
		{
				hour++;
				minute=0;
				if(hour==24)
					hour=0;
		}	
	}
							 			
	 tep=hour*shour+minute*sminute+second;
	display_8seg(tep);							
}		
	
void timer0_ISR() interrupt 1 
{
	TH0=(65536-10000)/256;  
	TL0=(65536-10000)%256;	
	count++;
	if(count==100)
			{
				count=0;
				second++;	
        		inc();				
					
			}		
		
 }
			
									

