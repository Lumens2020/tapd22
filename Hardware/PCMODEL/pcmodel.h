#ifndef _PCMODEL_H
#define _PCMODEL_H


#include "queue.h"
/*生产者与消费者放在一个结构体*/
typedef struct
{
	QUEUE_S *pQ;
	INT8U   buf[8];
	INT8U   idx;    //索引
	
}PCMODEL_S;

extern PCMODEL_S P1;
extern PCMODEL_S P2;
extern PCMODEL_S C1;
extern PCMODEL_S C2;

extern void Producer(void *pMsg);
extern void Consumer(void *pMsg);

#endif
