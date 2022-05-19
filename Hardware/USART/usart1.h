
#ifndef _USART1_H
#define _USART1_H

#include "stdint.h"

#define USART1_REC_MUL_NUM	2			//���ն໺��ͨ��
/*
�п��ظ���00 00 FF 00 FF 00 00 00 FF 0C F4 D5 4B 01 01 00 04 08 04 02 57 C1 80 34������24�ֽ�
�޿��ظ���00 00 FF 00 FF 00������6���ֽ�
*/
#define USART1_REC_BUF_SIZE	50		//���ջ����С,ʵ��24������������΢��һЩ
#define USART1_SND_BUF_SIZE	11    //���ͻ����С

typedef struct{
	
	INT8U  Buf[USART1_REC_MUL_NUM][USART1_REC_BUF_SIZE];	//�໺��
	INT8U  Index;																				  //��ǰ�����ĸ�����
	INT8U  RecOk[USART1_REC_MUL_NUM];										  //����װ����ɱ�ǣ�TRUE����ɣ���FALSE��δ��ɣ�
	INT32U Len[USART1_REC_MUL_NUM];
	INT8U  RxCount;                                       //��¼�����˶�������
  INT8U  RxBuf[USART1_REC_BUF_SIZE];                    //��ʱ���ջ�����
	
}USART1_DMA_MUL_BUF;



typedef struct{
	
	INT8U  SndBuf[USART1_SND_BUF_SIZE];
	INT8U  SndLen;
	INT8U  SndOk;
	
}USART1_SND_T;

typedef struct
{
	USART1_DMA_MUL_BUF  Rec;   //����
	USART1_SND_T        Snd;   //����
	
}USART1_TASK_STRUCT;

extern  USART1_TASK_STRUCT volatile Usart1TaskData;

extern  void Usart1_Rec_Task(void *pMsg);

extern void Usart1_Task_Init(INT32U baud);

extern void Usart1_Send_Task(void *pMsg);

extern void Usart1_Tx_Data(INT8U *str,INT8U len);

#endif
