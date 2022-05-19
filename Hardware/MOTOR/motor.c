#include "define.h"


RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelP1 = {&Queue,0};
RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelC1 = {&Queue,0};
/*
PA0 -- 24v��ˮ��
PB0 -- 36v��ˮ��
PB4 -- ��ˮ��
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


/*���������ݴ������*/
void ReShui_Motor_Producer(void *pMsg)
{
	
		RESHUI_MOTOR_PCMODEL *p = (RESHUI_MOTOR_PCMODEL *)pMsg;  //����ת��
		
			
			 /*����û����*/
			 if( Is_Queue_Full(p->pQ) == FALSE )  //TRUE������FALSE�ǲ���
			 {
				  Save_Queue_Dat(p->pQ,p->Order);  //������

			 }

			 
			  
	
}

/*�����߶�ȡ�����ڵ�����*/
void ReShui_Motor_Consumer(void *pMsg)
{
		RESHUI_MOTOR_PCMODEL *p = (RESHUI_MOTOR_PCMODEL *)pMsg;  //����ת��
	 /*�������Ƿ�������*/
	if(Queue_Num(p->pQ))
	{  
		 /*����һЩӦ�ü��㣬PID,EEPROM*/
		 p->Order = Get_QUEUE_Dat(p->pQ);  //ȡ�������ڵ����ݴ��뵽buf��
		 
		 if( p->Order )
		 {
//			  RESHUI_MOTOR_ON_24V;
		 }
	}
}