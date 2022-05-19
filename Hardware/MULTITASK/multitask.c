
#include "define.h"

TIME_SLICE_S MultiTask;

void Multi_Task_Init(void)
{
		INT8U i;
		INT8U num = Task_Group_Num();
		MultiTask.flag = 0;
		MultiTask.enable = ~0x00;    //�����־ʹ�ܣ�������8������
		for(i=0; i<num; i++)
		{
			 	MultiTask.cnt[i] = 0;
		}
		Init_Task_List();   //�����б��ʼ��
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
					 p = MultiTask.pTaskList[i].pGroup[j].pfTask;   //���е�����
					 
					 while(p != NULL)    //�ж����е�����ǿ�
					 {
						  (*p)(MultiTask.pTaskList[i].pGroup[j].pMsg); //��������
						  p = MultiTask.pTaskList[i].pGroup[++j].pfTask; //ָ����һ����     
					 }
				}
		 }
	 }
}