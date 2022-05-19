
#include "define.h"

/*P1,P2,C1,C2共用一个环形区，地址都是相同。因为初始化都是赋值&Queue*/

/*两个生产者初始化*/
PCMODEL_S P1 = {&Queue,{0X1,0X2,0X3,0X4,0X5},0X00};
PCMODEL_S P2 = {&Queue,{0X6,0X7,0X8,0X9,0X0},0X00};

/*两个消费者初始化*/
PCMODEL_S C1 = {&Queue,{0X0,0X0,0X0,0X0,0X0},0X00};
PCMODEL_S C2 = {&Queue,{0X0,0X0,0X0,0X0,0X0},0X00};

/*生产者数据存入队列*/
void Producer(void *pMsg)
{
	
		PCMODEL_S *p = (PCMODEL_S *)pMsg;  //类型转换
		
		if( p->idx < 4 )  //生产者产生4个数据
		{
			
			 /*队列没有满*/
			 if( Is_Queue_Full(p->pQ) == FALSE )  //TRUE是满，FALSE是不满
			 {
				  Save_Queue_Dat(p->pQ,p->buf[p->idx++]);  //存数据

			 }

			 
			  
		}
	
}

/*消费者读取队列内的数据*/
void Consumer(void *pMsg)
{
	 PCMODEL_S *p = (PCMODEL_S *)pMsg;
	 /*队列内是否有数据*/
	if(Queue_Num(p->pQ))
	{  
		 /*进行一些应用计算，PID,EEPROM*/
		 p->buf[p->idx++] = Get_QUEUE_Dat(p->pQ);  //取出队列内的数据存入到buf中
		
	}
}