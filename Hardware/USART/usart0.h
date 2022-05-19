#ifndef _USART0_H
#define _USART0_H

#include "stdint.h"


#define USART0_TDATA_ADDRESS      ((uint32_t)0x40013828)     //数据发送寄存器的地址，中文用户手册445
#define USART0_RDATA_ADDRESS      ((uint32_t)0x40013824)  

#define USART0_REC_MUL_NUM	2			//接收多缓冲通道
#define USART0_REC_BUF_SIZE	30		//接收缓存大小，实际11
#define USART0_SND_BUF_SIZE	16    //发送缓存大小

#define DOG_TIME_OUT        20    //20MS

typedef struct{
	
	INT8U  BufA[USART0_REC_BUF_SIZE];	                    //多缓冲
	INT8U  BufB[USART0_REC_BUF_SIZE];	                    //多缓冲
	INT8U  Index;																				  //当前处于哪个缓冲
	INT8U  CountA;                                        //记录接收了多少数据
	INT8U  CountB;                                        //记录接收了多少数据
	INT8U  FeeDog;                                        //通信过程中字节之间的超时定时器，“喂狗”
	INT8U  FinishFlag;                                    //接收完成	
	INT8U  TimeOutFlag;                                   //软件看门狗标志
	INT8U  TimeOutCnt;                                    //软件看门狗计数
	
}USART0_DMA_MUL_BUF;



typedef struct{
	
	INT8U  SndBuf[USART0_SND_BUF_SIZE];
	INT8U  SndLen;
	INT8U  SndOk;
	
	
}USART0_SND_T;

typedef struct
{
 	USART0_DMA_MUL_BUF  Rec;   //接收
	USART0_SND_T        Snd;   //发送
	
}USART0_TASK_STRUCT;

extern  USART0_TASK_STRUCT  volatile Usart0TaskData;


extern void Usart0_Task_Init(INT32U baud);


extern void Usart0_Rec_TimeOut_FeeDog(void);

extern void Usart0_Tx_Data(INT8U *str,INT8U len);

extern void Usart0_Rec_Task(void);

#endif

