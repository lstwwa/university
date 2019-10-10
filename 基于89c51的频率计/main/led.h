/*************************************************************************
*名称：频率计
*说明：实现0~5000hz频率计的统计
*作者：季浩原
*时间：11-8
*学号：1712220203
*数码管驱动
*************************************************************************/
#include "Include.h"
#ifndef LED_H
#define LED_H  1
  #ifndef LED_GLOBAL
    #define LED_EXT extern 
  #else
    #define LED_EXT 
  #endif
  #define  LED P0	        					   	//数码管数据端口配置
  sbit LED_CS1 =P2^6;                   //数码管片选端口配置 
  sbit LED_CS2 =P2^5;
  sbit LED_CS3 =P2^4;
  sbit LED_CS4 =P2^7;
  enum SEG_DISPLAY_MOD{DIGITAL=0x00,CHARACTER=0x01};
  //共阴极显示方式	  
  #ifdef LED_GLOBAL
 // uchar segLedCode[10]={0x7e,0x06,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x73};                                      //11111111111
	//uchar segLedCode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};           //2222222222222
	 //uchar segLedCode[10]={0x73,0x7f,0x70,0x5f,0x5b,0x33,0x79,0x6d,0x06,0x7e};  
  //uchar segLedCode[10]={0xb7,0x24,0xba,0X9B,0X0F,0X9D,0XBD,0X83,0XBF,0X8F}; 	 
  // uchar segLedCode[10]={0X6F,0X48,0X75,0X37,0X1E,0X3B,0X7B,0X07,0X7F,0X1F};	//333333333
	//uchar segLedCode[10]={0X77,0X24,0X7A,0X5B,0X0F,0X5D,0X7D,0X43,0X7F,0X4F};
	
	
	//uchar segLedCode[10]={0XFE,0XFE,0XFE,0XFE,0XFE,0XFE,0XFE,0XFE,0XFE,0XFE};    //H
	//uchar segLedCode[10]={0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD,0XFD};    //G 
  //uchar segLedCode[10]={0XFB,0XFB,0XFB,0XFB,0XFB,0XFB,0XFB,0XFB,0XFB,0XFB};  //F	
	//uchar segLedCode[10]={0XF7,0XF7,0XF7,0XF7,0XF7,0XF7,0XF7,0XF7,0XF7,0XF7};     //E
	//uchar segLedCode[10]={0XEF,0XEF,0XEF,0XEF,0XEF,0XEF,0XEF,0XEF,0XEF,0XEF};  //D
	//uchar segLedCode[10]={0XDF,0XDF,0XDF,0XDF,0XDF,0XDF,0XDF,0XDF,0XDF,0XDF}; 	//C
	//uchar segLedCode[10]={0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF,0XBF};//B
	//uchar segLedCode[10]={0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F,0X7F};//A
	//uchar segLedCode[10]={0XFA,0XFA,0XFA,0XFA,0XFA,0XFA,0XFA,0XFA,0XFA,0XFA};  //0 
	//uchar segLedCode[10]={0XFA,0X22,0XB9,0XAB,0X63,0XCB,0XDB,0XA2,0XFB,0XEB};//-MIANBAOBAN----------------------------------
	//uchar segLedCode[10]={0XF9,0X81,0XBA,0XAB,0XC3,0X6B,0X7B,0XA1,0XFB,0XEB};
	uchar segLedCode[10]={0X5F,0X41,0X9D,0XD5,0XC3,0XD6,0XDE,0X45,0XDF,0XD7};
	 //数码管数码译码数组,led6~led0==a~g,code1~10==0~9
  #endif
  LED_EXT void init_led(void);
  LED_EXT void ledWrite(uchar c,uchar n,uchar mod);
  //循环显示子函数，segCode为要显示的数据组，segCodeMode为要相应数据的显示模式
  LED_EXT void ledSweepDisplay(uchar *segCode,uchar segCodeMod,uchar segNumber);	

#endif