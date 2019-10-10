#include<reg52.h>
#include <intrins.h>
#define  uchar  unsigned  char 
#define  uint   unsigned  int
sbit  DATA = P1^1; //DS18B20接入口
uchar code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//共阴极字型码
int temp;  //温度值
int ss;		//中间的一个变量
int dd;
int j;
uchar data b;//定时器中断次数
uchar data buf[4];//字型显示中间变量
int alarmH=350;	  //默认报警值
int alarmL=-100;
//定义开关的接入口
sbit  k1=P2^5;//+
sbit  k2=P2^6;//-
sbit  k3=P2^7;//确认
sbit  k4=P2^4; //切换
sbit  bell=P1^0; //蜂鸣器
sbit  HLight=P1^2; //正温指示灯
sbit  LLight=P1^3; //负温度指示灯
sbit  warn=P1^4;  //报警指示灯
sbit  Red=P1^6;	  //温度上限设置指示灯
sbit  Green=P1^7; //温度下限设置指示灯
bit set=0;	//初始化
bit Flag=0;	//设置标志
int n;
//函数的声明区
void key_to1();
void key_to2();
void delay(uint); 
void key();
void Show();
//函数的定义区
/*延时子函数*/
void delay(uint num)
{
	while(num--) ;
}

//DS18b20温度传感器所需函数，分为初始化，读写字节，读取温度4个函数
Init_DS18B20(void)		   //传感器初始化
{
 	uchar x=0;
 	DATA = 1;    //DQ复位
 	delay(10);  //稍做延时
 	DATA = 0;    //单片机将DQ拉低
 	delay(80); //精确延时 大于 480us    //450
 	DATA = 1;    //拉高总线
 	delay(20);
 	x=DATA;      //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
 	delay(30);
}
ReadOneChar(void)  //读一个字节
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
 	{
  		DATA = 0; // 给脉冲信号
  		dat>>=1;
  		DATA = 1; // 给脉冲信号
  		if(DATA)
   		dat|=0x80;
  		delay(8);
 	}
 	return(dat);
}
WriteOneChar(unsigned char dat)	//写一个字节
{
 	uchar i=0;
 	for (i=8; i>0; i--)
 	{
  		DATA = 0;
  		DATA = dat&0x01;
  	delay(10);
  	DATA = 1;
  	dat>>=1;
 	}
	delay(8);
}
int ReadTemperature(void) //读取温度
{
	uchar a=0;
	uchar b=0;
	int t=0;
	float tt=0;
	Init_DS18B20();
	WriteOneChar(0xCC); // 跳过读序号列号的操作
	WriteOneChar(0x44); // 启动温度转换
	Init_DS18B20();
	WriteOneChar(0xCC); //跳过读序号列号的操作
	WriteOneChar(0xBE); //读取温度寄存器等（共可读9个寄存器） 前两个就是温度
	a=ReadOneChar();//低位
	b=ReadOneChar();//高位
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	t= tt*10+0.5; 
	return(t);
}
void  display00()     //*********显示负值子函数
{	
    dd=-(temp-1);
    buf[1]=dd/100;
	buf[2]=dd/100;
	buf[3]=dd%100/10;
	buf[0]=dd%10;
	//动态显示
	for(j=0;j<5;j++)
    {
	  P2=0xff;          			  // 初始灯为灭的
    P0=0x00;
		P2=0xfd; //显示小数点
		P0=0x80; //显示小数点
	delay(100);		
    P2=0xff;  // 初始灯为灭的
    P0=0x00;
    P2=0xf8; //片选LCD1
    P0=0x40;
	delay(100);
		P2=0xff;
    P0=0x00;
    P2=0xf4;  //片选LCD2
    P0=table[buf[2]];
	delay(100);
		P2=0xff;
    P0=0x00;
		P2=0Xf2;//片选LCD3
		P0=table[buf[3]];
	delay(100);
		P2=0xff;
    P0=0x00;
		P2=0Xf1;
		P0=table[buf[0]];		  //片选LCD4	
	delay(100);
	P2=0xff;   
    }
}
//显示正值子函数
void  display()
{		
	buf[1]=temp/1000;//显示百位
	buf[2]=temp/100%10;//显示十位
	buf[3]=temp%100/10;//显示个位
	buf[0]=temp%10; //小数位
	for(j=0;j<3;j++) 
	{
	P2=0xff;          			  // 初始灯为灭的
    P0=0x00;
	P2=0xfd; //显示小数点
	P0=0x80; //显示小数点
	delay(300);		
    P2=0xff;          			  // 初始灯为灭的
    P0=0x00;
    P2=0xf7;           			  //片选LCD1
    P0=table[buf[1]];
	delay(300);
 	P2=0xff;
    P0=0x00;
    P2=0xfb;             		 //片选LCD2
    P0=table[buf[2]];
	delay(300);
	P2=0xff;
    P0=0x00;
	P2=0Xfd;		  //片选LCD3
	P0=table[buf[3]];
	delay(300);
	P2=0xff;
    P0=0x00;
	P2=0Xfe;
	P0=table[buf[0]];		  //片选LCD4	
	delay(300);
	P2=0xff;
	}
}
void key()	//按键扫描子程序
{   if(k1!=1)
	{
	   delay(20);
	   if(k1!=1)
	   {
		while(k1!=1)
		  { key_to1();
		    for(n=0;n<8;n++)
		    Show();
	       }
	   }
	}
	if(k2!=1)
	{
		delay(20);
		if(k2!=1)
		{
		  while(k2!=1)
			{ key_to2(); 
			  for(n=0;n<8;n++)
		      Show();
			}
			
		}
	}	 
	if(k3!=1)
	{	TR0=1; //复位，开定时
        temp=ReadTemperature();
	}
	if(k4!=1)
	{  delay(20);
	   if(k4!=1)
	   {  while(k4!=1);
	      set=!set;
		   if(set==0)
		   { Red=0;Green=1;}
		   else { Green=0;Red=1;}
		}
	 }
}
void key_to1()
{
	    TR0=0;	       //关定时器
        temp+=10;
		if(temp>=1100)
		   {temp=-550;}
        if(set==0)
		{alarmH=temp;}
		else {alarmL=temp;}	
}
void key_to2()
{
	    TR0=0;
       //关定时器
        temp-=10;
		if(temp<=-550)
		    {temp=1100;}
        if(set==0)
		{ alarmH=temp;}
		else { alarmL=temp;}
}

void alarm(void)
{
    if(temp>alarmH||temp<alarmL) 
	{   //bell=1;
	    //delay(50);
		//bell=0;
		Flag=1;
     }else {Flag=0;}
}

logo()//开机的Logo  
{   P0=0x40;

    P2=0xf7;
	delay(50);
    P2=0xfb;
	delay(50);
	P2=0Xfd;
	delay(50);
	P2=0Xfe;
	delay(50);
    P1 = 0xff;         //关闭显示
}

void Show()  //显示函数,分别表示温度正负值
{	    if(temp>=0)
		{HLight=1;LLight=0;display();}
		if(temp<0)
		{HLight=0;LLight=1;display00();}
}
void  main()
{	
    TCON=0x01; //定时器T0工作在01模式下
    TMOD=0X01;	
	TH0=0XD8;//装入初值
	TL0=0XF0;
	EA=1;  //开总中断
	ET0=1; //开T0中断
	TR0=1; //T0开始运行计数
	EX0=1; //开外部中断0
	
	for(n=0;n<500;n++)//显示启动LOGo"- - - -"
	{bell=1;warn=1;logo();}
	 Red=0;
	while(1)
	{
	    key(); 
		ss=ReadTemperature();
        Show();
		alarm();   //报警函数
		if(Flag==1)
		{bell=!bell;
		 warn=!warn;}  //蜂鸣器滴滴响
		else {bell=1;
		      warn=1;}	
	}	 
}
 void time0(void) interrupt 1 using 1  //每隔10ms执行一次此子程序
 {	TH0=0X56;
	TL0=0XDC;	
	temp=ss;
 }