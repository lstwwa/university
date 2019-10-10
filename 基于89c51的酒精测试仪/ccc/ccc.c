#include <reg52.h>	         //调用单片机头文件
#define uchar unsigned char  //无符号字符型 宏定义	变量范围0~255
#define uint  unsigned int	 //无符号整型 宏定义	变量范围0~65535
uchar a_a;
#include <intrins.h>
#define pulse0832()_nop_();_nop_();CLK=1;_nop_();_nop_();CLK=0


//数码管段选定义      0     1    2    3    4    5	 6	 7	  8	   9	
uchar code smg_du[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};	 //断码

//数码管位选定义
uchar code smg_we[]={0x7F,0xBF,0xDF,0xEF};
uchar dis_smg[]  = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};	
uchar smg_i = 4;    //显示数码管的个位数

sbit CS=P3^2;		//CS定义为P1口的第4位脚，连接ADC0832CS脚
sbit CLK=P3^3;		//CL定义为P1口的第3位脚，连接ADC0832SCL脚
sbit DI=P3^4;
sbit DO=P3^4;		//DO定义为P1口的第4位脚，连接ADC0832DO脚

sbit beep = P3^6;   //蜂鸣器IO口定义
long dengji,s_dengji ;     //酒精等级

bit flag_300ms = 1;
uchar menu_1;        //菜单设计的变量

#define RdCommand 0x01 //定义ISP的操作命令
#define PrgCommand 0x02
#define EraseCommand 0x03 
#define Error 1
#define Ok 0
#define WaitTime 0x01 //定义CPU的等待时间
sfr ISP_DATA=0xe2;  //寄存器申明
sfr ISP_ADDRH=0xe3;
sfr ISP_ADDRL=0xe4;											 
sfr ISP_CMD=0xe7;
sfr ISP_TRIG=0xe6;
sfr ISP_CONTR=0xe5;

/* ================ 打开 ISP,IAP 功能 ================= */
void ISP_IAP_enable(void)
{
	 EA = 0;       /* 关中断   */
	 ISP_CONTR = ISP_CONTR & 0x18;       /* 0001,1000 */
	 ISP_CONTR = ISP_CONTR | WaitTime; /* 写入硬件延时 */
	 ISP_CONTR = ISP_CONTR | 0x80;       /* ISPEN=1  */
}
/* =============== 关闭 ISP,IAP 功能 ================== */
void ISP_IAP_disable(void)
{
	 ISP_CONTR = ISP_CONTR & 0x7f; /* ISPEN = 0 */
	 ISP_TRIG = 0x00;
	 EA   =   1;   /* 开中断 */
}
/* ================ 公用的触发代码 ==================== */
void ISPgoon(void)
{
	 ISP_IAP_enable();   /* 打开 ISP,IAP 功能 */
	 ISP_TRIG = 0x46;  /* 触发ISP_IAP命令字节1 */
	 ISP_TRIG = 0xb9;  /* 触发ISP_IAP命令字节2 */
	 _nop_();
}
/* ==================== 字节读 ======================== */
unsigned char byte_read(unsigned int byte_addr)
{
	EA = 0;
	 ISP_ADDRH = (unsigned char)(byte_addr >> 8);/* 地址赋值 */
	 ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff);
	 ISP_CMD   = ISP_CMD & 0xf8;   /* 清除低3位  */
	 ISP_CMD   = ISP_CMD | RdCommand; /* 写入读命令 */
	 ISPgoon();       /* 触发执行  */
	 ISP_IAP_disable();    /* 关闭ISP,IAP功能 */
	 EA  = 1;
	 return (ISP_DATA);    /* 返回读到的数据 */
}
/* ================== 扇区擦除 ======================== */
void SectorErase(unsigned int sector_addr)
{
	 unsigned int iSectorAddr;
	 iSectorAddr = (sector_addr & 0xfe00); /* 取扇区地址 */
	 ISP_ADDRH = (unsigned char)(iSectorAddr >> 8);
	 ISP_ADDRL = 0x00;
	 ISP_CMD = ISP_CMD & 0xf8;   /* 清空低3位  */
	 ISP_CMD = ISP_CMD | EraseCommand; /* 擦除命令3  */
	 ISPgoon();       /* 触发执行  */
	 ISP_IAP_disable();    /* 关闭ISP,IAP功能 */
}
/* ==================== 字节写 ======================== */
void byte_write(unsigned int byte_addr, unsigned char original_data)
{
	 EA  = 0;
	 SectorErase(byte_addr);
	 ISP_ADDRH = (unsigned char)(byte_addr >> 8);  /* 取地址  */
	 ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff);
	 ISP_CMD  = ISP_CMD | PrgCommand;  /* 写命令2 */
	 ISP_DATA = original_data;   /* 写入数据准备 */
	 ISPgoon();       /* 触发执行  */
	 ISP_IAP_disable();     /* 关闭IAP功能 */
	 EA =1;
}


/***********************1ms延时函数*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}

/******************把数据从单片机内部eeprom中读出来*****************/
void read_eeprom()	 //读出保存数据
{
	s_dengji  = byte_read(0x2001);
	s_dengji <<= 8;
	s_dengji  |= byte_read(0x2000);
	a_a      = byte_read(0x2058);
}

/******************把数据保存到单片机内部eeprom中******************/
void write_eeprom()	//保存数据
{
	SectorErase(0x2000);
	byte_write(0x2000, s_dengji % 256);
	byte_write(0x2001, s_dengji / 256);
	byte_write(0x2058,a_a);	
}

/**************开机自检eeprom初始化*****************/
void init_eeprom()	 ////开始初始化保存的数据
{
	read_eeprom();	   //读出保存数据
	if(a_a != 33)
	{
		a_a = 33;
		s_dengji = 80;
		write_eeprom();		 //保存数据
	}
}

/***********读数模转换数据********************************************************/	
//请先了解ADC0832模数转换的串行协议，再来读本函数，主要是对应时序图来理解，本函数是模拟0832的串行协议进行的
						//  1  0  0 通道
						//  1  1  1 通道 
unsigned char ad0832read()
{
	unsigned char i=0,ch=0,ch1=0;		
		CS=0;
		DI=1;
	pulse0832();		//开始
		DI=1;
		pulse0832();		//第一个上升沿	
		DI=0;
		pulse0832();
		DI=1;	    //第三个下降沿
		for(i=0;i<8;i++)
		{
			pulse0832(); //开始从第四个下降沿接收数据
			ch<<=1;
			if(DO==1)
				ch|=0x01;						
		}
		for(i=0;i<8;i++)
		{			//接收校验数据
			ch1>>=1;
			if(DO==1)
				ch1|=0x80;
			pulse0832();
		}
		CS=1;	
		return(ch==ch1)?ch:0;				//与校验数据比较，正确就返回数据，否则返回0	
	

}

/********************独立按键程序*****************/
uchar key_can;	 //按键值

void key()	 //独立按键程序
{
	static uchar key_new;
	key_can = 20;                   //按键值还原
	P2 |= 0x0f;					//把按键的IO口输出为高电平
	if((P2 & 0x0f) != 0x0f)		//按键按下
	{
		delay_1ms(1);	     	//按键消抖动
		if(((P2 & 0x0f) != 0x0f) && (key_new == 1))
		{						//确认是按键按下
			key_new = 0;
			switch(P2 & 0x0f)
			{
				case 0x0e: key_can = 3; break;	   //得到k1键值
				case 0x0d: key_can = 2; break;	   //得到k2键值
				case 0x0b: key_can = 1; break;	   //得到k3键值
//				case 0x07: key_can = 4; break;	   //得到k4键值
			}
		}			
	}
	else 
		key_new = 1;	
}



/****************按键处理数码管显示函数***************/
void key_with()
{
	if(key_can == 1)		   //设置键
	{
		menu_1 ++;
		if(menu_1 >= 2)
		{
			menu_1 = 0;
		}
	}
	if(menu_1 == 1)			//设置酒精报警值
	{
		smg_i = 4;		    //显示4位数码管
		if(key_can == 2)
		{
			s_dengji ++ ;		 //加1 
			if(s_dengji > 500)
				s_dengji = 500;
		}
		if(key_can == 3)
		{
			s_dengji -- ;		//减1	
			if(s_dengji <= 1)
				s_dengji = 1;
		}
		dis_smg[0] = smg_du[s_dengji % 10];	           //取个位显示
		dis_smg[1] = smg_du[s_dengji / 10 % 10] ;      //取十位显示
		dis_smg[2] = smg_du[s_dengji / 100 % 10] ;	   //
		dis_smg[3] = 0x05;	 //a
		write_eeprom();	//保存数据
	}	
}  


/***********************数码显示函数*****************************/
void display()
{
	static uchar i;	
	P1 = 0x00;			 //消隐 				          
	P2 = smg_we[i];	 		 //位选
	P1 = dis_smg[i];		 //段选	   
	i ++;
	if(i >= smg_i)
		i = 0;  	
}


/*************定时器0初始化程序***************/
void time_init()	  
{
	EA   = 1;	 	  //开总中断
	TMOD = 0X01;	  //定时器0、定时器1工作方式1
	ET0  = 1;		  //开定时器0中断 
	TR0  = 1;		  //允许定时器0定时
}



/****************报警函数***************/
void clock_h_l()
{
	static uchar value;
	if((dengji >= s_dengji))		//报警
	{
		value ++;
		if(value >= 2)
		{
			value = 10;
			beep = ~beep;	  //蜂鸣器报警
		}
	}else 
	{
		if((dengji < s_dengji))	  //取消报警
		{
			value = 0;
			beep = 1;
		}	
	}

}

/****************主函数***************/
void main()
{
	beep = 0;		                //开机叫一声   
	delay_1ms(150);
	P0 = P1 = P2 = P3 = 0xff;		//单片机IO口初始化为1
	time_init();				//初始化定时器 
	init_eeprom();              //开始初始化保存的数据
	while(1)
	{
		key();					//独立按键程序
		if(key_can < 10)
		{
			key_with();			//按键按下要执行的程序
		}
		if(flag_300ms == 1)
		{		
			flag_300ms = 0;
			clock_h_l();
			dengji = ad0832read();	
			dengji = dengji * 450 / 255.0;
		    dengji = dengji - 100;              //首先减去零点漂移，一般是100mV
			if(dengji < 0)
			{
				dengji = 0;	
			} 
			
			dengji = dengji * 2;             //将mV转变成mg/L，系数需要校准   
								  //电压每升高0.1V，实际被测气体的浓度增加20ppm 
								  //1ppm=1mg/kg=1mg/L=1×10-6 常用来表示气体浓度，或者溶液浓度。       
			if(menu_1 == 0)
			{
				if(dengji >= 1000)
					smg_i = 4;
				else 
					smg_i = 3;
				dis_smg[3]=smg_du[dengji/1000%10];	//千位
				dis_smg[2]=smg_du[dengji/100%10];	//百位
				dis_smg[1]=smg_du[dengji/10%10];	//十位
				dis_smg[0]=smg_du[dengji%10];	    //个位	ADC0832为8位ADC，数值为0~255，我们将其分开放入l_tmpdate数组中显示
			}
		} 
	}
}

/*************定时器0中断服务程序***************/
void time0_int() interrupt 1
{	
	static uchar value;
	TH0 = 0xf8;
	TL0 = 0x30;     // 2ms
	display();
	value ++;	 
	if(value % 150 == 0)
	{
		flag_300ms = 1;	   //300ms
		value = 0;					                                                  
	}

}