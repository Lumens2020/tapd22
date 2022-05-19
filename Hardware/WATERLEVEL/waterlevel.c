

#include "define.h"

/*

采用水位探针，不用浮子开关
低高液位口改用ADC采集接口(ADC采集口：PA0-PA7，PB0，PB1,)，去掉D25，D26稳压管
以TDS = 5水认为是无水，看阻值

*/

/*
HIGH_WATER -- PB12
LOW_WATER  -- PB13
*/
void Water_Level_Init(void)
{
	 
    rcu_periph_clock_enable(RCU_GPIOB);

    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_12|GPIO_PIN_13);

  
}


void Water_Level_Test(void *pMsg)
{
	  if( LOW_LEVEL_READ == 0)
		{ 
			//led亮
//			HEAT_SWITCH_LED_ON;
		}
		
		if( HIGH_LEVEL_READ ==0 )
		{
			//LED灭
//				HEAT_SWITCH_LED_OFF;
		}
}

