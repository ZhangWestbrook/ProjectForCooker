#ifndef __CAN_H_
#define __CAN_H_


/****************************************************

function:ʵ�ֶ�can����Ŀ��ƺ�������ʵ��

****************************************************/
#define CAN_RX0_INT_ENABLE 0   //��ʹ���ж�
#include "stm32f10x.h" 
void CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 CAN_Send_Msg(u8* msg,u8 len);						//��������

u8 CAN_Receive_Msg(u8 *buf);							//��������


#endif