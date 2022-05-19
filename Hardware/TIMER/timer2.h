
#ifndef _TIMER2_H
#define _TIMER2_H


#include "stdint.h"


#define  MOTOR_ON_36V   timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 4500)
#define  MOTOR_OFF_36V  timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 0)


extern void Timer2_Pwm_Init(INT16U duty);


#endif
