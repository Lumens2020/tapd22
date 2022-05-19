#ifndef _UART_TIMER_RECV_H
#define _UART_TIMER_RECV_H


#include "stdint.h"




typedef enum{
	
	Uart0Recv = 0,
	Uart1Recv,
	UartRecvNum,
	
}UART_T_RECV_E;

typedef struct{
  
	INT8U  Cnt;        //������
	INT8U  Index;      //��ǰ���ո���
	INT8U  LastIndex;  //�ϴν��ո���
	INT16U TimOutMax;  //��ʱʱ��
	INT16U MaxLen;     //
	INT8U  En;         //���ֹ���ģʽʹ��λ
}UART_RTIMER_T;

typedef struct{
	
	INT8U Status;
	UART_RTIMER_T Timer[5];
	
}UART_TIMER_RECV_STRUCT;

extern  UART_TIMER_RECV_STRUCT UartTimerRecvData;


extern void Uart_Timer_Recv_Init(void);
extern void Uart_Timer_Recv_Tick(void);
extern void Uart_Timer_Recv_Call_Back(INT8U index, INT8U *buf, INT8U len);

#endif
