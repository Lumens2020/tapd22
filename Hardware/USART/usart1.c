
#include "define.h"

/*
串口1的DMA没有调试成功，改用串口1收发中断，2022.04.08
*/


USART1_TASK_STRUCT volatile Usart1TaskData;


/*模块结构体初始化*/
void Usart1_Task_Struct_Init(void)
{
		INT32U i;
	  
	  memset((void *)&Usart1TaskData,0,sizeof(Usart1TaskData));
	
		Usart1TaskData.Snd.SndOk = TRUE;	 //发送缓存是空闲的
		Usart1TaskData.Rec.Index = 0; 
	  Usart1TaskData.Rec.RxCount = 0;
		for(i = 0;i < USART1_REC_MUL_NUM;i ++)
		{
			Usart1TaskData.Rec.RecOk[i] = FALSE;
		}
}




/*
  USART1_Tx -- PA2
	USART1_Rx -- PA3
*/
void Usart1_Task_Init(INT32U baud)
{
	  /*串口时钟使能、GPIOA使能、DMA1使能*/ 
	  rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);
//	  rcu_periph_clock_enable(RCU_DMA);
	
	  usart_deinit(USART1);
	  /*设置串口波特率*/
	  usart_baudrate_set(USART1,baud);
	  
	  /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);		
    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);
	
	  /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_3);
	
	  /*串口使用收发*/
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);  //打开，否则不能发送数据
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1);
		
		/*外部中断配置*/
	  nvic_irq_enable(USART1_IRQn, 0);                         //中断优先级
		
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART1, USART_INT_TBE);           //串口发送中断
		
		usart_interrupt_enable(USART1, USART_INT_RBNE);            //串口接收中断 
			

				
		Usart1_Task_Struct_Init();                               //结构体缓冲初始化
		
		Rfid_Data_Struct_Init();                                 //RFID结构体数据初始化
		 		
}


/*100MS*/
void Usart1_Rec_Task(void *pMsg)
{
	INT8U i = 0;
	pMsg = pMsg;
	for(i=0; i<USART1_REC_MUL_NUM; i++)
	{
		
		 if(TRUE == Usart1TaskData.Rec.RecOk[i] )
		 {
			  /*用户数据处理*/
			 /*复制给RFID_data,同时清除DMA缓存*/
				
 			 Rfid_Data_Rx_Ok(Usart1TaskData.Rec.Buf[i],Usart1TaskData.Rec.Len[i]);
			 memset((void *)Usart1TaskData.Rec.Buf[i],0,USART1_REC_BUF_SIZE);      //保证每次数据清0
			
		   Usart1TaskData.Rec.RecOk[i] = FALSE;
		 }
		 
	 
	}
		
}

/*串口1发送事件*/
void Usart1_Send_Task(void *pMsg)
{
	   pMsg = pMsg;   
	   Rfid_Data_Update_Infor();  
}


/*
串口1接收中断
*/
void USART1_IRQHandler(void)
{
	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
		{
			
//			  usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
        /* receive data */
        Usart1TaskData.Rec.RxBuf[Usart1TaskData.Rec.RxCount] = usart_data_receive(USART1);  //把接收到的数据保存到Usart1TaskData.Rec.RxBuf中
			  
			  if( Usart1TaskData.Rec.RxCount == 0 )
				{
					 timer_enable(TIMER15);  //使能
				}
				else
        {
					 timer_counter_value_config(TIMER15, 0); //清0
				}
				
				Usart1TaskData.Rec.RxCount++;
					
    }


}


/*串口1发送缓冲区数据*/
void Usart1_Tx_Data(INT8U *str,INT8U len)
{
		 INT8U i = 0;
	   INT8U timeout = 0;

 
		 Usart1TaskData.Snd.SndOk = FALSE;	 //发送缓存忙
	
		 for( i=0; i<len; i++)
		{
        timeout = 0;
				usart_data_transmit(USART1, str[i]);
				while(RESET == usart_flag_get(USART1, USART_FLAG_TC))
				{
					
						if( ++timeout >= 0XFF )   //超时判断
						{
							 timeout = 0;
							 break;
						}

				}
			
		}
		 Usart1TaskData.Snd.SndOk = TRUE;	 //发送缓存不忙
    
}

