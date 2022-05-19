

#include "define.h"

UART_TIMER_RECV_STRUCT UartTimerRecvData;

/*每个串口对应一个定时器*/
void Uart_Timer_Recv_En(UART_T_RECV_E index,INT8U en)
{
	UART_RTIMER_T *ptmr;
	if(index < UartRecvNum)
	{
		ptmr = &UartTimerRecvData.Timer[index];
		ptmr->En = en;
	}
}

/*模块结构体初始化*/
void Uart_Timer_Recv_Struct_Init(void)
{
	 INT8U i;
	 UART_RTIMER_T *ptmr;
	 
	 for(i=0; i<UartRecvNum; i++ )
	{
		ptmr = &UartTimerRecvData.Timer[i];
		ptmr->Cnt = 0;
		ptmr->En = FALSE;
		ptmr->LastIndex = 0;
		ptmr->TimOutMax = 10;
		ptmr->MaxLen = 0xffff;
	}
	
	Uart_Timer_Recv_En(Uart0Recv,TRUE);  //串口0的定时器使能
	Uart_Timer_Recv_En(Uart1Recv,TRUE);  //串口1的定时器使能
	
	UartTimerRecvData.Timer[Uart0Recv].MaxLen = 256;
	UartTimerRecvData.Timer[Uart1Recv].MaxLen = 256;
}

void Uart_Timer_Recv_Init(void)
{
	Uart_Timer_Recv_Struct_Init();
}

/*
 *说明：	串口接收回调函数，注意此处不要逗留太长时间，若数据需要长期处理，请将buf中数据拷贝出，避免与DMA操作数据冲突！
 *参数:	
			index：串口通道标号
			buf:接收缓存数据
			len:接收缓存数据长度
 *返回：	无
 */
void Uart_Timer_Recv_Call_Back(INT8U index, INT8U *buf, INT8U len)
{
	INT8U i;
	if(index < UartRecvNum)
	{
		 switch(index)
		 {
			 case Uart0Recv:{
			  for(i=0; i<len; i++ )
				 {
					 // com_mul_rec_phy(buf[i]);  //取出接收的数据
				 }
			 };break;
				case Uart1Recv:{

			 };break;
			 
			 default:break;
		 }
	}
}

/*完成接收处理函数*/
void Uart_Timer_Recv_Ok(INT8U index)
{
	if(index < UartRecvNum)
	{
		      switch(index)
					{
	//				case Uart0Recv:Usart0_Manu_Recv_Ok();break;
	//		    case Uart1Recv:Usart1_Manu_Recv_Ok();break;
						default:break;
					}
	}
}

/*获得对应通道当前DMA计数值*/
INT32U Get_Uart_Curr_Index(INT8U index)
{
		if(index < UartRecvNum)
		{
			switch(index)
			{
				 case Uart0Recv: return Usart0_Curr_Rec_Counter();
				
				default:break;
			}
		}
		
		return 0;
}

/*1ms调用一次*/
void Uart_Timer_Recv_Tick(void)
{
	 UART_RTIMER_T *ptmr;
	 INT8U i;
	 for(i=0; i<UartRecvNum; i++)
	{
		ptmr = &UartTimerRecvData.Timer[i];
		if(TRUE == ptmr->En)
		{
			ptmr->Index = Get_Uart_Curr_Index(i);
			if(ptmr->Index > 0)
			{
				 if(ptmr->Index != ptmr->LastIndex)
				 {
					 ptmr->Cnt = 0;
					 ptmr->LastIndex = ptmr->Index;
				 }
				 else
				 {
					 ptmr->Cnt++;
					 if(  (ptmr->Cnt > ptmr->TimOutMax) || (ptmr->Index >= ptmr->MaxLen) )
					 {
						 Uart_Timer_Recv_Ok(i);  //完成接收处理
						 ptmr->Cnt = 0;
						 ptmr->LastIndex = 0;
					 }
				 }
			}
		}
	}
}