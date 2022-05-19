#ifndef _MULTITASK_H
#define _MULTITASK_H

#include "stdint.h"

/*
1、时间片多任务对象描述  2、创建任务列表
  cnt -> flag ->指针 ->af[] (函数指针，参数(void*))
	            ->period
*/



typedef void (*PFTASK)(void *pMsg);

typedef struct
{
	PFTASK pfTask;  //pfTask是一个指向函数入口的指针变量，简称函数指针变量
	void *pMsg;
	
}TASK_S;

typedef struct
{
	 INT16U period;
	 TASK_S const*pGroup;  //指针的值可以改变，不能够用这个指针修改指针指向的内存的值		 
	
}TASK_GROUP_S;
/*
此处是8个任务，如果更多任务，需要把flag,enable改为INT16U,改cnt[8]
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

