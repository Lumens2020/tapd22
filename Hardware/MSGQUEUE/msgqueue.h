
#ifndef _MSGQUEUE_H
#define _MSGQUEUE_H

#include "stdint.h"
#include "queue.h"

#define  MSG_NUM  2
#define  MSG_NULL  (MSG_QUEUE_S *)0




typedef struct{
	 
	 INT8U Event;  //�¼�
	 INT8U Code;   //������
	 INT8U Time;   //ʱ���

}MSG_S;

typedef struct{

	 MSG_S Msg[MSG_NUM];  //�������ж�������
	 INT8U Num;
	 INT8U Save;
	 INT8U Get;
	 INT8U Id[2];         //��ʶ��
	 
}MSG_QUEUE_S;


extern void MsgQueue_Init(MSG_QUEUE_S *p);
extern void Save_MsgQueue(MSG_QUEUE_S *p, MSG_S *pMsg);
extern BOOL Is_MsgQueue_Full(MSG_QUEUE_S *p);
extern void Get_MsgQueue(MSG_QUEUE_S *p, MSG_S *pMsg );
extern INT8U MsgQueue_Num(MSG_QUEUE_S *p);


#endif
