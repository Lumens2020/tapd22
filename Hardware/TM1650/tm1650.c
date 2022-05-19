
#include "define.h"


///*开始信号：保持 CLK 为“1”电平，DAT 从“1”跳“0”，认为是开始信号*/
void Tm1650_Start(void)
{
	
		TM1650_SCLK_H;
		TM1650_DIN_H;
	  Delay_Us(15);  //实测4.9us
		TM1650_DIN_L;

}
/*结束信号:CLK 为“1”电平，DAT 从“0”跳“1”，认为是结束信号*/
void Tm1650_Stop(void)
{
    
		TM1650_SCLK_H;
		TM1650_DIN_L;
	  Delay_Us(15);  //实测4.9us
	  TM1650_DIN_H;
}

void Wait_Ack(void)
{
	 	 
	INT8U timeout = 1;
	SDA_IN_MODE();     //设置为输入模式
	TM1650_SCLK_H;
   Delay_Us(5);      //实测2.2US
	TM1650_SCLK_L;
	while((TM1650_DIN_IN)&&(timeout<=200))
	{
	  timeout++;	
	
	}

	 Delay_Us(5);
	 TM1650_SCLK_L;
	 SDA_OUT_MODE();   //设置为输出模式
}

	/*CLK 从“0”跳到“1”,再从“1”跳到“0”*/
void Tm1650_Write(INT8U dat)  //写数据
{
	
	 INT8U i; 
	
   for(i=0;i<8;i++) 
   { 
      if(dat&0x80) 
       TM1650_DIN_H;
      else TM1650_DIN_L; 
			
			dat=dat<<1; 
						
			TM1650_SCLK_L; 
			Delay_Us(5);    //实测2.2US
			TM1650_SCLK_H;	
			Delay_Us(5);
			TM1650_SCLK_L; 	
			Delay_Us(5);
		
   } 
	

}

void TM1650_Set(INT8U add,INT8U dat)
{
	Tm1650_Start();
	Tm1650_Write(add); 
	Wait_Ack();
	Tm1650_Write(dat);
	Wait_Ack();
	Tm1650_Stop();
}




/*模块结构体初始化*/
 void Tm1650_Struct_Init(void)
{

 	    TM1650_Set(0x48,TM1650MEDO_DISPLAY_8);
//	    TM1650_Set(0x68,0xff);
//	    TM1650_Set(0x6A,0xff);


}	   


/*
DIN -- PB1
SCLK -- PB2
*/

void Tm1650_Init(void)
{
	
	//IO设置为推挽输出	
	rcu_periph_clock_enable(RCU_GPIOB); 
	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_2);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);
	gpio_bit_reset(GPIOB,GPIO_PIN_2);
	
	SDA_OUT_MODE();
	TM1650_DIN_L;

	Tm1650_Struct_Init();
	
	
}





