
#include "define.h"

/*
PA7 -- TIMER2 CH1
PB0 -- TIMER2_CH2
*/
void Gpio_Config(void)
{

//    /* enable the GPIOA clock */
//    rcu_periph_clock_enable(RCU_GPIOA);

//	   /*configure PB0(TIMER2 CH2) as alternate function*/
////    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
//	  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
//    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_7);
//    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_7);
	
	
	///* enable the GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOB);

	   /*configure PB0(TIMER2 CH2) as alternate function*/
//    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
	  gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_0);
	  
}

void Timer2_Pwm_Init(INT16U duty)
{
	 /* -----------------------------------------------------------------------
    TIMER2 configuration: generate 3 PWM signals with 3 different duty cycles:
    TIMER2CLK = SystemCoreClock / 72 = 1MHz, the PWM frequency is 1KHz.

    TIMER2 channel0 duty cycle = (500/ 1000)* 100  = 50%

    ----------------------------------------------------------------------- */
	  Gpio_Config();
	
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER2);
    /* deinit a TIMER */
    timer_deinit(TIMER2);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 4999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* configure TIMER channel output function */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_LOW;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER2, TIMER_CH_2, &timer_ocinitpara);
	
		/* CH2 configuration in PWM mode0, duty cycle 66.6% */
//    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_1, duty );
//    timer_channel_output_mode_config(TIMER2, TIMER_CH_1, TIMER_OC_MODE_PWM0);
//    timer_channel_output_shadow_config(TIMER2, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);
    timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, duty );
    timer_channel_output_mode_config(TIMER2, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER2, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);
    /* auto-reload preload enable */
    timer_enable(TIMER2);
}

