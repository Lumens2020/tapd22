#include "define.h"

void Bsp_Init(void)
{ 
   

		rcu_ahb_clock_config( RCU_AHB_CKSYS_DIV1);
	
		systick_config();        //ʱ�ӳ�ʼ��	
	
	 	delay_1ms(500);          //�ϵ�ʹ������״̬�ȶ�������һ��
	
	  Multi_Task_Init();
	
	  Queue_Init(&Queue);      //���г�ʼ��
	
	  Led_Init();
	
//    Timer2_Pwm_Init(0);       //Ƶ��100HZ,period=9999�����ú����24V��
//	 Timer2_Pwm_Init(8800);  
	
	  Motor_Init();              //��ˮ�ã���ˮ��
		
    Adc_Init();                //NTC��ʼ��
		
		Scr_Heat_Init();           //�ɿع�IO
		
		Exti_Init();               //EXTI��ʼ��
		 
		Timer14_Init(72,10);       //��ʱ10US
		
    Relay_Heat_Init();         //�̵���

    Water_Level_Init();        //ˮλ
		
	  Tm1650_Init();             //��ʾ��ʼ��
			    			 
		Key_Init(&KeyS, TIME_5S,TIME_10S,Read_Key_Port);  //����
				
		Valve_Init();              //��
					
//		delay_1ms(3000);         //��ʱ3S����Ϊ��ͷ��һ�����ݲ��ԣ���ʼ��ʱ�����

		Usart0_Task_Init(9600);
			
		Usart1_Task_Init(115200);   //��RFIDͨ��
		Timer15_Init(7200,1000);    //��ʱ100MS
		
		Device_Infor_Init();        //�豸��Ϣ������ʼ��
		 
		Flash_Idev_Init();          //FLASH��ʼ������Ҫ�ŵ����2022.04.13	
	

}


