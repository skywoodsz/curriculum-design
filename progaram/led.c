// __author__ = skywoodsz

#include <reg52.h>
#include <intrins.h>
#define u8 unsigned char
#define LED_LIUSHUI 1
#define LED_BINGUP 1
#define LED_FENG 1
#define TIME 2

u8 time_cout = 0;
u8 led_bing = 0;
u8 flag = 0,flag_rev = 0;

void Time_Init()
{
		
		TMOD=0X01;//ѡ������ʽ1��16λ��ʱ��
		TH0=(65535-10000)/256; // 1e4us 1s = 10e6 us
		TL0=(65535-10000)%256;
		ET0=1;//����ʱ��T0
	  EA=1;//�����ж�
    TR0=1;//��16λ��ʱ��
}

int main()
{
	P2 = 0xfe; // 1111 1110
	Time_Init();
	while(1)
	{
		;
	}
	return 0;
}

void time1() interrupt 1
{
	TH0=(65535-10000)/256;
	TL0=(65535-10000)%256;
	time_cout++;
	if(time_cout >= TIME * 10 - 1) // Time s ִ��һ��
	{
		time_cout = 0;
		if(flag == 0) //mode 1 ��ˮ��
		{
			if(flag_rev == 0)
			{
				P2 = (P2 << 1) | 1;
				if(P2 == 0x7f) //0111 1111
				{
					//P2 = 0xfe;
					flag_rev = 1;
					led_bing++;
				}
			}
			else
			{
				P2 = (P2 >> 1) | 0x80; // 1000 0000 �����ƶ���0
				if(P2 == 0xfe)
				{
					flag_rev = 0;
					led_bing++;
				}
			}
			if(led_bing >= 2 * LED_LIUSHUI) //ѭ����ˮ LED_LIUSHUI ��
			{
				flag = 1;
				led_bing = 0;
				flag_rev = 0;
			}
		}
		else if(flag == 1) //mode2 ����
		{
			if(led_bing == 0)
			{
				P2 = 0xff; // 1111 1111
				led_bing++;
			}
			else
			{
				P2 = ~P2;
				led_bing++;
				if(led_bing >= 2 * LED_BINGUP) // ����LED_BINGUP��
				{
					flag = 2;
					led_bing = 0;
				}
			}
			
		}
		else if(flag == 2) //mode3 ˫����ˮ��
		{
			if(led_bing == 0)
			{
				led_bing++;
				P2 = 0X7e; // 0111 1110
			}
			else
			{
				/*
				* �Ը���λ�����λ�ضϣ��ֱ���ˮ��ϲ�
				*/
				if(flag_rev == 0)
				{
					P2 = ((((P2>>1)|0x80)/16)<<4) | (((P2<<1)|1)%16); //��Ŀ��������˫Ŀ����
					if(P2 == 0xe7)
					{
						flag_rev = 1;
						led_bing++;
					}
				}
				else
				{
					P2 = ((((P2<<1)|0x10)/16)<<4) | (((P2>>1)|0x08)%16);
					if(P2 == 0X7e)
					{
						flag_rev = 0;
						led_bing++;
					}
				}
				if(led_bing > 2 * LED_FENG) // ˫����ˮ LED_FENG ��
				{
					flag = 3;
				}
			}
		}
		if(flah = 3) //mode3 ������
		{
			;
		}
	}
}	
