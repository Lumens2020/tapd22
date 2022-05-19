#ifndef _KEY_H
#define _KEY_H


#include "stdint.h"
#include "msgqueue.h"
//  布尔代数最小项原理;S0第一状态，S1第二状态，S2第三状态
//  100  y= keyS2 &  ~ keyS1 & ~keyS0  检测按键按下
//  100 000  y = (keyS2 &  ~ keyS1 & ~keyS0) | (~keyS2 &  ~ keyS1 & ~keyS0) //检测按键按住
//             = (~ keyS1 & ~keyS0)(keyS2 | ~keyS2)
//             = (~ keyS1 & ~keyS0)     //检测按键按住


/*长按事件*/
#define HEAT_SWITCH_KEY_L_EVENT    0X01    
#define CPF_RESET_KEY_L_EVENT      0X02                     
#define DF_RESET_KEY_L_EVENT       0X04
#define YINGJI_SWITCH_KEY_L_EVENT  0X08 
#define COMBINATION_KEY_L_EVENT    0X09   

/*长按按键码*/
#define HEAT_SWITCH_KEY_CODE       0X01    
#define CPF_RESET_KEY_CODE         0X02                     
#define DF_RESET_KEY_CODE          0X04
#define YINGJI_SWITCH_KEY_CODE     0X08 
#define COMBINATION_KEY_CODE       0X03    //调试0X03，最后改为0X09

#define INTERVAL_NUM               0X01    //调试0X01，最后改为0X03


#define  KEY_BIT0  gpio_input_bit_get(GPIOA, GPIO_PIN_4)
#define  KEY_BIT1  gpio_input_bit_get(GPIOA, GPIO_PIN_5)
#define  KEY_BIT2  gpio_input_bit_get(GPIOA, GPIO_PIN_6)
#define  KEY_BIT3  1
#define  KEY_BIT4  1
#define  KEY_BIT5  1
#define  KEY_BIT6  1
#define  KEY_BIT7  1


#define  KEY_NUM 4               //按键数目
#define  KEY_S_NULL  (KEY_S *)0

typedef INT8U (*PFUN_KEY_READ)(void);

typedef struct{
	
	INT8U KeyS2;
	INT8U KeyS1;
	INT8U KeyFall;
	INT8U KeyF;
	INT8U KeyDown;
	PFUN_KEY_READ pf;  //函数指针
	
	INT8U KeyLF;
	INT16U KeyLCnt[KEY_NUM];
	INT8U KeyLTime0;
	INT16U KeyLTime1;
 
	MSG_QUEUE_S *Subject[KEY_NUM];  //主题栏
	
}KEY_S;

extern KEY_S KeyS;


typedef union{
	
	INT8U Port;
	struct{
		
		INT8U Bit0:1;
		INT8U Bit1:1;
		INT8U Bit2:1;
		INT8U Bit3:1;
		INT8U Bit4:1;
		INT8U Bit5:1;
		INT8U Bit6:1;
		INT8U Bit7:1;
	  
	}BITS;
	
}PORT_UNION;

extern PORT_UNION PortUnion;



extern void Key_Init(KEY_S *p, INT8U lTime0,INT16U lTime1,PFUN_KEY_READ pf);
extern void Key_Scan( KEY_S *p, INT8U keys0 );
extern void Key_LScan(KEY_S *p);
extern void Key_Main_Task(void *pMsg);
extern void Key_Port_Init(void);
extern INT8U Read_Key_Port(void);
extern void  Combination_Key_Task(void *pMsg);

#endif
