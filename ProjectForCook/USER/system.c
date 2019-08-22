#include"system.h"
#include"integer.h"
#include"bluetooth.h"
#include "cooker.h"
#include "motor.h"
#include "jiDianQi.h"
extern BYTE ReadBuffer[200];
extern UINT fnum;

/*****************************************************

function:ʵ�ֶ�������Ŀϵͳ������ʵ�֣����磺system_init,��ʱ��


*****************************************************/

//�Զ�ʱ��ȫ�ֱ����Ķ���
static unsigned int timer1_count=0;
static unsigned int timer1_n=0;
static unsigned char timer1_commend=0;
static unsigned char timer1_flag=0;

static unsigned int timer2_count=0;
static unsigned int timer2_n=0;
static unsigned char timer2_commend=0;
static unsigned char timer2_flag=0;

//ϵͳ��ʼ������
void init_System()
{}
	
//ִ����Ӧ����
	void execute()
	{
		
		unsigned  char i=0,j=0;
		unsigned char num;
		unsigned short parameters[10],temp=0;  //�����������
		
		printf("ִ�г��˵ĸ�����\n");
		//ѭ������F
		printf("%d,%s",fnum,ReadBuffer);
		while((i<=fnum)&&(ReadBuffer[i]!='E'))
		{
			while((ReadBuffer[i]!='F')&&(ReadBuffer[i]!='E')) ++i;
			//��ȡ�������
				num=0;
				num=(ReadBuffer[++i]-'0')*10;
				num+=(ReadBuffer[++i]-'0');
			
			
			printf("�ҵ��ĺ������Ϊ%d \n",num);
			
			//ָ��(��
			i++;
			
			//��ȡ����
			j=0;
			while(ReadBuffer[i]!=')'&&(ReadBuffer[i]!='E'))
			{
				i++; //ָ�����
				while((ReadBuffer[i]!=',')&&(ReadBuffer[i]!=')'))
				{
					temp*=10;
					temp+=(ReadBuffer[i++]-'0');
		
				}
				parameters[j++]=temp; //��������
				temp=0; //����
			}
			//����������
			if(ReadBuffer[i]==')')
				i++;
			
			switch(num)
			{
				//�����ȹ����� void induction_cooker_contro(float temperature,
											//unsigned char time,unsigned char start_degree)
				case 1: printf("����ĺ������Ϊ1��������Ϊ%d,%d,%d\n",
												parameters[0],parameters[1],parameters[2]);
								induction_cooker_contro(parameters[0],
																				parameters[1],parameters[2],parameters[3]);
				break;
				
				//���ͽ�����ת�� void motor_revolve(unsigned char motor_n,unsigned char direction,
                       //unsigned char speed_stage,float distance)
				case 2: printf("����ĺ������Ϊ2��������Ϊ%d,%d,%d,%d\n",
					            parameters[0],parameters[1],parameters[2],parameters[3]);
								motor_revolve(parameters[0],parameters[1],
				        parameters[2],parameters[3],parameters[4],parameters[5]);
				break;
				
				//���˺�ת��  void motor_contro(unsigned char motor_n,unsigned char direction,
         //�������Ҫ������ϲ�            //  unsigned char speed_stage,float distance)
				case 3: printf("����ĺ������Ϊ3��������Ϊ,%d,%d,%d,%d\n",
					parameters[0],parameters[1],parameters[2],parameters[3]);  
								//motor_revolve(parameters[0],parameters[1],parameters[2],parameters[3]);
				break;
				
				//�̵��� void setJiDianQi(unsigned char Fa_Name,unsigned char statement,unsigned int time)
				case 4:  printf("����ĺ������Ϊ4��������Ϊ,%d,%d,%d\n",
												parameters[0],parameters[1],parameters[2]); 
								 turn_Ji(parameters[0],parameters[1],parameters[2],parameters[3]);
				break;
				default:printf("�������\n");
			}
				
		}
	}
	
	
	
	/********************************************************/
//�ö�ʱ��ʵ����ʱ ��λΪ����
void timer1(unsigned long time,unsigned char commend)
{	
		while(timer1_flag!=0);
		//�ñ�־λ
		timer1_flag=1;
	
		//��ʹ��
		TIM_Cmd(TIM6,ENABLE); //ʹ�ܶ�ʱ��	
		//��������
		timer1_count=time;
		timer1_commend=commend;
		
}

//�ö�ʱ��ʵ����ʱ
void timer2(unsigned long time,unsigned char commend)
{	
		while(timer2_flag!=0);
		//�ñ�־λ
		timer2_flag=1;
	
		//��ʹ��
		TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��	
		//��������
		timer2_count=time;
		timer2_commend=commend;
		
}


//ÿ����ʱ����ʼ��
void timers_init()
{
	//��ʱ��һ(timer6)�ĳ�ʼ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=(1000-1);   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=71; //��Ƶϵ��
	//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	
	//�жϷ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	//��ʱ����(timer7)�ĳ�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period=(1000-1);   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=71; //��Ƶϵ��
	//TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	
	//�жϷ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	
}



//��ʱ��һ���жϷ�����
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update))
	{
		timer1_n++;
		if(timer1_n==timer1_count)  //��ʱ��
		{
			switch(timer1_commend)
			{
				case 1: 
					TIM_SetCompare3(TIM3,0);  //�رյ��¯����
				break;
				
				case 3:
					motor_stop(1);                //ֹͣ������
					break;
				
				case 2:
					motor_stop(2);                  //ֹͣ��ת��
					break;
				case 4:turn_Ji(JiDianQi_1,DISABLE_JI,1,0);//�رռ̵���1
					break;
				case 5:turn_Ji(JiDianQi_2,DISABLE_JI,1,0);
					break;
				case 6:turn_Ji(JiDianQi_3,DISABLE_JI,1,0);
					break;
			}
			//����
			TIM_Cmd(TIM6,DISABLE);  //�رն�ʱ��
			timer1_commend=0;
			timer1_n=0;
			timer1_count=0;
			timer1_flag=0;
		}
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);	
}

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update))
	{
		timer2_n++;
		if(timer2_n==timer2_count)  //��ʱ��
		{
			switch(timer2_commend)
			{
				case 1: 
					TIM_SetCompare3(TIM3,0);  //�رյ��¯����
				break;
				
				case 3:motor_stop(1);;                //ֹͣ������
					break;
				
				case 2:motor_stop(2);;                  //ֹͣ��ת��
					break;
				case 4:turn_Ji(JiDianQi_1,DISABLE_JI,1,0);                    //�رռ̵���
					break;
				case 5:turn_Ji(JiDianQi_2,DISABLE_JI,1,0);
					break;
				case 6:turn_Ji(JiDianQi_3,DISABLE_JI,1,0);
					break;
			}
			//����
			TIM_Cmd(TIM7,DISABLE);  //�رն�ʱ��
			timer2_commend=0;
			timer2_n=0;
			timer2_count=0;
			timer2_flag=0;
		}
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);	
}




