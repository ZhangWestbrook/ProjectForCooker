#include "jiDianQi.h"
#include "system.h"

void jiDianQi_Init()
{
	//�����ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//����GPIO
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//����ƽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_All);
}

//n��ȡJiDianQi_x  
//status��ȡENBALE_JI or DISABLE_JI
void turn_Ji(unsigned short n,unsigned char status,
	unsigned char timer_choose,unsigned char time)
{
	//����Ӧλ
	if(status==ENABLE_JI)
	{
		if(timer_choose==1)
		{
			//��ʱ��Ӧʱ��
			timer1(time*100,4);
		
			GPIO_SetBits(JiDianQi_Port,n);
		}
		if(timer_choose==2)
		{
			//��ʱ��Ӧʱ��
			timer2(time*100,4);
		
			GPIO_SetBits(JiDianQi_Port,n);
		}
		else
		{
			//��ʾ����
		}
	}
	else
	{
		GPIO_ResetBits(JiDianQi_Port,n);
	}
	
	
}
