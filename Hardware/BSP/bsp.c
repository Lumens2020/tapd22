#include "define.h"

void Bsp_Init(void)
{ 
   

		rcu_ahb_clock_config( RCU_AHB_CKSYS_DIV1);
	
		systick_config();        //时钟初始化	
	
	 	delay_1ms(500);          //上电使传感器状态稳定，四区一线
	
	  Multi_Task_Init();
	
	  Queue_Init(&Queue);      //队列初始化
	
	  Led_Init();
	
//    Timer2_Pwm_Init(0);       //频率100HZ,period=9999，带泵后输出24V；
//	 Timer2_Pwm_Init(8800);  
	
	  Motor_Init();              //制水泵，热水泵
		
    Adc_Init();                //NTC初始化
		
		Scr_Heat_Init();           //可控硅IO
		
		Exti_Init();               //EXTI初始化
		 
		Timer14_Init(72,10);       //定时10US
		
    Relay_Heat_Init();         //继电器

    Water_Level_Init();        //水位
		
	  Tm1650_Init();             //显示初始化
			    			 
		Key_Init(&KeyS, TIME_5S,TIME_10S,Read_Key_Port);  //按键
				
		Valve_Init();              //阀
					
//		delay_1ms(3000);         //延时3S，因为龙头第一次数据不对，初始化时间晚点

		Usart0_Task_Init(9600);
			
		Usart1_Task_Init(115200);   //与RFID通信
		Timer15_Init(7200,1000);    //定时100MS
		
		Device_Infor_Init();        //设备信息参数初始化
		 
		Flash_Idev_Init();          //FLASH初始化，需要放到最后2022.04.13	
	

}


