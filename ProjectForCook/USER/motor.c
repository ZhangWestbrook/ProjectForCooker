#include"motor.h"
#include "system.h"
#include "bluetooth.h"
#include "stdio.h"
#include "encoder.h"


unsigned short count=0;

//in1--1,in2--0,ena--1 //takeup[0:499]
void motor_rotate_zheng(unsigned char motor_choose,unsigned int takeUp)
{
	//�����λ
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
		GPIO_ResetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		TIM_SetCompare1(TIM3,takeUp);
	}
	else if(motor_choose==2)
	{
		GPIO_SetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
		GPIO_ResetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
		TIM_SetCompare2(TIM3,takeUp);
	}
	else
	{
		//��������
	}
	
	
}

//in1--0,in2--1,ena--1
void motor_rotate_fan(unsigned char motor_choose,unsigned int takeUp)
{
	//�����λ
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_ResetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
		TIM_SetCompare1(TIM3,takeUp);
	}
	else if(motor_choose==2)
	{
		GPIO_SetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
		GPIO_ResetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
		TIM_SetCompare2(TIM3,takeUp);
	}
	else
	{
		//��������
	}


}


void GPIO_MOTOR_INT(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* ����ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //�����ʹ�ܸ��ù�����
	                                                    //�����ʹ����ӳ��ʱ��
	//��ʼ��PWM����
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin=MOTOR1_PWM_PIN|MOTOR2_PWM_PIN;
	GPIO_Init(MOTOR1_PWM_PORT,&GPIO_InitStructure);
	
	//��ʼ��in1����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR1_IN1_PIN|MOTOR2_IN1_PIN;
	GPIO_Init(MOTOR1_IN1_PORT,&GPIO_InitStructure);
	
		//��ʼ��in2����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=MOTOR1_IN2_PIN|MOTOR2_IN2_PIN;
	GPIO_Init(MOTOR1_IN2_PORT,&GPIO_InitStructure);
	
	
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);//�ı�ָ���ܽŵ�ӳ��	
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High ;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure); //����Ƚ�ͨ��1��ʼ��
	TIM_OC2Init(TIM3,&TIM_OCInitStructure); //����Ƚ�ͨ��2��ʼ��
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR1 �ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIMx�� CCR2 �ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	
}
	
void motor_stop(unsigned char motor_choose)
{
		//�����λ
	if(motor_choose==1)
	{
		GPIO_ResetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_ResetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
	}
	else	if(motor_choose==2)
			  {
						GPIO_ResetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
						GPIO_ResetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
			  }
				else
				{
					//��ʾ������
				}
	
}

void motor_flue(unsigned char motor_choose)
{
	//�����λ
	if(motor_choose==1)
	{
		GPIO_SetBits(MOTOR1_IN2_PORT,MOTOR1_IN2_PIN);
		GPIO_SetBits(MOTOR1_IN1_PORT,MOTOR1_IN1_PIN);
	}
	else	if(motor_choose==2)
				{
					GPIO_SetBits(MOTOR2_IN2_PORT,MOTOR2_IN2_PIN);
					GPIO_SetBits(MOTOR2_IN1_PORT,MOTOR2_IN1_PIN);
				}
				else
				{
					//��ʾ������
				}
}
void delay_ms(unsigned int delaytime)
{}
	
	
//������ת�ӿ�  
/**
	
	@function:ʵ�ֶԵ���ľ�ȷ����
	@para ���ѡ���˶����򣬵�λ����λ�ƣ���ʱ��ѡ�񣬶�ʱ����
	@ע��distance�ľ���Ϊ0.001
*/
void motor_revolve(unsigned char motor_choose,
	unsigned char direction,unsigned short speed_stage,
		float distance,unsigned timer_choose,unsigned int time)
		{
		
		if(time==0)
		{
			//�趨�����תλ��
			count=distance*2400;
		
			//��Ӧ����Ӧ�ı�������ȥ
			if(motor_choose==1)
			{
				TIM_SetAutoreload(TIM2,count);
				if(direction==DIRECTION_ZHENG)  //���ϼ���
					TIM_SetCounter(TIM2,0);
				else
				{
					TIM_SetCounter(TIM2,count-1);  //���¼���
					printf("��ʼ���õ�counter:%d\n",Read_Encoder1());
				}
			}
			else
				if(motor_choose==2)
				{
					TIM_SetAutoreload(TIM4,count);
					if(direction==DIRECTION_ZHENG)
						TIM_SetCounter(TIM4,0);
					else
						TIM_SetCounter(TIM4,count);
				}
				else
				{
					//��ʾ�����������
				
				}
		}
		else
		{
			if(motor_choose==1)
			//ͨ����ʱ��ֹͣ�����ת
			switch(timer_choose)
			{
				case 1:timer1(time*100,COMMEND_MOTOR_1);
				break;
				
				case 2:timer2(time*100,COMMEND_MOTOR_1);
				break;
				
				case 3:;
				break;
				
				//��ʾ����;
				default:;
					
				
			}
			if(motor_choose==2)
			{
				switch(timer_choose)
				{
					case 1:timer1(time*100,COMMEND_MOTOR_2);
					break;
				
					case 2:timer2(time*100,COMMEND_MOTOR_2);;
					break;
				
					case 3:;
					break;
				
					//��ʾ����;
					default:;
					}
				}
				else
				{
					//��ʾ����
				}
					
			}
		
			//�趨����͵�λ
		if(direction==DIRECTION_ZHENG)
		{
			motor_rotate_zheng(motor_choose,speed_stage);
		}
		else
		{
			motor_rotate_fan(motor_choose,speed_stage);
		}
	
}
	
