
#ifndef _VALVE_H
#define _VALVE_H


/*
PB9 -- 废水阀
PB8 -- 回流阀
PB7 -- 热胆阀
PB6 -- 常温阀
PB5 -- 进水阀
*/
#define  FEISHUI_VALVE_ON    gpio_bit_set(GPIOB,GPIO_PIN_9)
#define  FEISHUI_VALVE_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_9)

#define  HUILIU_VALVE_ON    gpio_bit_set(GPIOB,GPIO_PIN_8)
#define  HUILIU_VALVE_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_8)

#define  REDAN_VALVE_ON    gpio_bit_set(GPIOB,GPIO_PIN_7)
#define  REDAN_VALVE_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_7)

#define  CHANGWEN_VALVE_ON    gpio_bit_set(GPIOB,GPIO_PIN_6)
#define  CHANGWEN_VALVE_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_6)

#define  JINSHUI_VALVE_ON    gpio_bit_set(GPIOB,GPIO_PIN_5)
#define  JINSHUI_VALVE_OFF   gpio_bit_reset(GPIOB,GPIO_PIN_5)

/*
F1-JINSHUI
F2-CHANGWEN
F3-REDAN
F4-HUILIU
F5-FEISHUI
*/
#define  F1_VALVE_ON   JINSHUI_VALVE_ON
#define  F1_VALVE_OFF  JINSHUI_VALVE_OFF

#define  F2_VALVE_ON   CHANGWEN_VALVE_ON
#define  F2_VALVE_OFF  CHANGWEN_VALVE_OFF

#define  F3_VALVE_ON   REDAN_VALVE_ON 
#define  F3_VALVE_OFF  REDAN_VALVE_OFF

#define  F4_VALVE_ON   HUILIU_VALVE_ON
#define  F4_VALVE_OFF  HUILIU_VALVE_OFF

#define  F5_VALVE_ON   FEISHUI_VALVE_ON
#define  F5_VALVE_OFF  FEISHUI_VALVE_OFF


extern void Valve_Init(void);



#endif
