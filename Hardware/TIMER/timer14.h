
#ifndef _TIMER14_H
#define _TIMER14_H


#include "stdint.h"

#define  SCR_HEAT_OFF   gpio_bit_reset(GPIOA,GPIO_PIN_12)
#define  SCR_HEAT_ON    gpio_bit_set(GPIOA,GPIO_PIN_12)


#define  RELAY_HEAT_ON     gpio_bit_set(GPIOB,GPIO_PIN_3)
#define  RELAY_HEAT_OFF    gpio_bit_reset(GPIOB,GPIO_PIN_3)

/*95¶È*/
#define  HOT_WATER_LOW_92      91
#define  HOT_WATER_HEAT_93     92
#define  HOT_WATER_KEEP_95     93
#define  HOT_WATER_EXCEED_96   94

/*60¶È*/
#define  HOT_WATER_LOW_57      57
#define  HOT_WATER_HEAT_58     57
#define  HOT_WATER_KEEP_60     59
#define  HOT_WATER_EXCEED_61   60

/*45¶È*/
#define  HOT_WATER_LOW_42      42
#define  HOT_WATER_HEAT_43     42
#define  HOT_WATER_KEEP_45     44
#define  HOT_WATER_EXCEED_46   45


/*
#define  HOT_WATER_95     58
#define  HOT_WATER_HIGH   61
#define  HOT_WATER_LOW    58
#define  HOT_WATER_LOWER  55
*/

/*
#define  HOT_WATER_95     43
#define  HOT_WATER_HIGH   46
#define  HOT_WATER_LOW    43
#define  HOT_WATER_LOWER  40
*/


typedef struct{

	INT8U    ScrHeatFlag;
	INT16U   ScrCount;
	INT8U    ScrCycleFlag;
	INT8U    StartKeepWarmFlag;
	INT8U    StopKeepWarmFlag;
	
}TIMER14_S;

extern TIMER14_S volatile Timer14S;

extern void Timer14_Init(INT16U prescaler,INT16U period);
extern void Scr_Heat_Init(void);
extern void Relay_Heat_Init(void);
extern void Timer14_Scr_Heat_Flag(INT8U val);
extern void Realy_Heat_Task(void *pMsg);


#endif
