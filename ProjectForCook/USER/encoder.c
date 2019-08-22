#include"encoder.h"
#include "bluetooth.h"
extern unsigned char fan_go_first;
extern unsigned short count;
//��ʼ��������
void Encode_Encoder_Init() //PA0(TIM2 CH1),PA1(TIM2 CH2)
{
	 //������һ�ĳ�ʼ��
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
	//ʹ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//����GPIO
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����ʱ��
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //���÷�Ƶ
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӷ�Ƶ�����ù�
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);  

    TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
		//����ṹ��
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM2,&TIM_ICInitStructure);
		
		//����жϱ�־λ
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2,ENABLE);
		
		//�����ж�
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		
		//���������ĳ�ʼ��********************************
		
		//ʹ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		//��ӳ��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	
		//����GPIO
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOD,&GPIO_InitStructure);
		
		//������ӳ��
		GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);//�ı�ָ���ܽŵ�ӳ��	
	
		//����ʱ��
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);  //ʱ����ʼ��
    TIM_TimeBaseStructure.TIM_Prescaler=0x0;  //���÷�Ƶ
    TIM_TimeBaseStructure.TIM_Period=ENCODER_TIM_PERIOD; //�Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӷ�Ƶ�����ù�
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  //���ϼ���
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);  

    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
		//����ṹ��
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter=10;
    TIM_ICInit(TIM4,&TIM_ICInitStructure);
		
		//����жϱ�־λ
    TIM_ClearFlag(TIM4,TIM_FLAG_Update);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4,ENABLE);
		
		//�����ж�
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}


short Read_Encoder1()
{
    short Encoder_TIM;    
        Encoder_TIM= (short)TIM2 -> CNT;  
        //TIM2 -> CNT=0;    //��������ǳ��ֶ�����ԭ��
        return Encoder_TIM;
}

short Read_Encoder2()
{
    short Encoder_TIM;    
        Encoder_TIM= (short)TIM4 -> CNT;  
        //TIM4 -> CNT=0; 
        return Encoder_TIM;
}
//������һ�ķ�����
void TIM2_IRQHandler(void)
{  
		short n;
    if(TIM2->SR&0X0001)//����ж�
    {    //ͨ���˴���֤�����������ʱ������������Ϊ�Զ���װ�ؼĴ�����ֵ
				n=Read_Encoder1();
				printf("������1���Ϊ��%d\n",n);
				motor_stop(1);
//				if(n==0)
//					motor_stop(1);
//				else
//				{
//					if(fan_go_first==1)
//					{
//						motor_stop(1);
//						fan_go_first=0;
//					}
//					else
//						{
//							delay(1000000);
//							fan_go_first=1;
//						}
//				}
    }                  
    TIM2->SR&=~(1<<0);//����жϱ�־λ        
}

//�����������жϷ�����
void TIM4_IRQHandler(void)
{  
		short n;
    if(TIM4->SR&0X0001)//����ж�
    {    //ͨ���˴���֤�����������ʱ������������Ϊ�Զ���װ�ؼĴ�����ֵ
				n=Read_Encoder2();
				printf("������2���Ϊ��%d\n",n);
				motor_stop(2);
			
    }                  
    TIM4->SR&=~(1<<0);//����жϱ�־λ        
}
 
static void delay(unsigned long i)
{
	while(--i);
}
