
#include "define.h"


void MsgQueue_Init(MSG_QUEUE_S *p)
{
	
	 p->Num  = 0x00;
	 p->Get  = 0x00;
	 p->Save = 0x00;
	
}

void Save_MsgQueue(MSG_QUEUE_S *p, MSG_S *pMsg)
{
	 p->Msg[p->Save] = *pMsg;          //结构体类型赋值给结构体类型
	 p->Num         += 1;
	 p->Save         = ++p->Save%MSG_NUM;	 
}

void Get_MsgQueue(MSG_QUEUE_S *p, MSG_S *pMsg )
{
	  *pMsg = p->Msg[p->Get];
	   p->Num--;
	   p->Get = ++p->Get%MSG_NUM;
}

BOOL Is_MsgQueue_Full(MSG_QUEUE_S *p)
{
	return (p->Num >= MSG_NUM)? TRUE : FALSE;
}

INT8U MsgQueue_Num(MSG_QUEUE_S *p)
{
	 return p->Num;
}

