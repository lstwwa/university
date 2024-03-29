#include <reg52.H>//器件配置文件
#include <intrins.h>
/*************************传感器接口****************************/
sbit RX  = P3^2;
sbit TX  = P3^3;
/**************************显示选片*****************************/
sbit W1=P2^0;
sbit W2=P2^1;
sbit W3=P2^2;
sbit W4=P2^3;
/**************************变量声明******************************/
unsigned int  time=0;
unsigned int  timer=0;
unsigned char posit=0;
unsigned long S=0;
unsigned long BJS=50;//报警距离80CM
/********************模式 0正常模式 1调整*************************/
char Mode=0;
bit  flag=0;
bit flag_KEY=0;
/******************************   0    1    2    3    4    5    6    7    8    9    -    空**************/
unsigned char const discode[] ={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xff};	
unsigned char disbuff[4]	   ={0,0,0,0};		 //数组用于存放距离信息
unsigned char disbuff_BJ[4]	={0,0,0,0};//报警信息
void Display();//延时20ms（不精确）
void delay(void)
{
    unsigned char a,b,c;
    for(c=2;c>0;c--)
        for(b=38;b>0;b--)
            for(a=60;a>0;a--);
}
/**********************************************************************************************************/�
void Display(void)				 
{
	if(Mode==0)
	{
		P0=0x00;  //关闭显示
		if(posit==1)//数码管的小数点
		{
			P0=(discode[disbuff[posit]])|0x80;//按位或，最高位变为1，显示小数点
		}
		else if(posit==0)
		{
			P0=~discode[11];
		}
		else
		{
			P0=discode[disbuff[posit]];
		}
		switch(posit)
		{
			case 0 : W1=0;W2=1;W3=1;W4=1; break;
			case 1 : W1=1;W2=0;W3=1;W4=1; break;
			case 2 : W1=1;W2=1;W3=0;W4=1; break;
			case 3 : W1=1;W2=1;W3=1;W4=0; break;
		}
		posit++;
		if(posit>3)		//每进一次显示函数，变量加1
			posit=0;		//加到3时清零
	}
	//报警显示
	else
	{
		P0=0x00;
		if(posit==1)//数码管的小数点
		{
			P0=(discode[disbuff_BJ[posit]])|0x80;
		}
		else if(posit==0)
		{
			P0=0x76;	//显示字母		
		}
		else
		{
			P0=discode[disbuff_BJ[posit]];
		}
		switch(posit)
		{
			case 0 : W1=0;W2=1;W3=1;W4=1; break;
			case 1 : W1=1;W2=0;W3=1;W4=1; break;
			case 2 : W1=1;W2=1;W3=0;W4=1; break;
			case 3 : W1=1;W2=1;W3=1;W4=0; break;
		}
		posit++;
		if(posit>3)
			posit=0;
	}
}
/**********************************************************************************************************/
//计算
void Conut(void)
{
	time=TH0*256+TL0;	  //读出T0的计时数值
	TH0=0;
	TL0=0;				  //清空计时器
	S=(time*1.7)/100;     //算出来是CM
	//声音的速度是340m/s，时间的单位是us，计算到秒需要将时间数据/1000000，
	//长度=速度*时间，340*time/1000000，长度数据单位是m转换成cm需要乘以100得到340*time/10000，
	//小数点都向左移两位得到3.4*time/100，因为超声波是往返了，所以再除以2,得到距离数据(time*1.7)/100
	if(Mode==0)			  //非设置状态时
	{
		if((S>=700)||flag==1) //超出测量范围显示“-”
		{	
			flag=0;
			disbuff[1]=10;	   //“-”
			disbuff[2]=10;	   //“-”
			disbuff[3]=10;	   //“-”
		}
		else
		{
			disbuff[1]=S%1000/100;		 //将距离数据拆成单个位赋值
			disbuff[2]=S%1000%100/10;
			disbuff[3]=S%1000%10 %10;
		}
	}
	else
	{
			disbuff_BJ[1]=BJS%1000/100;
			disbuff_BJ[2]=BJS%1000%100/10;
			disbuff_BJ[3]=BJS%1000%10 %10;
	}
}
/**********************************************************************************************************/
//定时器0
void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
{
	flag=1;						 //中断溢出标志
}
/**********************************************************************************************************/
//定时器1
void zd3() interrupt 3 		 //T1中断用来扫描数码管和计800MS启动模块
{
	TH1=0xf8;
	TL1=0x30;				 //定时2ms
	Display();				//扫描显示
	timer++;				//变量加
	if(timer>=400)			//400次就是800ms
	{
		timer=0;
		TX=1;			          //800MS  启动一次模块
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_();
		_nop_(); 
		_nop_(); 
		_nop_(); 
		_nop_();
		TX=0;
	} 
}
/**********************************************************************************************************/
//主函数
void main(void)
{  
	TMOD=0x11;		   //设T0为方式1
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2MS定时
	TL1=0x30;
	ET0=1;				//允许T0中断
	ET1=1;			   //允许T1中断
	TR1=1;			   //开启定时器
	EA=1;					//开启总中断	
	while(1)
	{
		while(!RX);		//当上次接收完波后，RX引脚是低电平，取反就是1，此while成立，反复判断RX状态。当RX没有接收到返回波时是高电平，取反就是0，此while不成立，跳出
		TR0=1;			//开启计数
		while(RX);		//当RX没有接收到返回波，此while成立，程序停在这里一直判断RX状态。当RX接收到返回波，RX引脚变为低电平，此while不成立，跳出
		TR0=0;			//停止计数
		Conut();			//计算
	}
}
             