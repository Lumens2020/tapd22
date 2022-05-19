
#include "define.h"

TIME_SLICE_S MultiTask;

void Multi_Task_Init(void)
{
		INT8U i;
		INT8U num = Task_Group_Num();
		MultiTask.flag = 0;
		MultiTask.enable = ~0x00;    //任务标志使能，这里是8个任务
		for(i=0; i<num; i++)
		{
			 	MultiTask.cnt[i] = 0;
		}
		Init_Task_List();   //任务列表初始化
}

void Time_Slice(void)
{
	INT8U i;
	INT8U num = Task_Group_Num();
	
	for(i=0; i<num; i++)
	{
		 if( MultiTask.enable & (1<<i) )
		 {
			 
			  MultiTask.cnt[i]++;
			  if( MultiTask.cnt[i] >= MultiTask.pTaskList[i].period )
				{
					 MultiTask.flag |= (1<<i);
					 MultiTask.cnt[i] = 0;
				}
		 }
	}
}

void Task_Exec(void)
{
	 PFTASK p;
	 INT8U i;
	 INT8U j;
   INT8U num = Task_Group_Num();
	
	 if( MultiTask.flag )
	 {
		  for(i=0; i< num; i++)
		 {
			  if(MultiTask.flag & (1<<i))
				{
					 j=0;
					 MultiTask.flag &= ~(1<<i);
					 p = MultiTask.pTaskList[i].pGroup[j].pfTask;   //运行的任务
					 
					 while(p != NULL)    //判断运行的任务非空
					 {
						  (*p)(MultiTask.pTaskList[i].pGroup[j].pMsg); //任务运行
						  p = MultiTask.pTaskList[i].pGroup[++j].pfTask; //指向下一任务     
					 }
				}
		 }
	 }
}