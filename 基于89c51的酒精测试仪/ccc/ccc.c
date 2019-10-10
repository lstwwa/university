#include <reg52.h>	         //���õ�Ƭ��ͷ�ļ�
#define uchar unsigned char  //�޷����ַ��� �궨��	������Χ0~255
#define uint  unsigned int	 //�޷������� �궨��	������Χ0~65535
uchar a_a;
#include <intrins.h>
#define pulse0832()_nop_();_nop_();CLK=1;_nop_();_nop_();CLK=0


//����ܶ�ѡ����      0     1    2    3    4    5	 6	 7	  8	   9	
uchar code smg_du[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};	 //����

//�����λѡ����
uchar code smg_we[]={0x7F,0xBF,0xDF,0xEF};
uchar dis_smg[]  = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71
};	
uchar smg_i = 4;    //��ʾ����ܵĸ�λ��

sbit CS=P3^2;		//CS����ΪP1�ڵĵ�4λ�ţ�����ADC0832CS��
sbit CLK=P3^3;		//CL����ΪP1�ڵĵ�3λ�ţ�����ADC0832SCL��
sbit DI=P3^4;
sbit DO=P3^4;		//DO����ΪP1�ڵĵ�4λ�ţ�����ADC0832DO��

sbit beep = P3^6;   //������IO�ڶ���
long dengji,s_dengji ;     //�ƾ��ȼ�

bit flag_300ms = 1;
uchar menu_1;        //�˵���Ƶı���

#define RdCommand 0x01 //����ISP�Ĳ�������
#define PrgCommand 0x02
#define EraseCommand 0x03 
#define Error 1
#define Ok 0
#define WaitTime 0x01 //����CPU�ĵȴ�ʱ��
sfr ISP_DATA=0xe2;  //�Ĵ�������
sfr ISP_ADDRH=0xe3;
sfr ISP_ADDRL=0xe4;											 
sfr ISP_CMD=0xe7;
sfr ISP_TRIG=0xe6;
sfr ISP_CONTR=0xe5;

/* ================ �� ISP,IAP ���� ================= */
void ISP_IAP_enable(void)
{
	 EA = 0;       /* ���ж�   */
	 ISP_CONTR = ISP_CONTR & 0x18;       /* 0001,1000 */
	 ISP_CONTR = ISP_CONTR | WaitTime; /* д��Ӳ����ʱ */
	 ISP_CONTR = ISP_CONTR | 0x80;       /* ISPEN=1  */
}
/* =============== �ر� ISP,IAP ���� ================== */
void ISP_IAP_disable(void)
{
	 ISP_CONTR = ISP_CONTR & 0x7f; /* ISPEN = 0 */
	 ISP_TRIG = 0x00;
	 EA   =   1;   /* ���ж� */
}
/* ================ ���õĴ������� ==================== */
void ISPgoon(void)
{
	 ISP_IAP_enable();   /* �� ISP,IAP ���� */
	 ISP_TRIG = 0x46;  /* ����ISP_IAP�����ֽ�1 */
	 ISP_TRIG = 0xb9;  /* ����ISP_IAP�����ֽ�2 */
	 _nop_();
}
/* ==================== �ֽڶ� ======================== */
unsigned char byte_read(unsigned int byte_addr)
{
	EA = 0;
	 ISP_ADDRH = (unsigned char)(byte_addr >> 8);/* ��ַ��ֵ */
	 ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff);
	 ISP_CMD   = ISP_CMD & 0xf8;   /* �����3λ  */
	 ISP_CMD   = ISP_CMD | RdCommand; /* д������� */
	 ISPgoon();       /* ����ִ��  */
	 ISP_IAP_disable();    /* �ر�ISP,IAP���� */
	 EA  = 1;
	 return (ISP_DATA);    /* ���ض��������� */
}
/* ================== �������� ======================== */
void SectorErase(unsigned int sector_addr)
{
	 unsigned int iSectorAddr;
	 iSectorAddr = (sector_addr & 0xfe00); /* ȡ������ַ */
	 ISP_ADDRH = (unsigned char)(iSectorAddr >> 8);
	 ISP_ADDRL = 0x00;
	 ISP_CMD = ISP_CMD & 0xf8;   /* ��յ�3λ  */
	 ISP_CMD = ISP_CMD | EraseCommand; /* ��������3  */
	 ISPgoon();       /* ����ִ��  */
	 ISP_IAP_disable();    /* �ر�ISP,IAP���� */
}
/* ==================== �ֽ�д ======================== */
void byte_write(unsigned int byte_addr, unsigned char original_data)
{
	 EA  = 0;
	 SectorErase(byte_addr);
	 ISP_ADDRH = (unsigned char)(byte_addr >> 8);  /* ȡ��ַ  */
	 ISP_ADDRL = (unsigned char)(byte_addr & 0x00ff);
	 ISP_CMD  = ISP_CMD | PrgCommand;  /* д����2 */
	 ISP_DATA = original_data;   /* д������׼�� */
	 ISPgoon();       /* ����ִ��  */
	 ISP_IAP_disable();     /* �ر�IAP���� */
	 EA =1;
}


/***********************1ms��ʱ����*****************************/
void delay_1ms(uint q)
{
	uint i,j;
	for(i=0;i<q;i++)
		for(j=0;j<120;j++);
}

/******************�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*****************/
void read_eeprom()	 //������������
{
	s_dengji  = byte_read(0x2001);
	s_dengji <<= 8;
	s_dengji  |= byte_read(0x2000);
	a_a      = byte_read(0x2058);
}

/******************�����ݱ��浽��Ƭ���ڲ�eeprom��******************/
void write_eeprom()	//��������
{
	SectorErase(0x2000);
	byte_write(0x2000, s_dengji % 256);
	byte_write(0x2001, s_dengji / 256);
	byte_write(0x2058,a_a);	
}

/**************�����Լ�eeprom��ʼ��*****************/
void init_eeprom()	 ////��ʼ��ʼ�����������
{
	read_eeprom();	   //������������
	if(a_a != 33)
	{
		a_a = 33;
		s_dengji = 80;
		write_eeprom();		 //��������
	}
}

/***********����ģת������********************************************************/	
//�����˽�ADC0832ģ��ת���Ĵ���Э�飬����������������Ҫ�Ƕ�Ӧʱ��ͼ����⣬��������ģ��0832�Ĵ���Э����е�
						//  1  0  0 ͨ��
						//  1  1  1 ͨ�� 
unsigned char ad0832read()
{
	unsigned char i=0,ch=0,ch1=0;		
		CS=0;
		DI=1;
	pulse0832();		//��ʼ
		DI=1;
		pulse0832();		//��һ��������	
		DI=0;
		pulse0832();
		DI=1;	    //�������½���
		for(i=0;i<8;i++)
		{
			pulse0832(); //��ʼ�ӵ��ĸ��½��ؽ�������
			ch<<=1;
			if(DO==1)
				ch|=0x01;						
		}
		for(i=0;i<8;i++)
		{			//����У������
			ch1>>=1;
			if(DO==1)
				ch1|=0x80;
			pulse0832();
		}
		CS=1;	
		return(ch==ch1)?ch:0;				//��У�����ݱȽϣ���ȷ�ͷ������ݣ����򷵻�0	
	

}

/********************������������*****************/
uchar key_can;	 //����ֵ

void key()	 //������������
{
	static uchar key_new;
	key_can = 20;                   //����ֵ��ԭ
	P2 |= 0x0f;					//�Ѱ�����IO�����Ϊ�ߵ�ƽ
	if((P2 & 0x0f) != 0x0f)		//��������
	{
		delay_1ms(1);	     	//����������
		if(((P2 & 0x0f) != 0x0f) && (key_new == 1))
		{						//ȷ���ǰ�������
			key_new = 0;
			switch(P2 & 0x0f)
			{
				case 0x0e: key_can = 3; break;	   //�õ�k1��ֵ
				case 0x0d: key_can = 2; break;	   //�õ�k2��ֵ
				case 0x0b: key_can = 1; break;	   //�õ�k3��ֵ
//				case 0x07: key_can = 4; break;	   //�õ�k4��ֵ
			}
		}			
	}
	else 
		key_new = 1;	
}



/****************���������������ʾ����***************/
void key_with()
{
	if(key_can == 1)		   //���ü�
	{
		menu_1 ++;
		if(menu_1 >= 2)
		{
			menu_1 = 0;
		}
	}
	if(menu_1 == 1)			//���þƾ�����ֵ
	{
		smg_i = 4;		    //��ʾ4λ�����
		if(key_can == 2)
		{
			s_dengji ++ ;		 //��1 
			if(s_dengji > 500)
				s_dengji = 500;
		}
		if(key_can == 3)
		{
			s_dengji -- ;		//��1	
			if(s_dengji <= 1)
				s_dengji = 1;
		}
		dis_smg[0] = smg_du[s_dengji % 10];	           //ȡ��λ��ʾ
		dis_smg[1] = smg_du[s_dengji / 10 % 10] ;      //ȡʮλ��ʾ
		dis_smg[2] = smg_du[s_dengji / 100 % 10] ;	   //
		dis_smg[3] = 0x05;	 //a
		write_eeprom();	//��������
	}	
}  


/***********************������ʾ����*****************************/
void display()
{
	static uchar i;	
	P1 = 0x00;			 //���� 				          
	P2 = smg_we[i];	 		 //λѡ
	P1 = dis_smg[i];		 //��ѡ	   
	i ++;
	if(i >= smg_i)
		i = 0;  	
}


/*************��ʱ��0��ʼ������***************/
void time_init()	  
{
	EA   = 1;	 	  //�����ж�
	TMOD = 0X01;	  //��ʱ��0����ʱ��1������ʽ1
	ET0  = 1;		  //����ʱ��0�ж� 
	TR0  = 1;		  //����ʱ��0��ʱ
}



/****************��������***************/
void clock_h_l()
{
	static uchar value;
	if((dengji >= s_dengji))		//����
	{
		value ++;
		if(value >= 2)
		{
			value = 10;
			beep = ~beep;	  //����������
		}
	}else 
	{
		if((dengji < s_dengji))	  //ȡ������
		{
			value = 0;
			beep = 1;
		}	
	}

}

/****************������***************/
void main()
{
	beep = 0;		                //������һ��   
	delay_1ms(150);
	P0 = P1 = P2 = P3 = 0xff;		//��Ƭ��IO�ڳ�ʼ��Ϊ1
	time_init();				//��ʼ����ʱ�� 
	init_eeprom();              //��ʼ��ʼ�����������
	while(1)
	{
		key();					//������������
		if(key_can < 10)
		{
			key_with();			//��������Ҫִ�еĳ���
		}
		if(flag_300ms == 1)
		{		
			flag_300ms = 0;
			clock_h_l();
			dengji = ad0832read();	
			dengji = dengji * 450 / 255.0;
		    dengji = dengji - 100;              //���ȼ�ȥ���Ư�ƣ�һ����100mV
			if(dengji < 0)
			{
				dengji = 0;	
			} 
			
			dengji = dengji * 2;             //��mVת���mg/L��ϵ����ҪУ׼   
								  //��ѹÿ����0.1V��ʵ�ʱ��������Ũ������20ppm 
								  //1ppm=1mg/kg=1mg/L=1��10-6 ��������ʾ����Ũ�ȣ�������ҺŨ�ȡ�       
			if(menu_1 == 0)
			{
				if(dengji >= 1000)
					smg_i = 4;
				else 
					smg_i = 3;
				dis_smg[3]=smg_du[dengji/1000%10];	//ǧλ
				dis_smg[2]=smg_du[dengji/100%10];	//��λ
				dis_smg[1]=smg_du[dengji/10%10];	//ʮλ
				dis_smg[0]=smg_du[dengji%10];	    //��λ	ADC0832Ϊ8λADC����ֵΪ0~255�����ǽ���ֿ�����l_tmpdate��������ʾ
			}
		} 
	}
}

/*************��ʱ��0�жϷ������***************/
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