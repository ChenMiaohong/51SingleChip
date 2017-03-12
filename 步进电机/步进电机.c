#include <reg52.h>
 
sbit A1=P1^0; //定义步进电机连接端口
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;
 
#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A相通电，其他相断电
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B相通电，其他相断电
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C相通电，其他相断电
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D相通电，其他相断电
#define Coil_OFF{A1=0;B1=0;C1=0;D1=0;}//全部断电
 
 unsigned char Speed;
 /*------------------------------------------------
24  uS延时函数，含有输入参数 unsigned char t，无返回值
25  unsigned char 是定义无符号字符变量，其值的范围是
26  0~255 这里使用晶振12M，精确延时请使用汇编,大致延时
27  长度如下 T=tx2+5 uS 
28 ------------------------------------------------*/
 void DelayUs2x(unsigned char t)
 {   
  while(--t);
}
 /*------------------------------------------------
34  mS延时函数，含有输入参数 unsigned char t，无返回值
35  unsigned char 是定义无符号字符变量，其值的范围是
36  0~255 这里使用晶振12M，精确延时请使用汇编
37 ------------------------------------------------*/
 void DelayMs(unsigned char t)
 {  
  while(t--)
  {
      //大致延时1mS
      DelayUs2x(245);
      DelayUs2x(245);
  }
 }
 /*------------------------------------------------
48                     主函数
49 ------------------------------------------------*/
 main()
 {
  Speed=3; //调整速度
  while(1)  
   {         
      Coil_A1                 //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
     DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
                           //数字越小，转速越大,力矩越小
      Coil_B1
      DelayMs(Speed);
      Coil_C1
      DelayMs(Speed);
      Coil_D1
      DelayMs(Speed);
   }
 }