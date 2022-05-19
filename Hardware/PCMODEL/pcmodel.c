
#include "define.h"

/*P1,P2,C1,C2����һ������������ַ������ͬ����Ϊ��ʼ�����Ǹ�ֵ&Queue*/

/*���������߳�ʼ��*/
PCMODEL_S P1 = {&Queue,{0X1,0X2,0X3,0X4,0X5},0X00};
PCMODEL_S P2 = {&Queue,{0X6,0X7,0X8,0X9,0X0},0X00};

/*���������߳�ʼ��*/
PCMODEL_S C1 = {&Queue,{0X0,0X0,0X0,0X0,0X0},0X00};
PCMODEL_S C2 = {&Queue,{0X0,0X0,0X0,0X0,0X0},0X00};

/*���������ݴ������*/
void Producer(void *pMsg)
{
	
		PCMODEL_S *p = (PCMODEL_S *)pMsg;  //����ת��
		
		if( p->idx < 4 )  //�����߲���4������
		{
			
			 /*����û����*/
			 if( Is_Queue_Full(p->pQ) == FALSE )  //TRUE������FALSE�ǲ���
			 {
				  Save_Queue_Dat(p->pQ,p->buf[p->idx++]);  //������

			 }

			 
			  
		}
	
}

/*�����߶�ȡ�����ڵ�����*/
void Consumer(void *pMsg)
{
	 PCMODEL_S *p = (PCMODEL_S *)pMsg;
	 /*�������Ƿ�������*/
	if(Queue_Num(p->pQ))
	{  
		 /*����һЩӦ�ü��㣬PID,EEPROM*/
		 p->buf[p->idx++] = Get_QUEUE_Dat(p->pQ);  //ȡ�������ڵ����ݴ��뵽buf��
		
	}
}