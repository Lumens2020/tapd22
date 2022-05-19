#ifndef _MOTOR_H
#define _MOTOR_H

#include "queue.h"



#define  MOTOR_ON_24V   gpio_bit_reset(GPIOA,GPIO_PIN_0)
#define  MOTOR_OFF_24V  gpio_bit_set(GPIOA,GPIO_PIN_0)

//#define  MOTOR_ON_36V   gpio_bit_reset(GPIOB,GPIO_PIN_0)  timer_channel_output_pulse_value_config(TIMER2, TIMER_CH_2, 2000 );
//#define  MOTOR_OFF_36V  gpio_bit_set(GPIOB,GPIO_PIN_0)


#define  RESHUI_MOTOR_ON_24V   gpio_bit_set(GPIOB,GPIO_PIN_4)
#define  RESHUI_MOTOR_OFF_24V  gpio_bit_reset(GPIOB,GPIO_PIN_4)

#define  M1_ON   (MOTOR_ON_24V,MOTOR_ON_36V)
#define  M1_OFF  (MOTOR_OFF_24V,MOTOR_OFF_36V)

/*生产者与消费者放在一个结构体*/
typedef struct
{
	QUEUE_S *pQ;
	INT8U   Order;
	
}RESHUI_MOTOR_PCMODEL;

extern RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelP1;
extern RESHUI_MOTOR_PCMODEL ReShuiMotorPcmodelC1;

extern void Motor_Init(void);


extern void ReShui_Motor_Producer(void *pMsg);
extern void ReShui_Motor_Consumer(void *pMsg);
	

#endif
