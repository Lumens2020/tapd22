
#include "define.h"


static INT8U  ScrHeatFlag = 0;
static INT16U ScrHeatCount = 0;

TIMER14_S volatile Timer14S;

static void Timer14_Data_Struct_Init(void)
{
	 memset((void *)&Timer14S,0,sizeof(Timer14S));
	
}


void Timer14_Init(INT16U prescaler,INT16U period)
{
	  timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;
	  /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER14);
	
	   /* TIMER14 configuration */
    timer_deinit(TIMER14);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = prescaler - 1;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = period - 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER14, &timer_initpara);
	  
	  nvic_irq_enable(TIMER14_IRQn, 3U);
		timer_interrupt_enable(TIMER14, TIMER_INT_UP);
	
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER14);
    /* auto-reload preload enable */
    timer_disable(TIMER14);               //��ʱ����ʹ��
		
		Timer14_Data_Struct_Init();
		
}
/*�����ɿع����IO
PA12 -- SCR_HOT
*/
void Scr_Heat_Init(void)
{
		/* enable the HEAT_ GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure HEAT_SWITCH_LED GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_12);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);
	
    /* set HEAT_ GPIO pin */
    gpio_bit_reset(GPIOA,GPIO_PIN_12);
	

	

}

/*�̵�������IO
PB3 -- Relay_Heat
*/

void Relay_Heat_Init(void)
{
	 		/* enable the HEAT_ GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure HEAT_SWITCH_LED GPIO port */ 
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
	
    /* set HEAT_ GPIO pin */
    gpio_bit_reset(GPIOB,GPIO_PIN_3);
}
/*
�ɼ����Ĳ��Σ�7.5MS�ߵ�ƽ��2.5MS�͵�ƽ(����ͨ�������Ŷ�������ɵ�)		
1000W:����570,575
1500W:����608,615
*/
void TIMER14_IRQHandler(void) 
{
	  timer_flag_clear(TIMER14,TIMER_FLAG_UP);
	 /*
	 ���ֲ�����ʽ220V���λ�����
	  if( ScrHeatFlag == TRUE )
		{
				ScrHeatCount++;
		

			 	 if( ScrHeatCount >= 613) 
			  {
				        SCR_HEAT_ON;
					   
					     if(ScrHeatCount >= 620)
							 {
								
									SCR_HEAT_OFF;
							
	
									ScrHeatFlag = FALSE;
								 	ScrHeatCount = 0;	
                	timer_disable(TIMER14);               //��ʱ����ʹ��								 
									nvic_irq_enable(EXTI4_15_IRQn, 3U);
								 
								
							 }
	
			  }
				
			 
		}
*/

    if( Timer14S.ScrHeatFlag )
		{
				 Timer14S.ScrCount++;

			  if(  Timer14S.ScrCount >= 608) 
			  {
				        SCR_HEAT_ON;
					    
					     if(Timer14S.ScrCount >= 615)
							 {
								 
									SCR_HEAT_OFF;
									Timer14S.ScrHeatFlag = 0;
									Timer14S.ScrCount = 0;
								  timer_counter_value_config(TIMER14, 0);
									timer_disable(TIMER14);               //��ʱ����ʹ��
									nvic_irq_enable(EXTI4_15_IRQn, 3U);
								 
								
							 }
	
			  }
							 
		}
    
   
}

void Timer14_Scr_Heat_Flag(INT8U val)
{
	  if(val == TRUE)
		ScrHeatFlag = TRUE;
		else
		ScrHeatFlag = FALSE;
}



/*����+����*/
static void Relay_Heat_Keep(INT8U realtemp,INT8U heattemp,INT8U lowtemp,INT8U keeptemp,INT8U exceedtemp )
{
			static INT8U count = 0,heatflag = 0;
	
      if( heatflag == 0 )
			{
					if( realtemp >= exceedtemp  )
					{							
								 nvic_irq_disable(EXTI4_15_IRQn);       //��ʹ��
								 timer_disable(TIMER14);                //��ʱ����ʹ��					
									
								 RELAY_HEAT_OFF;						
												 
					}
					else if( realtemp >= lowtemp && realtemp <= keeptemp )
					{
								 nvic_irq_enable(EXTI4_15_IRQn, 3U);   //ʹ���ⲿ�ж�
					}
					else if( realtemp < lowtemp )
					{
						      heatflag = 1;
									nvic_irq_disable(EXTI4_15_IRQn);       //��ʹ��
									timer_disable(TIMER14);                //��ʱ����ʹ��
						
									RELAY_HEAT_ON;
					}			
					
		  }
			else
			{
				  	if( realtemp >= heattemp  )
					 {
							count++;
							if( count >= 1 )    //2���ж�
							{
								 count    = 0;				
								 heatflag = 0;	
								
								 RELAY_HEAT_OFF;						
							}		
			     }
					 
		  }
			
			/*�¶ȱ���*/
			if( realtemp > BOIL_POINT_97C )
			{
				
				  gpio_bit_reset(GPIOC,GPIO_PIN_13);  //ͨ�Ų���
				  RELAY_HEAT_OFF;	
					nvic_irq_disable(EXTI4_15_IRQn);       //��ʹ��
					timer_disable(TIMER14);                //��ʱ����ʹ��	
			}
			
}


/*����������*/
void Realy_Heat_Task(void *pMsg)
{
	   pMsg = pMsg;
	
	   INT8U temp =  DeviceInfor.InControl.Keep.SetTemp;
	
	   if( DeviceInfor.OutControl.ReDan.StopHeatFlag == TRUE )
		 {
			    //45�ȼ���
				 if(  LOW_LEVEL_READ  )
				 {

					   switch( temp )
						 {
							 
							   case KEEP_TEMP_95C:{
									          Relay_Heat_Keep(RealTemp,HOT_WATER_HEAT_93,HOT_WATER_LOW_92,HOT_WATER_KEEP_95,HOT_WATER_EXCEED_96 );
							          }break;
								 
								 case KEEP_TEMP_60C:{
									          Relay_Heat_Keep(RealTemp,HOT_WATER_HEAT_58,HOT_WATER_LOW_57,HOT_WATER_KEEP_60,HOT_WATER_EXCEED_61 );
							          }break;
								 
								 case KEEP_TEMP_45C:{
									          Relay_Heat_Keep(RealTemp,HOT_WATER_HEAT_43,HOT_WATER_LOW_42,HOT_WATER_KEEP_45,HOT_WATER_EXCEED_46 );
							          }break;
								 
								 default:break;
												
						 }
			
				 }
				 else
				 {
			 					 
						RELAY_HEAT_OFF;
						RESHUI_MOTOR_OFF_24V;		
				 }
		 }
		 else
		 {
			     RELAY_HEAT_OFF; 
		 }


		 
//		 /*��Һλ��ȡ��0���ÿ�*/
//		 if( HIGH_LEVEL_READ == 0 )
//		 {
//			   RESHUI_MOTOR_ON_24V;
//		 }
//		 else
//		 {
//			   RESHUI_MOTOR_OFF_24V;
//		 }
		   
}
