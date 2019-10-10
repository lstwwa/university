/*************************************************************************
*名称：频率计
*说明：实现0~5000hz频率计的统计
*作者：季浩原
*时间：11-8
*学号：1712220203
*主程序
*************************************************************************/
#include "target.h"
#include "led.h"
//#define delay_time 25000
sbit bee=P2^1;
sbit ledunwork=P2^3;
sbit ledwork=P2^2;
void INIT(void)
{

}
uchar displayBuff[4]={'0','0','0','0'}; //数码管显示缓冲区,千,百,十,个
const uchar displayMode=0x00;           //后四位分别对应缓冲区中每位的数据类型,1为任意字符显示,0为数字显示
void main()
{
  uint get_fre;
  target_init();
  init_led();
  sei();
  while(1)
  {
    get_fre=get_fre_number();
		
	if(get_fre/1000<0x01)                                                                                    //0-1000
	{
	get_fre=get_fre;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledunwork=1;
	ledwork=0;
	bee=1;
  }
	
	if(get_fre/1000>=0x01&&get_fre/1000<0x02)                                                                                 //1000-2000
	{
		get_fre=get_fre-85;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledunwork=1;
	ledwork=0;
	bee=1;
  }

	if(get_fre/1000>=0x02&&get_fre/1000<0x03)                                                               // 2000-3000
	{
		get_fre=get_fre-170;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
	if(get_fre/1000>=0x03&&get_fre/1000<0x04)                                                               // 3000-4000
	{
		get_fre=get_fre-255;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
		if(get_fre/1000>=0x04&&get_fre/1000<0x05)                                                               // 4000-5000
	{
		get_fre=get_fre-340;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
		if(get_fre/1000>=0x05&&get_fre/1000<0x06)                                                               // 5000-6000
	{
		get_fre=get_fre-425;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x06&&get_fre/1000<0x07)                                                               // 6000-7000
	{
		get_fre=get_fre-510;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x07&&get_fre/1000<0x08)                                                               // 7000-8000
	{
		get_fre=get_fre-595;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x08&&get_fre/1000<0x09)                                                               // 8000-9000
	{
		get_fre=get_fre-680;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';          //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';		          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
	
	
	if(get_fre/1000>=0x09)                                                                                  //>9000
	{
	get_fre=get_fre-765;
	displayBuff[0]=get_fre/1000+'0';            //计算频率千位,放在缓冲区第0位
	displayBuff[1]=(get_fre/100)%10+'0';          //计算频率百位,放在缓冲区第1位
  displayBuff[2]=(get_fre%100)/10+'0';         //计算频率十位,放在缓冲区第2位
	displayBuff[3]=get_fre%10+'0';	          //计算频率个位,放在缓冲区第3位
  ledSweepDisplay(displayBuff,displayMode,0x04);//循环显示数码管缓冲区中数据
	ledunwork=1;
	ledwork=0;
	bee=0;
	}
	


	}	

}

