/*************************************************************************
*名称：频率计
*说明：实现0~5000hz频率计的统计
*作者：季浩原
*时间：11-8
*学号：1712220203
数码管
*************************************************************************/
#define LED_GLOBAL 1
#include "led.h"
/*******************************************************************************************
delay_us(uint us)
*******************************************************************************************/
void delay_us(uint us)
{
  uint i;
  while(us--)
  {
    for(i=0;i<100;i++);
  }
}
/*******************************************************************************************
led_init(void)
*******************************************************************************************/
void init_led(void)
{
   LED=segLedCode[0];
   LED_CS1=1;
   LED_CS2=1;
   LED_CS3=1;
   LED_CS4=1;
}
/*******************************************************************************************
ledWrite(uchar c,uchar n,uchar mod)
当mod==DIGITAL时,c为数字0~9的ANCII码形式，此时在n位数码管上显示想用数字
当mod==CHARACTER,直接将c输出数据口
*******************************************************************************************/
void ledWrite(uchar c,uchar n,uchar mod)
{
   if(CHARACTER==mod)
   {
	  LED=c;  
   }
   else if(DIGITAL==mod)
	   {
	      if((c-'0')<0x0a)
		  {
		    LED=segLedCode[c-'0'];
		  } 
		  else
		  {  
			LED=segLedCode[0];
		  }
	   }
   switch(n)
   {
		case 0x01:
		{
		  LED_CS1=0;
		  delay_us(4);
		  LED_CS1=1;
		  break;
		}
		case 0x02:
		{
		  LED_CS2=0;
		  delay_us(4);
		  LED_CS2=1;
		  break;
		}
		case 0x03:
		{
		  LED_CS3=0;
		  delay_us(4);
		  LED_CS3=1;
		  break;
		}
	    case 0x04:
		{
		  LED_CS4=0;
		  delay_us(4);
		  LED_CS4=1; 
		  break;
		}
		default:
		  break;

   }

}
/*******************************************************************************************
ledSweepDisplay(uchar *segCode,uchar segCodeMod,uchar segNumber)
*******************************************************************************************/	   	  
void ledSweepDisplay(uchar *segCode,uchar segCodeMod,uchar segNumber)
{
   	uchar segCount;
	for(segCount=0;segCount<segNumber;segCount++)
	{
	   if(segCodeMod&0x01)	   /*1代表此处显示任意字符 */
	   {
	     ledWrite(segCode[segCount],segCount+1,CHARACTER); 
	   }
	   else
	   {
	     ledWrite(segCode[segCount],segCount+1,DIGITAL);
	   }
	   segCodeMod=segCodeMod>>1;
	}
}

