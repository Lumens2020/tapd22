
#include "define.h"

/*
���ڳ�ʼ��
DMA��ʼ��
�ο��ļ�������(��֤�ɹ�)�ļ���DMA����
*/


USART0_TASK_STRUCT  volatile Usart0TaskData;


/*ģ��ṹ���ʼ��*/
static void Usart0_Task_Struct_Init(void)
{
		
	memset((void *)&Usart0TaskData,0,sizeof(Usart0TaskData));
	
  Usart0TaskData.Snd.SndOk = TRUE;	 //���ͻ����ǿ��е�
	
}


void Usart0_Task_Init(INT32U baud)
{
	  /*����ʱ��ʹ�ܡ�GPIOAʹ�ܡ�DMA1ʹ��*/ 
	  rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
//	  rcu_periph_clock_enable(RCU_DMA);
	
	  usart_deinit(USART0);
	  /*���ô��ڲ�����*/
	  usart_baudrate_set(USART0,baud);
	  
	  /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);		
    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
	
	  /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_9);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP,GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ,GPIO_PIN_10);
	
	  /*����ʹ���շ�*/
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);  //�򿪣������ܷ�������
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);
		
		/*�ⲿ�ж�����*/
	  nvic_irq_enable(USART0_IRQn, 0);                       //�ж����ȼ�
		
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART0, USART_INT_TBE);       //���ڷ����ж�
//   	usart_interrupt_disable(USART0, USART_INT_TBE);
		
		usart_interrupt_enable(USART0, USART_INT_RBNE);      //���ڽ����ж� 
				
		Usart0_Task_Struct_Init();                          //�ṹ�建���ʼ��
		 
    Mdu_Data_Struct_Init();
		
}


void USART0_IRQHandler(void)
{
    /*��������*/
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
	  {
        Usart0TaskData.Rec.FinishFlag = 0;
			  Usart0TaskData.Rec.FeeDog = 1;	
			
        if( Usart0TaskData.Rec.Index == 0 )		
				{
					 if( Usart0TaskData.Rec.CountA < USART0_REC_BUF_SIZE )
					 {
						  Usart0TaskData.Rec.BufA[Usart0TaskData.Rec.CountA] =   usart_data_receive(USART0);
						  Usart0TaskData.Rec.CountA++;
					 }
				}
        else
				{
					 	if( Usart0TaskData.Rec.CountB < USART0_REC_BUF_SIZE )
					 {
						  Usart0TaskData.Rec.BufB[Usart0TaskData.Rec.CountB] =   usart_data_receive(USART0);
						  Usart0TaskData.Rec.CountB++;
					 }
				}					
     

    }
		
		  /*��������*/
//		 if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))
//		{
//        /* transmit data */
//       
//			  Usart0TaskData.Snd.SndByteFinishFlag = 1;
//			   usart_data_transmit(USART0, transmitter_buffer[txcount++]);
//        if(txcount == transfersize){
//            usart_interrupt_disable(USART0, USART_INT_TBE);
//         }

//    }


}


/*����1���ͻ���������*/
void Usart0_Tx_Data(INT8U *str,INT8U len)
{
	   INT8U i = 0;

     INT16U timeout = 0;  //���濴��һ�ֽ�ʱ��0x929
	
		 Usart1TaskData.Snd.SndOk = FALSE;	 //���ͻ���æ
	
		 for( i=0; i<len; i++)
		{
        timeout = 0;
				usart_data_transmit(USART0, str[i]);
				while (RESET == usart_flag_get(USART0, USART_FLAG_TC))
				{
					
						if( ++timeout >= 0XFFF )   //��ʱ�ж�
						{
							 timeout = 0;
							 break;
						}

				}
			
		}
		 Usart1TaskData.Snd.SndOk = TRUE;	 //���ͻ��治æ
    
}






/*����������*/
void Usart0_Rec_Task(void)
{
	  
    
		static  INT8U   *pSu8ReceBuffer;   //��ָ���л��������е�ָ�룬�л��ڴ�
		static  INT8U   su8lock = 0;         //��������һֱ���µ���ʱ����
		static  INT8U   i = 0;                 //�������ݴ����е�ѭ������
		static  INT8U   su32receSize = 0;    //���յ������ݴ�С����ʱ����
	  static  INT16U  checksum = 0;
	
	  if( Usart0TaskData.Rec.FeeDog )    //ÿ����ιһ�ι������ͼ�ʱ����һ�Ρ���ʱ���Ķ�ʱ�����ĳ�ֵ
		{
			 Usart0TaskData.Rec.FeeDog = 0;
			 su8lock = 0; 
			 //�������д����ǿ��Ź��еġ�ι���������������������ʱ������Ѫ��
       Usart0TaskData.Rec.TimeOutFlag = 0;
			 Usart0TaskData.Rec.TimeOutCnt  = DOG_TIME_OUT;
			 Usart0TaskData.Rec.TimeOutFlag = 1;
		}
		
		else if( ( su8lock == 0 ) && ( Usart0TaskData.Rec.TimeOutCnt == 0 ))
		{
			  su8lock = 1; 
			  Usart0TaskData.Rec.FinishFlag = 1;
			  
		}
		
		if(  Usart0TaskData.Rec.FinishFlag )
		{ 
			 if( Usart0TaskData.Rec.Index == 0 )
			 {
					 Usart0TaskData.Rec.Index = 1;    //�������ٶ����л������ڴ棬���ⶪʧ�·�����������   
					 Usart0TaskData.Rec.FinishFlag  = 0;
           pSu8ReceBuffer = (INT8U  *)&Usart0TaskData.Rec.BufA[0];  //�����ոս��յ����ݻ���
				   su32receSize   =  Usart0TaskData.Rec.CountA;
				   Usart0TaskData.Rec.CountA = 0;
			 }
			 else
			 {
				   Usart0TaskData.Rec.Index = 0;    //�������ٶ����л������ڴ棬���ⶪʧ�·�����������   
					 Usart0TaskData.Rec.FinishFlag  = 0;
           pSu8ReceBuffer = (INT8U  *)&Usart0TaskData.Rec.BufB[0];  //�����ոս��յ����ݻ���
				   su32receSize   =  Usart0TaskData.Rec.CountB;
				   Usart0TaskData.Rec.CountB = 0;
			 }
			 
			  /*������յ�����*/
			  if( ( pSu8ReceBuffer[0] == 0X9A ) && ( su32receSize == 11 ) )
				{
					  checksum = 0;
					  for(i=0; i<su32receSize-1; i++)
						{
							  checksum+= pSu8ReceBuffer[i];
						}
						
						checksum &= 0xff;
						
						if( pSu8ReceBuffer[su32receSize-1] == checksum )
						{
							   memcpy((void *)MduDataRx.RecBuf,pSu8ReceBuffer,su32receSize);
							   Mdu_Data_Rx_Exe_Func();
						}
						
				}
				else
				{
					 return;
				}
			
			 
			 
		}
	  
	
		
}

void Usart0_Rec_TimeOut_FeeDog(void)
{
		if( Usart0TaskData.Rec.TimeOutFlag && ( Usart0TaskData.Rec.TimeOutCnt  > 0 ) )  //ͨ�Ź������ֽ�֮��ĳ�ʱ��ʱ��
		{
		   Usart0TaskData.Rec.TimeOutCnt--;
		}
}



/*
 *����Usart0TaskData.Snd.SndLen�Լ�Usart0TaskData.Snd.SndBuf
 *���ص�DMA��ȥ����
 */

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


