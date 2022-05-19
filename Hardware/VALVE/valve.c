
#include "define.h"

/*
PB9 -- 废水阀
PB8 -- 回流阀
PB7 -- 热胆阀
PB6 -- 常温阀
PB5 -- 进水阀
*/
void Valve_Init(void)
{
	 
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure GPIO port */ 
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
	
    /*置低*/
    gpio_bit_reset(GPIOB, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
	
}

