
#include "define.h"

/*
串口初始化
DMA初始化
参考文件：代码(验证成功)文件：DMA例程
*/


USART0_TASK_STRUCT  volatile Usart0TaskData;


/*模块结构体初始化*/
static void Usart0_Task_Struct_Init(void)
{
		
	memset((void *)&Usart0TaskData,0,sizeof(Usart0TaskData));
	
  Usart0TaskData.Snd.SndOk = TRUE;	 //发送缓存是空闲的
	
}


void Usart0_Task_Init(INT32U baud)
{
	  /*串口时钟使能、GPIOA使能、DMA1使能*/ 
	  rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
//	  rcu_periph_clock_enable(RCU_DMA);
	
	  usart_deinit(USART0);
	  /*设置串口波特率*/
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
	
	  /*串口使用收发*/
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);  //打开，否则不能发送数据
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_enable(USART0);
		
		/*外部中断配置*/
	  nvic_irq_enable(USART0_IRQn, 0);                       //中断优先级
		
    /* enable USART TBE interrupt */  
//    usart_interrupt_enable(USART0, USART_INT_TBE);       //串口发送中断
//   	usart_interrupt_disable(USART0, USART_INT_TBE);
		
		usart_interrupt_enable(USART0, USART_INT_RBNE);      //串口接收中断 
				
		Usart0_Task_Struct_Init();                          //结构体缓冲初始化
		 
    Mdu_Data_Struct_Init();
		
}


void USART0_IRQHandler(void)
{
    /*接收数据*/
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
		
		  /*发送数据*/
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


/*串口1发送缓冲区数据*/
void Usart0_Tx_Data(INT8U *str,INT8U len)
{
	   INT8U i = 0;

     INT16U timeout = 0;  //仿真看到一字节时间0x929
	
		 Usart1TaskData.Snd.SndOk = FALSE;	 //发送缓存忙
	
		 for( i=0; i<len; i++)
		{
        timeout = 0;
				usart_data_transmit(USART0, str[i]);
				while (RESET == usart_flag_get(USART0, USART_FLAG_TC))
				{
					
						if( ++timeout >= 0XFFF )   //超时判断
						{
							 timeout = 0;
							 break;
						}

				}
			
		}
		 Usart1TaskData.Snd.SndOk = TRUE;	 //发送缓存不忙
    
}






/*放主函数中*/
void Usart0_Rec_Task(void)
{
	  
    
		static  INT8U   *pSu8ReceBuffer;   //“指针切换关联”中的指针，切换内存
		static  INT8U   su8lock = 0;         //用来避免一直更新的临时变量
		static  INT8U   i = 0;                 //用在数据处理中的循环变量
		static  INT8U   su32receSize = 0;    //接收到的数据大小的临时变量
	  static  INT16U  checksum = 0;
	
	  if( Usart0TaskData.Rec.FeeDog )    //每被“喂一次狗”，就及时更新一次“超时检测的定时器”的初值
		{
			 Usart0TaskData.Rec.FeeDog = 0;
			 su8lock = 0; 
			 //以下三行代码是看门狗中的“喂狗”操作。继续给软件定时器“输血”
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
					 Usart0TaskData.Rec.Index = 1;    //以最快的速度先切换接收内存，避免丢失新发过来的数据   
					 Usart0TaskData.Rec.FinishFlag  = 0;
           pSu8ReceBuffer = (INT8U  *)&Usart0TaskData.Rec.BufA[0];  //关联刚刚接收的数据缓存
				   su32receSize   =  Usart0TaskData.Rec.CountA;
				   Usart0TaskData.Rec.CountA = 0;
			 }
			 else
			 {
				   Usart0TaskData.Rec.Index = 0;    //以最快的速度先切换接收内存，避免丢失新发过来的数据   
					 Usart0TaskData.Rec.FinishFlag  = 0;
           pSu8ReceBuffer = (INT8U  *)&Usart0TaskData.Rec.BufB[0];  //关联刚刚接收的数据缓存
				   su32receSize   =  Usart0TaskData.Rec.CountB;
				   Usart0TaskData.Rec.CountB = 0;
			 }
			 
			  /*处理接收的数据*/
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
		if( Usart0TaskData.Rec.TimeOutFlag && ( Usart0TaskData.Rec.TimeOutCnt  > 0 ) )  //通信过程中字节之间的超时定时器
		{
		   Usart0TaskData.Rec.TimeOutCnt--;
		}
}



/*
 *根据Usart0TaskData.Snd.SndLen以及Usart0TaskData.Snd.SndBuf
 *挂载到DMA中去发送
 */

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


