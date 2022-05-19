
#ifndef _ADC_H
#define _ADC_H

#include "stdint.h"


#define BOARD_ADC_CHANNEL          ADC_CHANNEL_1
#define ADC_GPIO_PORT              GPIOA
#define ADC_GPIO_PIN               GPIO_PIN_1

#define NTC_NUM  110

extern volatile INT8U  RealTemp;				                            

extern void Adc_Init(void);

extern void Get_Temperature_Task(void *pMsg);



#endif
