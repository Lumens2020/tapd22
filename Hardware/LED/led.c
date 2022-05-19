#include "define.h"

/*
HEAT_SWITCH_LED -- PC13
*/
void Led_Init(void)
{
	  /* enable the HEAT_SWITCH_LED GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* configure HEAT_SWITCH_LED GPIO port */ 
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_13);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
	
    /* set HEAT_SWITCH_LED GPIO pin */
    gpio_bit_set(GPIOC,GPIO_PIN_13);
	
}

void Led_Toggle(void *pMsg)
{
	 pMsg = pMsg;
	 gpio_bit_toggle(GPIOC,GPIO_PIN_13);
}