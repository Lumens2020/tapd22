
#include "define.h"

/*
PA11 --- ZERO
*/
void Exti_Init(void)
{
	
		rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_CFGCMP);

    /* configure button pin as input */
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,GPIO_PIN_11);


		/* enable and set key EXTI interrupt to the lowest priority */
//		nvic_irq_enable(EXTI4_15_IRQn, 3U);
     nvic_irq_disable(EXTI4_15_IRQn);  //不使能

		/* connect key EXTI line to key GPIO pin */
		syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN11);

		/* configure key EXTI line */
//		exti_init(EXTI_11, EXTI_INTERRUPT, EXTI_TRIG_FALLING);  //下降沿

	  exti_init(EXTI_11, EXTI_INTERRUPT, EXTI_TRIG_RISING);
		exti_interrupt_flag_clear(EXTI_11);
	
}

/*捕获下降沿*/
void EXTI4_15_IRQHandler(void)
{
	  if( exti_interrupt_flag_get(EXTI_11) != RESET)
		{
			
			  exti_interrupt_flag_clear(EXTI_11);
		
				nvic_irq_disable(EXTI4_15_IRQn);
			
			  Timer14S.ScrHeatFlag = 1;        //全局变量，原子操作
			
			  timer_counter_value_config(TIMER14, 0); 
			
			  timer_enable(TIMER14); 
	  
		}
		
}