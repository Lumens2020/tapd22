
#ifndef _TM1650_H
#define _TM1650_H

#include "stdint.h"

/*输出*/
#define  TM1650_DIN_H    gpio_bit_set(GPIOB,GPIO_PIN_1)
#define  TM1650_DIN_L    gpio_bit_reset(GPIOB,GPIO_PIN_1)
/*输入*/
#define  TM1650_DIN_IN   gpio_input_bit_get(GPIOB, GPIO_PIN_1) 
/*输出*/
#define  TM1650_SCLK_H   gpio_bit_set(GPIOB,GPIO_PIN_2)
#define  TM1650_SCLK_L   gpio_bit_reset(GPIOB,GPIO_PIN_2)


 

#define  SDA_OUT_MODE() { rcu_periph_clock_enable(RCU_GPIOB); \
	                        gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);\
	                        gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_1);\
                        }
#define  SDA_IN_MODE()  { rcu_periph_clock_enable(RCU_GPIOB);\
                          gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);\
                        }

												

												

												
/*数据命令设置*/
#define TM1650MEDO_ADD        0X68   //固定地址模式

/*显示控制命令设置*/
  
#define TM1650MEDO_DISPLAY_1  0X11    //亮度最小，
#define TM1650MEDO_DISPLAY_2  0X21 
#define TM1650MEDO_DISPLAY_3  0X31
#define TM1650MEDO_DISPLAY_4  0X41 
#define TM1650MEDO_DISPLAY_5  0X51 
#define TM1650MEDO_DISPLAY_6  0X61
#define TM1650MEDO_DISPLAY_7  0X71    
#define TM1650MEDO_DISPLAY_8  0X01    //亮度最大，开显示

#define TM1650MEDO_DISPLAY_OFF  0X00  //亮度，关显示





extern void Tm1650_Init(void);
extern void Tm1650_Start(void);
extern void Tm1650_Stop(void);
extern void Wait_Ack(void);
extern void Tm1650_Write(INT8U dat);
extern void TM1650_Set(INT8U add,INT8U dat);


#endif
