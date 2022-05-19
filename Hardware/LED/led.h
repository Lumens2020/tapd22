#ifndef _LED_H
#define _LED_H

/*
HEAT_SWITCH_LED -- PC13
*/
//#define LED_PORT      GPIOB
//#define LED_PIN       GPIO_PIN_4
//#define LED_PORT_RCC  RCU_GPIOB


 

extern void Led_Init(void);
extern void Led_Toggle(void *pMsg);

#endif
