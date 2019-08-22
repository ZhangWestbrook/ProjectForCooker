#include "led.h"
#include "pwm.h"
#include "stm32f10x.h"
#include "encoder.h"
#include "bluetooth.h"
#include "spi_fat_flash.h"
#include "ff.h"
#include "stdio.h"
#include "system.h"
#include "can.h"
#include "cooker.h"
#include "jiDianQi.h"


//main�ļ��еĺ�������
void file_System_Init();
void anolyses_Commend();


//���巴�������ʼ������־��־
unsigned char fan_go_first=0;

//������������Ҫ�ı���
unsigned char arry[8]={0};
unsigned char arry_i=0;
unsigned char check_flag=0;
unsigned char save_flag=0;
unsigned char no_or=0;
unsigned char dat[100];
unsigned char finish_flag=0;
unsigned char made_flag=0;

//�����ļ�ϵͳ����Ҫ��ȫ�ֱ���
FILINFO file_info;
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */
BYTE ReadBuffer[200]={0};        /* �������� */
//BYTE WriteBuffer[] =              /* д������*/
//"��ӭʹ�ñ���STM32������ xie �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n"; 

int main(void)
{
	//�������
	unsigned char res;
	unsigned char tbuf[8],char_buf[8];
	unsigned char rbuf[8];
	unsigned char j=0;
	
	//�����¶�ֵ
	unsigned char temper=0;
	
	/* ��ʼ��LED */
	//LED_Init();	
	
	//���¯��ʼ��
	Cooker_Init(500,72-1);
	
	//�����ʼ��
	GPIO_MOTOR_INT(500,72-1);
	
	/* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
	USART1_Bluetooth_Init(115200);	
	
	//�����ļ�ϵͳ��ʼ��
	file_System_Init();
	
	//can��ʼ��
	CAN_Mode_Init(CAN_SJW_2tq,CAN_BS2_3tq,
	CAN_BS1_5tq,4,CAN_Mode_Normal);//������Ϊ1M
	
	//��ʼ���̵���
	jiDianQi_Init();
	
	//��������ʼ��
	Encode_Encoder_Init();
	
	//ϵͳ��ʱ����ʼ��
	timers_init();
	
	//SPI_MAX6675��ʼ��
	MAX6675_init();
	
	/******************************************************/
	#if 0
	//�����¶�
	while(1)
	{
	
		temper=Get_temperature();
		printf("ת��Ϊ���Σ����Ϊ��%d\n",temper);
	}
	
	#endif
	
	//////////////////////////////////////////////////////////
	/**********************************************************/
	//����Ϊ���Դ���
	#if 1
	//�������ļ�ϵͳ�������Գɹ���ֻ�����ݴ��䳤�ˣ�����ִ���
	
	//���Լ̵��� //��ɼ̵�������
	turn_Ji(JiDianQi_1,ENABLE_JI,1,100);
	//���Ե��¯  //��ɵ�λ�Ĳ���  ʱ�䱣֤Ϊ0.1��
	induction_cooker_contro(200,1,200,110);
	//���Ե��һ  //��ɵ�λ�Ĳ���
	motor_revolve(1,0,250,2,1,100);  //���һ�����򣬵�λ��Ȧ����ʱ��1��ʱ��40s
	//���Ե����
	
	while(1);
	#endif
	
	#if 0
	/*************************�ع���**********************/
	//1.�ȹ� 180c 20s
	induction_cooker_contro(200,1,20,110);  //200�ȣ�timer1,2s,��λ(0-400)
  //2. ���� �Ϻ�һ
	turn_Ji(JiDianQi_1,ENABLE_JI,1,30);  //�̵���ѡ��״̬��timer,ʱ��
	induction_cooker_contro(200,2,60,110);
	//3.���⣨���٣� �Ϻж�
	turn_Ji(JiDianQi_2,ENABLE_JI,2,20);
	//induction_cooker_contro(200,2,100,110);
	//4.�����г� 220c 50s
	induction_cooker_contro(200,2,500,110);
	
	motor_revolve(1,1,400,0,1,500);    //��������򣬵�λ��Ȧ����timer��50s
	
	//5.���ཷ���Ϻ���������
	turn_Ji(JiDianQi_3,ENABLE_JI,1,100);
	//6.�����쳴 220c 70s
	induction_cooker_contro(200,1,700,110);
	motor_revolve(1,0,400,0,2,700);  //��������򣬵�λ��Ȧ����timer��70s
	//7.end
	while(1);
	
	#endif
	////////////////////////////////////////////////////////////
	/**********************************************/
	# if 0
	//����Ϊʵ�ʴ���
	while(1)
	{
		file_System_Init();//����spi-flash
		
		/*�ȴ�������������д���������ļ���д�룬����Ƕ�����
		����Ҫ�����ݶ���*/
		anolyses_Commend();
		
		//ͨ��CAN��ӻ�ͨ�ţ�ѡ�񳴲˻�
		for(j=0;j<8;j++)
			{
				tbuf[j]=j;
				char_buf[j]=tbuf[j]+0x30;
			}
			res=CAN_Send_Msg(tbuf,8);
			if(res)
			{
				printf("Send Failed!\r\n");
			}
			else
			{
				printf("�������ݣ�%s\r\n",char_buf);
			}
		res=CAN_Receive_Msg(rbuf);
		if(res)
		{
			for(j=0;j<res;j++)
			{
				char_buf[j]=rbuf[j]+0x30;
			}
			printf("�������ݣ�%s\r\n",char_buf);
		}
		
		//ͨ������can���߻�õ����ݣ�ѡ�񳴲˻�����ѡ�񱾻�
		
		
		//������ҵ����˵Ĳ���
		if(made_flag==1)
		{
			//��������
			printf("��������\n");
			execute();
			made_flag=0;  //�����־Ӧ�÷���canͨ�ź���λ�������Ƿ��ڶ����ݺ���
		}
			
	}
	#endif
	
}
	
/*********************************************************************
 �ļ�ϵͳ�ĳ�ʼ������
********************************************************************/
void file_System_Init()
{
	  printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
  
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��...\r\n");
    /* ��ʽ�� */
		res_flash=f_mkfs("1:",0,0);							
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ��\r\n");
      /* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"1:",1);			
      /* ���¹���	*/			
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			//LED_RED;
			printf("������ʽ��ʧ�ܡ�����\r\n");
			while(1);
		}
	}
  else if(res_flash!=FR_OK)
  {
    printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
  }
  else
  {
    printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
  }
}
	
	
	
	
void anolyses_Commend()
{
	unsigned long address=0;
	unsigned char length=0;
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	while(finish_flag==0);
	printf("�յ���Э�鳤��Ϊ��%d \n",arry_i);
	if(made_flag==0)
	{
		printf("\r\n****** ���������ļ�д�����... ******\r\n");	
		//��һ���Ǵ������ڶ��������
		res_flash = f_open(&fnew, "1:���ܲ���.txt",FA_CREATE_ALWAYS | FA_WRITE );
		//res_flash = f_open(&fnew, "1:���ܲ���.txt",  FA_WRITE );
		if ( res_flash == FR_OK )
		{
			printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
			/* ��ָ���洢������д�뵽�ļ��� */
			//res_flash=f_write(&fnew,dat,arry_i,&fnum);
			
			//��ȡ�ļ�����
			f_stat("1:���ܲ���.txt",&file_info);
			f_lseek(&fnew,file_info.fsize);
			f_puts(dat,&fnew);
			if(res_flash==FR_OK)
			{
				printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);
				printf("�����ļ�д�������Ϊ��\r\n%s\r\n",dat);
			}
			else
			{
				printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
			}   
	
			/* ���ٶ�д���ر��ļ� */
			f_close(&fnew);
			
		}
	
		else
		{	
			printf("������/�����ļ�ʧ�ܡ�\r\n");
		}
}
	
/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	if(made_flag==1)
	{
		printf("****** ���������ļ���ȡ����... ******\r\n");
		res_flash = f_open(&fnew, "1:���ܲ���.txt",FA_OPEN_EXISTING | FA_READ); 	 
		if(res_flash == FR_OK)
		{
			printf("�����ļ��ɹ���\r\n");
			
			//��λ���ļ���Ӧ��λ��**********************
			address=0;
			address|=arry[4]<<16;
			address|=arry[5]<<8;
			address|=arry[6]<<0;
			
			//��ȡ���ݵĳ���
			length=arry[7];
			
			f_lseek(&fnew,address);
			res_flash = f_read(&fnew, ReadBuffer, length, &fnum); 
			if(res_flash==FR_OK)
			{
				printf("���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n",fnum);
				printf("����ȡ�õ��ļ�------����Ϊ��\r\n%s \r\n", ReadBuffer);	
				
				//��λ���˱�־λ
				
			}
			else
			{
				printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_flash);
			}		
			/* ���ٶ�д���ر��ļ� */
			f_close(&fnew);	
		}
		else
		{
			printf("�������ļ�ʧ�ܡ�\r\n");
		}
		
	}
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);
  
	//��������ȡ��ɱ�־����
	finish_flag=0;
}

