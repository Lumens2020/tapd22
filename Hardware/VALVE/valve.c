
#include "define.h"

/*
PB9 -- ��ˮ��
PB8 -- ������
PB7 -- �ȵ���
PB6 -- ���·�
PB5 -- ��ˮ��
*/
void Valve_Init(void)
{
	 
    rcu_periph_clock_enable(RCU_GPIOB);
    /* configure GPIO port */ 
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
	
    /*�õ�*/
    gpio_bit_reset(GPIOB, GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5);
	
}

