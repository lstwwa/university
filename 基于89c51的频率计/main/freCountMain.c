/*************************************************************************
*���ƣ�Ƶ�ʼ�
*˵����ʵ��0~5000hzƵ�ʼƵ�ͳ��
*���ߣ�����ԭ
*ʱ�䣺11-8
*ѧ�ţ�1712220203
*������
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
uchar displayBuff[4]={'0','0','0','0'}; //�������ʾ������,ǧ,��,ʮ,��
const uchar displayMode=0x00;           //����λ�ֱ��Ӧ��������ÿλ����������,1Ϊ�����ַ���ʾ,0Ϊ������ʾ
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
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledunwork=1;
	ledwork=0;
	bee=1;
  }
	
	if(get_fre/1000>=0x01&&get_fre/1000<0x02)                                                                                 //1000-2000
	{
		get_fre=get_fre-85;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledunwork=1;
	ledwork=0;
	bee=1;
  }

	if(get_fre/1000>=0x02&&get_fre/1000<0x03)                                                               // 2000-3000
	{
		get_fre=get_fre-170;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
	if(get_fre/1000>=0x03&&get_fre/1000<0x04)                                                               // 3000-4000
	{
		get_fre=get_fre-255;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
		if(get_fre/1000>=0x04&&get_fre/1000<0x05)                                                               // 4000-5000
	{
		get_fre=get_fre-340;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=0;
	ledunwork=1;
	bee=0;
	}
	
		if(get_fre/1000>=0x05&&get_fre/1000<0x06)                                                               // 5000-6000
	{
		get_fre=get_fre-425;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x06&&get_fre/1000<0x07)                                                               // 6000-7000
	{
		get_fre=get_fre-510;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x07&&get_fre/1000<0x08)                                                               // 7000-8000
	{
		get_fre=get_fre-595;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
		if(get_fre/1000>=0x08&&get_fre/1000<0x09)                                                               // 8000-9000
	{
		get_fre=get_fre-680;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';          //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';		          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledwork=1;
	ledunwork=0;
	bee=0;
	}
	
	
	
	if(get_fre/1000>=0x09)                                                                                  //>9000
	{
	get_fre=get_fre-765;
	displayBuff[0]=get_fre/1000+'0';            //����Ƶ��ǧλ,���ڻ�������0λ
	displayBuff[1]=(get_fre/100)%10+'0';          //����Ƶ�ʰ�λ,���ڻ�������1λ
  displayBuff[2]=(get_fre%100)/10+'0';         //����Ƶ��ʮλ,���ڻ�������2λ
	displayBuff[3]=get_fre%10+'0';	          //����Ƶ�ʸ�λ,���ڻ�������3λ
  ledSweepDisplay(displayBuff,displayMode,0x04);//ѭ����ʾ����ܻ�����������
	ledunwork=1;
	ledwork=0;
	bee=0;
	}
	


	}	

}

