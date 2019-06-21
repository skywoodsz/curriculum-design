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
		
		TMOD=0X01;//选择工作方式1，16位定时器
		TH0=(65535-10000)/256; // 1e4us 1s = 10e6 us
		TL0=(65535-10000)%256;
		ET0=1;//允许定时器T0
	  EA=1;//开总中断
    TR0=1;//打开16位定时器
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
	if(time_cout >= TIME * 10 - 1) // Time s 执行一次
	{
		time_cout = 0;
		if(flag == 0) //mode 1 流水灯
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
				P2 = (P2 >> 1) | 0x80; // 1000 0000 左右移都补0
				if(P2 == 0xfe)
				{
					flag_rev = 0;
					led_bing++;
				}
			}
			if(led_bing >= 2 * LED_LIUSHUI) //循环流水 LED_LIUSHUI 次
			{
				flag = 1;
				led_bing = 0;
				flag_rev = 0;
			}
		}
		else if(flag == 1) //mode2 亮灭
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
				if(led_bing >= 2 * LED_BINGUP) // 亮灭LED_BINGUP次
				{
					flag = 2;
					led_bing = 0;
				}
			}
			
		}
		else if(flag == 2) //mode3 双向流水灯
		{
			if(led_bing == 0)
			{
				led_bing++;
				P2 = 0X7e; // 0111 1110
			}
			else
			{
				/*
				* 对高四位与低四位截断，分别流水后合并
				*/
				if(flag_rev == 0)
				{
					P2 = ((((P2>>1)|0x80)/16)<<4) | (((P2<<1)|1)%16); //单目运算优于双目运算
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
				if(led_bing > 2 * LED_FENG) // 双向流水 LED_FENG 次
				{
					flag = 3;
				}
			}
		}
		if(flah = 3) //mode3 霹雳灯
		{
			;
		}
	}
}	
