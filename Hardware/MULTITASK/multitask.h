#ifndef _MULTITASK_H
#define _MULTITASK_H

#include "stdint.h"

/*
1��ʱ��Ƭ�������������  2�����������б�
  cnt -> flag ->ָ�� ->af[] (����ָ�룬����(void*))
	            ->period
*/



typedef void (*PFTASK)(void *pMsg);

typedef struct
{
	PFTASK pfTask;  //pfTask��һ��ָ������ڵ�ָ���������ƺ���ָ�����
	void *pMsg;
	
}TASK_S;

typedef struct
{
	 INT16U period;
	 TASK_S const*pGroup;  //ָ���ֵ���Ըı䣬���ܹ������ָ���޸�ָ��ָ����ڴ��ֵ		 
	
}TASK_GROUP_S;
/*
�˴���8�������������������Ҫ��flag,enable��ΪINT16U,��cnt[8]
*/
typedef struct
{
   INT8U flag;
	 INT16U cnt[8]; 
	 INT8U enable;
	 TASK_GROUP_S const*pTaskList;  
	
}TIME_SLICE_S;

extern TIME_SLICE_S MultiTask;


extern void Multi_Task_Init(void);
extern void Task_Exec(void);
extern void Time_Slice(void);


#endif

