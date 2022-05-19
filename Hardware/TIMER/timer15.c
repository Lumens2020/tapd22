
#include "define.h"


void Timer15_Init(INT16U prescaler,INT16U period)
{
	  timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;
	  /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER15);
	
	   /* TIMER15 configuration */
    timer_deinit(TIMER15);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = prescaler - 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = period - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER15, &timer_initpara);
	  
	  nvic_irq_enable(TIMER15_IRQn, 3U);
		timer_interrupt_enable(TIMER15, TIMER_INT_UP);
	
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER15);
    /* auto-reload preload enable */
    timer_disable(TIMER15);               //关闭定时器15
	
}


/*100MS进入一次中断*/
void TIMER15_IRQHandler(void)
{	
	  usart_interrupt_disable(USART1, USART_INT_RBNE);            //关闭串口接收中断 
	 
		timer_flag_clear(TIMER15,TIMER_FLAG_UP);
    timer_disable(TIMER15);                  //关闭定时器15
	  timer_counter_value_config(TIMER15, 0);  //清0
	 
	  
		/*一维数组赋值给二维数组*/
    memcpy( (void *)Usart1TaskData.Rec.Buf[Usart1TaskData.Rec.Index],(void *)Usart1TaskData.Rec.RxBuf,Usart1TaskData.Rec.RxCount );	
	
//	  memset((void *)Usart1TaskData.Rec.RxBuf,0,sizeof(Usart1TaskData.Rec.RxBuf));
	  Usart1TaskData.Rec.Len[Usart1TaskData.Rec.Index] = Usart1TaskData.Rec.RxCount;     //赋值接收的长度
	  
	  
	
	  Usart1TaskData.Rec.RxCount = 0;
	 
	  Usart1TaskData.Rec.RecOk[Usart1TaskData.Rec.Index] = TRUE; //第一个缓存
	 
	  Usart1TaskData.Rec.Index++;
	
	  Usart1TaskData.Rec.Index = Usart1TaskData.Rec.Index%USART1_REC_MUL_NUM;
		
	  Usart1TaskData.Rec.RecOk[Usart1TaskData.Rec.Index] = FALSE; //第二个缓存
	  
		
		usart_interrupt_enable(USART1, USART_INT_RBNE);            //串口接收中断 
	
}