

#include "define.h"

/*

����ˮλ̽�룬���ø��ӿ���
�͸�Һλ�ڸ���ADC�ɼ��ӿ�(ADC�ɼ��ڣ�PA0-PA7��PB0��PB1,)��ȥ��D25��D26��ѹ��
��TDS = 5ˮ��Ϊ����ˮ������ֵ

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
			//led��
//			HEAT_SWITCH_LED_ON;
		}
		
		if( HIGH_LEVEL_READ ==0 )
		{
			//LED��
//				HEAT_SWITCH_LED_OFF;
		}
}

