
#include "define.h"

static INT8U G_Comm_Snd_Alloc_Index = 0;

COMM_SND_T CommSndBuf[COMM_SND_BUF_SIZE];

COMM_SND_T *PMduSnd;    //MDU句柄，指向MDU一段空间

//COMM_SND_T *PRfidSnd;   //RFID句柄，指向RFID一段空间

/*判忙*/
INT8U Is_Comm_Snd_Busy(COMM_SND_T *pSnd)
{
	 if( FALSE == pSnd->Tmr.En )
	 {
		 return FALSE;
	 }
	 else
	 {
		  return TRUE;
	 }
	 

}

/*发送数据包*/
INT8U Comm_Snd_Packet(COMM_SND_T *pSnd,INT8U *src,INT8U len)
{
	  INT8U bret  = FALSE; 
	  if( pSnd->pfSnd != (FUNC_HD_SND *)0 )
		{
			if( FALSE == pSnd->Tmr.En )  //只有不忙的时候可以发送,FALSE为不忙
			{
				 pSnd->Tmr.Max = pSnd->Tmr.TimeOut;
				 bret = pSnd->pfSnd(src,len);  //回调到最底层：Usart1_Dma_Snd
				
				if(TRUE == bret)          //发送成功
				 {
					 pSnd->Tmr.Cnt = 0;
					 pSnd->Tmr.En = TRUE; //设置定时器不忙
				 }
			}
		}
	 
		return bret;
}

/*释放发送句柄*/
void Comm_Snd_Release(COMM_SND_T *pSnd)
{
	 pSnd->Tmr.En = FALSE;
}


/*设置超时计数器延时超时时间，当正在接收回传时，需要重新设置超时时间，避免接收数据本身长度超出超时长度*/
void Comm_Snd_Set_Cnt_Dly(COMM_SND_T *pSnd,INT8U val)
{
	 if(val >= pSnd->Tmr.Max)
	 {
		 pSnd->Tmr.Cnt = 0;
	 }
	 else
	 {
		 pSnd->Tmr.Cnt = pSnd->Tmr.Max - val;
	 }
}


/*创建发送句柄*/
COMM_SND_T *Comm_Snd_Create(FUNC_HD_SND *pfSnd,FUNC_SND_TIMEOUT *pfTimeOut, INT16U timeout)
{
	  COMM_SND_T *pBuf;
	  pBuf = (COMM_SND_T *)0;
	  if(G_Comm_Snd_Alloc_Index <= COMM_SND_BUF_SIZE )
		{
			if(pfSnd != (FUNC_HD_SND *)0)
			{
				 pBuf = &CommSndBuf[G_Comm_Snd_Alloc_Index];
				 G_Comm_Snd_Alloc_Index++;
				/*对应函数的回调初始化*/
				 pBuf->pfSnd = pfSnd;
				 pBuf->pfTimeOut = pfTimeOut;
				 
				 /*定时器初始化*/
				 pBuf->Tmr.En = FALSE;
				 pBuf->Tmr.Cnt = 0;
				 pBuf->Tmr.TimeOut = timeout;
				 pBuf->Tmr.Max = timeout;
				 
			}
			
		}
		
		return pBuf;
}

/*
数据发送超时计数，包含了串口0和串口1
*/

void Comm_Snd_Tick(void)
{
		INT8U i;
		static COMM_SND_T *pSnd;
		
		for(i=0; i<G_Comm_Snd_Alloc_Index; i++ )
		{
				pSnd = &CommSndBuf[i];
			
			 if(TRUE == pSnd->Tmr.En)
			 {
					 pSnd->Tmr.Cnt++;
					 if(pSnd->Tmr.Cnt > pSnd->Tmr.Max)
					 {
							 pSnd->Tmr.Cnt = 0;
							 pSnd->Tmr.En = FALSE;
							 if( pSnd->pfTimeOut != ( FUNC_SND_TIMEOUT *)0 )
							 {
								 /*超时发送*/
								 pSnd->pfTimeOut();  //跳转到void Mdu_Send_TimeOut(void)
							 } 
					 }
				}
		}
}