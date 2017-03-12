#include <reg52.h>
#define uint unsigned int
#define uchar unsigned char
sbit dula=P2^6;
sbit wela=P2^7;
 uchar code table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};
//int table[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	 uint temp;

	void display(uint aa,uint bb,uint cc);
void delay(uint z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=110;y>0;y--);
}

	
uchar keyboard_Scan[]={0xef,0xdf,0xbf,0x7f};
uchar Keyboard()
{	
	uchar i,j,pc,in;
	in = 0;
	for(i=0;i<4;i++)
	{
		P3=keyboard_Scan[i];
		pc=P3;
		pc=pc<<4|0x0f;         
		for(j=0;j<4;j++)       
		{
			if(keyboard_Scan[j]==pc)in=i*4+j;
			if(in>=10)in=15-in;
			while(keyboard_Scan[i]!=P3)         
			{
				P3=keyboard_Scan[i];
			}
		}	
	}
	return in;
}	 
uint keyscan()
{	  uint num;
	P3=0xfe;
	temp=P3;
	temp=temp&0xf0;
	while(temp!=0xf0)
	{
		delay(5);
		temp=P3;
		temp=temp&0xf0;
		while(temp!=0xf0)
		{
			temp=P3;
		switch(temp)
		{
			case 0xee:num=1;
				break;
			case 0xde:num=2;
				break;
			case 0xbe:num=3	;
				break;
			case 0x7e:num=4	;
				break;
		
		}
		while(temp!=0xf0)
		{
			temp=P3;
			temp=temp&0xf0;
		}
		 
		    return num;
		}
	}
	 
} 
void display(uint aa,uint bb,uint cc)
{
        dula=1;
		P0=table[aa];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfe;
		wela=0;
		delay(1);

       	dula=1;
		P0=table[bb];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfd;
		wela=0;
		delay(1);

       dula=1;
		P0=table[cc];
		dula=0;
		P0=0xff;
		wela=1;
		P0=0xfb;
		wela=0;
		delay(1);
}
int main(){
		  	uint number = 222;
	uint keyval,one,two,three;
	while(1){	   
		  	 keyval=Keyscan();
	/*switch(keyval)
		{
			case 1:number++;
				break;
			case 2:number--;
				break;
			
		}	*/
				//delay(5);
			 if(keyval==1)number++;	
			 //delay(5);
			 if(keyval==2)number--;
		display(number/100,number%100/10,number%10);
	} 
}
