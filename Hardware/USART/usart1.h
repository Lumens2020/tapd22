
#ifndef _USART1_H
#define _USART1_H

#include "stdint.h"

#define USART1_REC_MUL_NUM	2			//接收多缓冲通道
/*
有卡回复：00 00 FF 00 FF 00 00 00 FF 0C F4 D5 4B 01 01 00 04 08 04 02 57 C1 80 34，共计24字节
无卡回复：00 00 FF 00 FF 00，共计6个字节
*/
#define USART1_REC_BUF_SIZE	50		//接收缓存大小,实际24，接收容量稍微大一些
#define USART1_SND_BUF_SIZE	11    //发送缓存大小

typedef struct{
	
	INT8U  Buf[USART1_REC_MUL_NUM][USART1_REC_BUF_SIZE];	//多缓冲
	INT8U  Index;																				  //当前处于哪个缓冲
	INT8U  RecOk[USART1_REC_MUL_NUM];										  //缓冲装载完成标记：TRUE（完成），FALSE（未完成）
	INT32U Len[USART1_REC_MUL_NUM];
	INT8U  RxCount;                                       //记录接收了多少数据
  INT8U  RxBuf[USART1_REC_BUF_SIZE];                    //临时接收缓冲区
	
}USART1_DMA_MUL_BUF;



typedef struct{
	
	INT8U  SndBuf[USART1_SND_BUF_SIZE];
	INT8U  SndLen;
	INT8U  SndOk;
	
}USART1_SND_T;

typedef struct
{
	USART1_DMA_MUL_BUF  Rec;   //接收
	USART1_SND_T        Snd;   //发送
	
}USART1_TASK_STRUCT;

extern  USART1_TASK_STRUCT volatile Usart1TaskData;

extern  void Usart1_Rec_Task(void *pMsg);

extern void Usart1_Task_Init(INT32U baud);

extern void Usart1_Send_Task(void *pMsg);

extern void Usart1_Tx_Data(INT8U *str,INT8U len);

#endif
