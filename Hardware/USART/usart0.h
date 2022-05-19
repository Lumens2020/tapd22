#ifndef _USART0_H
#define _USART0_H

#include "stdint.h"


#define USART0_TDATA_ADDRESS      ((uint32_t)0x40013828)     //���ݷ��ͼĴ����ĵ�ַ�������û��ֲ�445
#define USART0_RDATA_ADDRESS      ((uint32_t)0x40013824)  

#define USART0_REC_MUL_NUM	2			//���ն໺��ͨ��
#define USART0_REC_BUF_SIZE	30		//���ջ����С��ʵ��11
#define USART0_SND_BUF_SIZE	16    //���ͻ����С

#define DOG_TIME_OUT        20    //20MS

typedef struct{
	
	INT8U  BufA[USART0_REC_BUF_SIZE];	                    //�໺��
	INT8U  BufB[USART0_REC_BUF_SIZE];	                    //�໺��
	INT8U  Index;																				  //��ǰ�����ĸ�����
	INT8U  CountA;                                        //��¼�����˶�������
	INT8U  CountB;                                        //��¼�����˶�������
	INT8U  FeeDog;                                        //ͨ�Ź������ֽ�֮��ĳ�ʱ��ʱ������ι����
	INT8U  FinishFlag;                                    //�������	
	INT8U  TimeOutFlag;                                   //������Ź���־
	INT8U  TimeOutCnt;                                    //������Ź�����
	
}USART0_DMA_MUL_BUF;



typedef struct{
	
	INT8U  SndBuf[USART0_SND_BUF_SIZE];
	INT8U  SndLen;
	INT8U  SndOk;
	
	
}USART0_SND_T;

typedef struct
{
 	USART0_DMA_MUL_BUF  Rec;   //����
	USART0_SND_T        Snd;   //����
	
}USART0_TASK_STRUCT;

extern  USART0_TASK_STRUCT  volatile Usart0TaskData;


extern void Usart0_Task_Init(INT32U baud);


extern void Usart0_Rec_TimeOut_FeeDog(void);

extern void Usart0_Tx_Data(INT8U *str,INT8U len);

extern void Usart0_Rec_Task(void);

#endif

