#include <reg52.H>//Æ÷¼þÅäÖÃÎÄ¼þ
#include <intrins.h>
/*************************´«¸ÐÆ÷½Ó¿Ú****************************/
sbit RX  = P3^2;
sbit TX  = P3^3;
/**************************ÏÔÊ¾Ñ¡Æ¬*****************************/
sbit W1=P2^0;
sbit W2=P2^1;
sbit W3=P2^2;
sbit W4=P2^3;
/**************************±äÁ¿ÉùÃ÷******************************/
unsigned int  time=0;
unsigned int  timer=0;
unsigned char posit=0;
unsigned long S=0;
unsigned long BJS=50;//±¨¾¯¾àÀë80CM
/********************Ä£Ê½ 0Õý³£Ä£Ê½ 1µ÷Õû*************************/
char Mode=0;
bit  flag=0;
bit flag_KEY=0;
/******************************   0    1    2    3    4    5    6    7    8    9    -    ¿Õ**************/
unsigned char const discode[] ={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40,0xff};	
unsigned char disbuff[4]	   ={0,0,0,0};		 //Êý×éÓÃÓÚ´æ·Å¾àÀëÐÅÏ¢
unsigned char disbuff_BJ[4]	={0,0,0,0};//±¨¾¯ÐÅÏ¢
void Display();//ÑÓÊ±20ms£¨²»¾«È·£©
void delay(void)
{
    unsigned char a,b,c;
    for(c=2;c>0;c--)
        for(b=38;b>0;b--)
            for(a=60;a>0;a--);
}
/**********************************************************************************************************/Ü
void Display(void)				 
{
	if(Mode==0)
	{
		P0=0x00;  //¹Ø±ÕÏÔÊ¾
		if(posit==1)//ÊýÂë¹ÜµÄÐ¡Êýµã
		{
			P0=(discode[disbuff[posit]])|0x80;//°´Î»»ò£¬×î¸ßÎ»±äÎª1£¬ÏÔÊ¾Ð¡Êýµã
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
		if(posit>3)		//Ã¿½øÒ»´ÎÏÔÊ¾º¯Êý£¬±äÁ¿¼Ó1
			posit=0;		//¼Óµ½3Ê±ÇåÁã
	}
	//±¨¾¯ÏÔÊ¾
	else
	{
		P0=0x00;
		if(posit==1)//ÊýÂë¹ÜµÄÐ¡Êýµã
		{
			P0=(discode[disbuff_BJ[posit]])|0x80;
		}
		else if(posit==0)
		{
			P0=0x76;	//ÏÔÊ¾×ÖÄ¸		
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
//¼ÆËã
void Conut(void)
{
	time=TH0*256+TL0;	  //¶Á³öT0µÄ¼ÆÊ±ÊýÖµ
	TH0=0;
	TL0=0;				  //Çå¿Õ¼ÆÊ±Æ÷
	S=(time*1.7)/100;     //Ëã³öÀ´ÊÇCM
	//ÉùÒôµÄËÙ¶ÈÊÇ340m/s£¬Ê±¼äµÄµ¥Î»ÊÇus£¬¼ÆËãµ½ÃëÐèÒª½«Ê±¼äÊý¾Ý/1000000£¬
	//³¤¶È=ËÙ¶È*Ê±¼ä£¬340*time/1000000£¬³¤¶ÈÊý¾Ýµ¥Î»ÊÇm×ª»»³ÉcmÐèÒª³ËÒÔ100µÃµ½340*time/10000£¬
	//Ð¡Êýµã¶¼Ïò×óÒÆÁ½Î»µÃµ½3.4*time/100£¬ÒòÎª³¬Éù²¨ÊÇÍù·µÁË£¬ËùÒÔÔÙ³ýÒÔ2,µÃµ½¾àÀëÊý¾Ý(time*1.7)/100
	if(Mode==0)			  //·ÇÉèÖÃ×´Ì¬Ê±
	{
		if((S>=700)||flag==1) //³¬³ö²âÁ¿·¶Î§ÏÔÊ¾¡°-¡±
		{	
			flag=0;
			disbuff[1]=10;	   //¡°-¡±
			disbuff[2]=10;	   //¡°-¡±
			disbuff[3]=10;	   //¡°-¡±
		}
		else
		{
			disbuff[1]=S%1000/100;		 //½«¾àÀëÊý¾Ý²ð³Éµ¥¸öÎ»¸³Öµ
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
//¶¨Ê±Æ÷0
void zd0() interrupt 1 		 //T0ÖÐ¶ÏÓÃÀ´¼ÆÊýÆ÷Òç³ö,³¬¹ý²â¾à·¶Î§
{
	flag=1;						 //ÖÐ¶ÏÒç³ö±êÖ¾
}
/**********************************************************************************************************/
//¶¨Ê±Æ÷1
void zd3() interrupt 3 		 //T1ÖÐ¶ÏÓÃÀ´É¨ÃèÊýÂë¹ÜºÍ¼Æ800MSÆô¶¯Ä£¿é
{
	TH1=0xf8;
	TL1=0x30;				 //¶¨Ê±2ms
	Display();				//É¨ÃèÏÔÊ¾
	timer++;				//±äÁ¿¼Ó
	if(timer>=400)			//400´Î¾ÍÊÇ800ms
	{
		timer=0;
		TX=1;			          //800MS  Æô¶¯Ò»´ÎÄ£¿é
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
//Ö÷º¯Êý
void main(void)
{  
	TMOD=0x11;		   //ÉèT0Îª·½Ê½1
	TH0=0;
	TL0=0;          
	TH1=0xf8;		   //2MS¶¨Ê±
	TL1=0x30;
	ET0=1;				//ÔÊÐíT0ÖÐ¶Ï
	ET1=1;			   //ÔÊÐíT1ÖÐ¶Ï
	TR1=1;			   //¿ªÆô¶¨Ê±Æ÷
	EA=1;					//¿ªÆô×ÜÖÐ¶Ï	
	while(1)
	{
		while(!RX);		//µ±ÉÏ´Î½ÓÊÕÍê²¨ºó£¬RXÒý½ÅÊÇµÍµçÆ½£¬È¡·´¾ÍÊÇ1£¬´Ëwhile³ÉÁ¢£¬·´¸´ÅÐ¶ÏRX×´Ì¬¡£µ±RXÃ»ÓÐ½ÓÊÕµ½·µ»Ø²¨Ê±ÊÇ¸ßµçÆ½£¬È¡·´¾ÍÊÇ0£¬´Ëwhile²»³ÉÁ¢£¬Ìø³ö
		TR0=1;			//¿ªÆô¼ÆÊý
		while(RX);		//µ±RXÃ»ÓÐ½ÓÊÕµ½·µ»Ø²¨£¬´Ëwhile³ÉÁ¢£¬³ÌÐòÍ£ÔÚÕâÀïÒ»Ö±ÅÐ¶ÏRX×´Ì¬¡£µ±RX½ÓÊÕµ½·µ»Ø²¨£¬RXÒý½Å±äÎªµÍµçÆ½£¬´Ëwhile²»³ÉÁ¢£¬Ìø³ö
		TR0=0;			//Í£Ö¹¼ÆÊý
		Conut();			//¼ÆËã
	}
}
             