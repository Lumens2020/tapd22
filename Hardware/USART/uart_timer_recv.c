

#include "define.h"

UART_TIMER_RECV_STRUCT UartTimerRecvData;

/*ÿ�����ڶ�Ӧһ����ʱ��*/
void Uart_Timer_Recv_En(UART_T_RECV_E index,INT8U en)
{
	UART_RTIMER_T *ptmr;
	if(index < UartRecvNum)
	{
		ptmr = &UartTimerRecvData.Timer[index];
		ptmr->En = en;
	}
}

/*ģ��ṹ���ʼ��*/
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
	
	Uart_Timer_Recv_En(Uart0Recv,TRUE);  //����0�Ķ�ʱ��ʹ��
	Uart_Timer_Recv_En(Uart1Recv,TRUE);  //����1�Ķ�ʱ��ʹ��
	
	UartTimerRecvData.Timer[Uart0Recv].MaxLen = 256;
	UartTimerRecvData.Timer[Uart1Recv].MaxLen = 256;
}

void Uart_Timer_Recv_Init(void)
{
	Uart_Timer_Recv_Struct_Init();
}

/*
 *˵����	���ڽ��ջص�������ע��˴���Ҫ����̫��ʱ�䣬��������Ҫ���ڴ����뽫buf�����ݿ�������������DMA�������ݳ�ͻ��
 *����:	
			index������ͨ�����
			buf:���ջ�������
			len:���ջ������ݳ���
 *���أ�	��
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
					 // com_mul_rec_phy(buf[i]);  //ȡ�����յ�����
				 }
			 };break;
				case Uart1Recv:{

			 };break;
			 
			 default:break;
		 }
	}
}

/*��ɽ��մ�����*/
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

/*��ö�Ӧͨ����ǰDMA����ֵ*/
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

/*1ms����һ��*/
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
						 Uart_Timer_Recv_Ok(i);  //��ɽ��մ���
						 ptmr->Cnt = 0;
						 ptmr->LastIndex = 0;
					 }
				 }
			}
		}
	}
}