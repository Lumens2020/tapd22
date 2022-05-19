
#include "define.h"

static INT8U G_Comm_Snd_Alloc_Index = 0;

COMM_SND_T CommSndBuf[COMM_SND_BUF_SIZE];

COMM_SND_T *PMduSnd;    //MDU�����ָ��MDUһ�οռ�

//COMM_SND_T *PRfidSnd;   //RFID�����ָ��RFIDһ�οռ�

/*��æ*/
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

/*�������ݰ�*/
INT8U Comm_Snd_Packet(COMM_SND_T *pSnd,INT8U *src,INT8U len)
{
	  INT8U bret  = FALSE; 
	  if( pSnd->pfSnd != (FUNC_HD_SND *)0 )
		{
			if( FALSE == pSnd->Tmr.En )  //ֻ�в�æ��ʱ����Է���,FALSEΪ��æ
			{
				 pSnd->Tmr.Max = pSnd->Tmr.TimeOut;
				 bret = pSnd->pfSnd(src,len);  //�ص�����ײ㣺Usart1_Dma_Snd
				
				if(TRUE == bret)          //���ͳɹ�
				 {
					 pSnd->Tmr.Cnt = 0;
					 pSnd->Tmr.En = TRUE; //���ö�ʱ����æ
				 }
			}
		}
	 
		return bret;
}

/*�ͷŷ��;��*/
void Comm_Snd_Release(COMM_SND_T *pSnd)
{
	 pSnd->Tmr.En = FALSE;
}


/*���ó�ʱ��������ʱ��ʱʱ�䣬�����ڽ��ջش�ʱ����Ҫ�������ó�ʱʱ�䣬����������ݱ����ȳ�����ʱ����*/
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


/*�������;��*/
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
				/*��Ӧ�����Ļص���ʼ��*/
				 pBuf->pfSnd = pfSnd;
				 pBuf->pfTimeOut = pfTimeOut;
				 
				 /*��ʱ����ʼ��*/
				 pBuf->Tmr.En = FALSE;
				 pBuf->Tmr.Cnt = 0;
				 pBuf->Tmr.TimeOut = timeout;
				 pBuf->Tmr.Max = timeout;
				 
			}
			
		}
		
		return pBuf;
}

/*
���ݷ��ͳ�ʱ�����������˴���0�ʹ���1
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
								 /*��ʱ����*/
								 pSnd->pfTimeOut();  //��ת��void Mdu_Send_TimeOut(void)
							 } 
					 }
				}
		}
}