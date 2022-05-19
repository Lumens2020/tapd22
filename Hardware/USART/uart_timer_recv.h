#ifndef _UART_TIMER_RECV_H
#define _UART_TIMER_RECV_H


#include "stdint.h"




typedef enum{
	
	Uart0Recv = 0,
	Uart1Recv,
	UartRecvNum,
	
}UART_T_RECV_E;

typedef struct{
  
	INT8U  Cnt;        //计数器
	INT8U  Index;      //当前接收个数
	INT8U  LastIndex;  //上次接收个数
	INT16U TimOutMax;  //超时时间
	INT16U MaxLen;     //
	INT8U  En;         //此种工作模式使能位
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
