
#include "define.h"


QUEUE_S Queue;
/*
 ��ʼ��
*/
void Queue_Init(QUEUE_S *p)
{
	 p->get = 0x00;
	 p->save = 0x00;
	 p->num  = 0x00;
	
}
/*
�ж϶����Ƿ���
*/
BOOL Is_Queue_Full(QUEUE_S *p)
{
	return (p->num >= QUEUE_NUM) ? TRUE:FALSE;
}

/*
�ж϶������Ƿ�������
*/
INT8U Queue_Num(QUEUE_S *p)
{
	 return p->num;
}

/*
�ж϶����ڻ��ܴ漸������
*/
INT8U Queue_Remain_Num(QUEUE_S *p)
{
	 return (QUEUE_NUM - p->num);
}

/*
��һ������
*/
void Save_Queue_Dat(QUEUE_S *p,INT8U dat)
{
	  p->dat[p->save] = dat;
	  p->num++;
	  p->save = ( p->save + 1)%QUEUE_NUM;
}

/*
ȡһ������
*/
INT8U Get_QUEUE_Dat(QUEUE_S *p)
{
	  INT8U dat = p->dat[p->get];
	  p->num--;
	  p->get = (p->get + 1)%QUEUE_NUM;
	
	  return dat;
}

/*
��Դ��ַ�������ݵ�Ŀ�ĵ�ַ
*/
void copy(INT8U *pDst,INT8U *pSrc,INT8U n)
{
	 while(n--)
	 {
		 *pDst++ = *pSrc++ ;
	 }
}

/*
��һ������
*/
void Save_N_Queue_Dat(QUEUE_S *p,INT8U *pDat,INT8U n)
{
	INT8U len;
	if( QUEUE_NUM - p->save >= n )
	{
		 copy(&p->dat[p->save],pDat,n);
	}
	else
	{
		  len = QUEUE_NUM - p->save;
		  copy(&p->dat[p->save],pDat,len);
		  copy(p->dat,&pDat[len],n-len);
	}
	
	p->num += n;
	p->save = (	p->save + n)%QUEUE_NUM;
	
}

/*
ȡһ������,ȱ����Ҫ��ǰ֪����ȡ�ĳ��ȣ�����p->num >= n;
*/
void Get_N_QUEUE_Dat(QUEUE_S *p,INT8U *pDat, INT8U n)
{
	 INT8U len = QUEUE_NUM - p->get; 
	
	 if(len >= n)
	 {
		    copy(pDat,&p->dat[p->get],n);
	 }
	 else
	 {
		   copy(pDat,&p->dat[p->get],len);
		   copy(&pDat[len],p->dat,n-len);     //n-len ���ܴ���QUEUE_NUM
	 }
	 
	 p->num -= n;     //���濴�½��2022.02.18
	 p->get = (p->get + n)%QUEUE_NUM;
	 
	 
}
