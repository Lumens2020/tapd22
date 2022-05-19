
#include "define.h"

/*
����1��DMAû�е��Գɹ������ô���1�շ��жϣ�2022.04.08
*/


USART1_TASK_STRUCT volatile Usart1TaskData;


/*ģ��ṹ���ʼ��*/
void Usart1_Task_Struct_Init(void)
{
		INT32U i;
	  
	  memset((void *)&Usart1TaskData,0,sizeof(Usart1TaskData));
	
		Usart1TaskData.Snd.SndOk = TRUE;	 //���ͻ����ǿ��е�
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
	  /*����ʱ��ʹ�ܡ�GPIOAʹ�ܡ�DMA1ʹ��*/ 
	  rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART1);
//	  rcu_periph_clock_enable(RCU_DMA);
	
	  usart_deinit(USART1);
	  /*���ô��ڲ�����*/
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
	
	  /*����ʹ���շ�*/
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);  //�򿪣������ܷ�������
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1);
		
		/*�ⲿ�ж�����*/
	  nvic_irq_enable(USART1_IRQn, 0);                         //�ж����ȼ�
		
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART1, USART_INT_TBE);           //���ڷ����ж�
		
		usart_interrupt_enable(USART1, USART_INT_RBNE);            //���ڽ����ж� 
			

				
		Usart1_Task_Struct_Init();                               //�ṹ�建���ʼ��
		
		Rfid_Data_Struct_Init();                                 //RFID�ṹ�����ݳ�ʼ��
		 		
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
			  /*�û����ݴ���*/
			 /*���Ƹ�RFID_data,ͬʱ���DMA����*/
				
 			 Rfid_Data_Rx_Ok(Usart1TaskData.Rec.Buf[i],Usart1TaskData.Rec.Len[i]);
			 memset((void *)Usart1TaskData.Rec.Buf[i],0,USART1_REC_BUF_SIZE);      //��֤ÿ��������0
			
		   Usart1TaskData.Rec.RecOk[i] = FALSE;
		 }
		 
	 
	}
		
}

/*����1�����¼�*/
void Usart1_Send_Task(void *pMsg)
{
	   pMsg = pMsg;   
	   Rfid_Data_Update_Infor();  
}


/*
����1�����ж�
*/
void USART1_IRQHandler(void)
{
	
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
		{
			
//			  usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
        /* receive data */
        Usart1TaskData.Rec.RxBuf[Usart1TaskData.Rec.RxCount] = usart_data_receive(USART1);  //�ѽ��յ������ݱ��浽Usart1TaskData.Rec.RxBuf��
			  
			  if( Usart1TaskData.Rec.RxCount == 0 )
				{
					 timer_enable(TIMER15);  //ʹ��
				}
				else
        {
					 timer_counter_value_config(TIMER15, 0); //��0
				}
				
				Usart1TaskData.Rec.RxCount++;
					
    }


}


/*����1���ͻ���������*/
void Usart1_Tx_Data(INT8U *str,INT8U len)
{
		 INT8U i = 0;
	   INT8U timeout = 0;

 
		 Usart1TaskData.Snd.SndOk = FALSE;	 //���ͻ���æ
	
		 for( i=0; i<len; i++)
		{
        timeout = 0;
				usart_data_transmit(USART1, str[i]);
				while(RESET == usart_flag_get(USART1, USART_FLAG_TC))
				{
					
						if( ++timeout >= 0XFF )   //��ʱ�ж�
						{
							 timeout = 0;
							 break;
						}

				}
			
		}
		 Usart1TaskData.Snd.SndOk = TRUE;	 //���ͻ��治æ
    
}

