
#include "define.h"


QUEUE_S Queue;
/*
 初始化
*/
void Queue_Init(QUEUE_S *p)
{
	 p->get = 0x00;
	 p->save = 0x00;
	 p->num  = 0x00;
	
}
/*
判断队列是否满
*/
BOOL Is_Queue_Full(QUEUE_S *p)
{
	return (p->num >= QUEUE_NUM) ? TRUE:FALSE;
}

/*
判断队列内是否有数据
*/
INT8U Queue_Num(QUEUE_S *p)
{
	 return p->num;
}

/*
判断队列内还能存几个数据
*/
INT8U Queue_Remain_Num(QUEUE_S *p)
{
	 return (QUEUE_NUM - p->num);
}

/*
存一个数据
*/
void Save_Queue_Dat(QUEUE_S *p,INT8U dat)
{
	  p->dat[p->save] = dat;
	  p->num++;
	  p->save = ( p->save + 1)%QUEUE_NUM;
}

/*
取一个数据
*/
INT8U Get_QUEUE_Dat(QUEUE_S *p)
{
	  INT8U dat = p->dat[p->get];
	  p->num--;
	  p->get = (p->get + 1)%QUEUE_NUM;
	
	  return dat;
}

/*
从源地址复制数据到目的地址
*/
void copy(INT8U *pDst,INT8U *pSrc,INT8U n)
{
	 while(n--)
	 {
		 *pDst++ = *pSrc++ ;
	 }
}

/*
存一组数据
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
取一组数据,缺陷是要提前知道所取的长度，而且p->num >= n;
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
		   copy(&pDat[len],p->dat,n-len);     //n-len 不能大于QUEUE_NUM
	 }
	 
	 p->num -= n;     //仿真看下结果2022.02.18
	 p->get = (p->get + n)%QUEUE_NUM;
	 
	 
}
