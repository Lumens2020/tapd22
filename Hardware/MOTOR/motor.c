#include "define.h"


RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelP1 = {&Queue,0};
RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelC1 = {&Queue,0};
/*
PA0 -- 24v制水泵
PB0 -- 36v制水泵
PB4 -- 热水泵
*/
void Motor_Init(void)
{
	  /* enable the HEAT_SWITCH_LED GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure HEAT_SWITCH_LED GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
	
    /* set HEAT_SWITCH_LED GPIO pin */
    gpio_bit_set(GPIOA,GPIO_PIN_0);
	

		 /* enable the HEAT_SWITCH_LED GPIO clock */
//    rcu_periph_clock_enable(RCU_GPIOB);
//    /* configure HEAT_SWITCH_LED GPIO port */ 
//    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0);
//    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
//	
//    /* set HEAT_SWITCH_LED GPIO pin */
//    gpio_bit_set(GPIOB,GPIO_PIN_0);

	
		  /* enable the HEAT_SWITCH_LED GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure HEAT_SWITCH_LED GPIO port */ 
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	
    /* set HEAT_SWITCH_LED GPIO pin */
    gpio_bit_reset(GPIOB,GPIO_PIN_4);
}


/*生产者数据存入队列*/
void ReShui_Motor_Producer(void *pMsg)
{
	
		RESHUI_MOTOR_PCMODEL *p = (RESHUI_MOTOR_PCMODEL *)pMsg;  //类型转换
		
			
			 /*队列没有满*/
			 if( Is_Queue_Full(p->pQ) == FALSE )  //TRUE是满，FALSE是不满
			 {
				  Save_Queue_Dat(p->pQ,p->Order);  //存数据

			 }

			 
			  
	
}

/*消费者读取队列内的数据*/
void ReShui_Motor_Consumer(void *pMsg)
{
		RESHUI_MOTOR_PCMODEL *p = (RESHUI_MOTOR_PCMODEL *)pMsg;  //类型转换
	 /*队列内是否有数据*/
	if(Queue_Num(p->pQ))
	{  
		 /*进行一些应用计算，PID,EEPROM*/
		 p->Order = Get_QUEUE_Dat(p->pQ);  //取出队列内的数据存入到buf中
		 
		 if( p->Order )
		 {
//			  RESHUI_MOTOR_ON_24V;
		 }
	}
}