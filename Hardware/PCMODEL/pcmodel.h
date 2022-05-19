#ifndef _PCMODEL_H
#define _PCMODEL_H


#include "queue.h"
/*�������������߷���һ���ṹ��*/
typedef struct
{
	QUEUE_S *pQ;
	INT8U   buf[8];
	INT8U   idx;    //����
	
}PCMODEL_S;

extern PCMODEL_S P1;
extern PCMODEL_S P2;
extern PCMODEL_S C1;
extern PCMODEL_S C2;

extern void Producer(void *pMsg);
extern void Consumer(void *pMsg);

#endif
