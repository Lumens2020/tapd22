#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H


#define HIGH_LEVEL_READ  gpio_input_bit_get(GPIOB, GPIO_PIN_12) 
#define LOW_LEVEL_READ   gpio_input_bit_get(GPIOB, GPIO_PIN_13) 

/*磁簧开关*/
/*
#define HIGH_FULL_WATER  0
#define HIGH_LACK_WATER  1
#define LOW_FULL_WATER   1
#define LOW_LACK_WATER   0
*/

/*液位探针开关*/
#define HIGH_FULL_WATER  0
#define HIGH_LACK_WATER  1
#define LOW_FULL_WATER   0
#define LOW_LACK_WATER   1 


extern void Water_Level_Init(void);
extern void Water_Level_Test(void *pMsg);

#endif