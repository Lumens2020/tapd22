#ifndef _QUEUE_H
#define _QUEUE_H

#include "stdint.h"


/*
C语言未提供BOOL型数据
*/
typedef enum
{
	FALSE,
	TRUE
}BOOL;

#define QUEUE_NUM 4


/*
环形缓冲区,合理设置缓冲区大小
*/
typedef struct
{
	INT8U save;
	INT8U get;
	INT8U num;
	INT8U dat[QUEUE_NUM];
	
}QUEUE_S;

extern QUEUE_S Queue;


extern void Queue_Init(QUEUE_S *p);
extern BOOL Is_Queue_Full(QUEUE_S *p);
extern INT8U Queue_Num(QUEUE_S *p);
extern INT8U Queue_Remain_Num(QUEUE_S *p);
extern void Save_Queue_Dat(QUEUE_S *p,INT8U dat);
extern INT8U Get_QUEUE_Dat(QUEUE_S *p);
extern void copy(INT8U *pDst,INT8U *pSrc,INT8U n);
extern void Save_N_Queue_Dat(QUEUE_S *p,INT8U *pDat,INT8U n);
extern void Get_N_QUEUE_Dat(QUEUE_S *p,INT8U *pDat, INT8U n);


#endif
