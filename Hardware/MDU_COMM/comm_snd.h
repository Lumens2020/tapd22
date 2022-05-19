
#ifndef _COMM_SND_H
#define _COMM_SND_H


#include "stdint.h"

#define COMM_SND_BUF_SIZE 4   //发送句柄数

typedef INT8U FUNC_HD_SND(INT8U *src,INT8U len);
typedef void  FUNC_SND_TIMEOUT(void);

/*超时计数器*/
typedef struct{
  INT8U En;
	INT8U Cnt;
	INT8U Max;
	INT8U TimeOut;
}Snd_Timer_T;


/*通信发送管理结构体*/
typedef struct{

	Snd_Timer_T       Tmr;
	FUNC_HD_SND       *pfSnd;
	FUNC_SND_TIMEOUT  *pfTimeOut;
	
}COMM_SND_T;


extern  COMM_SND_T CommSndBuf[COMM_SND_BUF_SIZE];

extern  COMM_SND_T *PMduSnd;

extern  COMM_SND_T *PRfidSnd;

extern INT8U Is_Comm_Snd_Busy(COMM_SND_T *pSnd);

extern INT8U Comm_Snd_Packet(COMM_SND_T *pSnd,INT8U *src,INT8U len);

extern void Comm_Snd_Release(COMM_SND_T *pSnd);

extern void Comm_Snd_Set_Cnt_Dly(COMM_SND_T *pSnd,INT8U val);

extern COMM_SND_T *Comm_Snd_Create(FUNC_HD_SND *pfSnd,FUNC_SND_TIMEOUT *pfTimeOut, INT16U timeout);

extern void Comm_Snd_Tick(void);


#endif
