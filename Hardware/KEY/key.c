
#include "define.h"
#include "gd32e23x_it.h"

KEY_S KeyS;
MSG_QUEUE_S Queue0;  //加热
MSG_QUEUE_S Queue1;  //CPF
MSG_QUEUE_S Queue2;  //DF
MSG_QUEUE_S Queue3;  //应急


PORT_UNION PortUnion;


void Key_Init(KEY_S *p, INT8U lTime0,INT16U lTime1,PFUN_KEY_READ pf)
{
	   INT8U i;
	
		 p->KeyDown = 0x00;
		 p->KeyF    = 0x00;
		 p->KeyFall = 0x00;
		 p->KeyS1   = ~0x00;  //设置为1，代表按键初始状态
		 p->KeyS2   = ~0x00;  //设置为1，代表按键初始状态
		 p->pf      = pf;     //回调按键接口
		
	   p->KeyLF   = 0x00;
	   p->KeyLTime0= lTime0;
	   p->KeyLTime1= lTime1; 

		 for(i=0; i<KEY_NUM; i++)
		 {
			 p->KeyLCnt[i] = 0x00;
		 }
				
		 MsgQueue_Init(&Queue0);
		 Queue0.Id[0] = HEAT_SWITCH_KEY_L_EVENT;
		 Queue0.Id[1] = 0XFF;                     //可以设置单按
		
		 MsgQueue_Init(&Queue1);
		 Queue1.Id[0] = CPF_RESET_KEY_L_EVENT;
		 Queue1.Id[1] = 0XFF;
		
		 MsgQueue_Init(&Queue2);
		 Queue2.Id[0] = DF_RESET_KEY_L_EVENT;
		 Queue2.Id[1] = 0XFF;
		 
		 MsgQueue_Init(&Queue3);
		 Queue3.Id[0] = YINGJI_SWITCH_KEY_L_EVENT;
		 Queue3.Id[1] = 0XFF;
		
		 /*主题栏*/
		 p->Subject[0] = &Queue0;
		 p->Subject[1] = &Queue1;
		 p->Subject[2] = &Queue2;
		 p->Subject[3] = &Queue3;
		
	   Key_Port_Init();    //按键IO初始化        
}
/*S2,S1,S0按键状态*/
void Key_Scan( KEY_S *p, INT8U keys0 )
{
	 p->KeyDown = (~p->KeyS1) & (~keys0);     //检测按键按住
	 p->KeyFall =  p->KeyDown & p->KeyS2;     //按键按下
	 p->KeyF   |=  p->KeyFall;                //保存按键状态
	
	 p->KeyS2   = p->KeyS1;
	 p->KeyS1   = keys0;
	
}



/*长按*/
void Key_LScan(KEY_S *p)
{
	  INT8U  i = 0;
	  static INT8U  combinationflag = 0;
	
	  for(i=0; i<KEY_NUM; i++ )
		{
			
					if( p->KeyDown & (1<<i) )                 //按下p->keyDown为1，
				 {
					  p->KeyLCnt[i]++;
					   /*两个按键按住2S以上代表长按*/
					  if( p->KeyLCnt[0] > TIME_2S && p->KeyLCnt[0+INTERVAL_NUM] > TIME_2S && ( combinationflag == 0 ) )
						{
							 combinationflag = 1;
               p->KeyLCnt[0] = TIME_2S;
							 p->KeyLCnt[0+INTERVAL_NUM] = TIME_2S; 
						}
				    
						 if( combinationflag )
						 {
									if(  p->KeyLCnt[i] == p->KeyLTime1  )
								{
										p->KeyLF |= (1<<i); 
                    p->KeyLF |= (2<<i); 	
                    p->KeyLCnt[i] = 0;
							      p->KeyLCnt[i+INTERVAL_NUM] = 0; 									
										combinationflag = 0;
								}
						 }
						 else
						 {
									if( p->KeyLCnt[i] == p->KeyLTime0 )     //长按时间到
								{
										p->KeyLF |= (1<<i);                   //长按标志位置1
									  
								}
						 }
	
				 }
				 else
				 {
						p->KeyLCnt[i] = 0x00;
					 

				 }
			 
		}
	
}



/*
HEAT_SWITCH -- PA4
CPF_RESET   -- PA5
DF_RESET    -- PA6
*/
void Key_Port_Init(void)
{
	 
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

}

/*读取按键IO*/
INT8U Read_Key_Port(void)
{
	 PortUnion.BITS.Bit0 = KEY_BIT0;
	 PortUnion.BITS.Bit1 = KEY_BIT1;
	 PortUnion.BITS.Bit2 = KEY_BIT2;
	 PortUnion.BITS.Bit3 = KEY_BIT3;
	 PortUnion.BITS.Bit4 = KEY_BIT4;
	 PortUnion.BITS.Bit5 = KEY_BIT5;
	 PortUnion.BITS.Bit6 = KEY_BIT6;
	 PortUnion.BITS.Bit7 = KEY_BIT7;
	 
	 return PortUnion.Port;
}


/*主题模式*/
void Creat_Subject_Msg(KEY_S *p, INT8U *pKeyF)
{
	  MSG_S msg; 
	  INT8U num;
	  MSG_QUEUE_S *pQ;
	  INT8U i,len;
	
	  msg.Code   = 0;
	  msg.Event  = 0;
	  msg.Time   = 0;
	
	  if( *pKeyF )
		{
			  num = *pKeyF;         //长按按键值
			

	    
			  /*计算有几个主题栏*/
			  len = sizeof(p->Subject)/sizeof(MSG_QUEUE_S *);
			 
			  for(i=0; i<len; i++)
			 {
						pQ = p->Subject[i];
					 
						if( ( pQ != MSG_NULL ) && Is_MsgQueue_Full(pQ) == FALSE )
						{
											/*加热*/					
										 if( num == HEAT_SWITCH_KEY_CODE )
										 {
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
		                 	 /*存入队列*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 	/*CPF*/		
										  else if( num == CPF_RESET_KEY_CODE )
										 {
											 i       += 1;
											 pQ       = pQ + i;  //指向 p->Subject[1]
											 msg.Code = num;
											 msg.Time = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*存入队列*/
									     Save_MsgQueue(pQ,&msg);
										   break;
										 }
										 	/*DF*/		
										 else if( num == DF_RESET_KEY_CODE )
										 {
											 i         += 2;
											 pQ        = pQ + i; //指向 p->Subject[2]
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*存入队列*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 /*应急*/										 							 
										 else if( num == YINGJI_SWITCH_KEY_CODE )
										 {
											 i         += 3;
											 pQ        = pQ + i; //指向 p->Subject[3]
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*存入队列*/
											
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 
										 /*加热+应急*/
										 else if( num == COMBINATION_KEY_CODE )
										 {
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = COMBINATION_KEY_L_EVENT;
											 /*存入队列*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
																														
						}
			 }
			 
			 /*按键标志清0*/
			 *pKeyF &= ~(num);
			 
		}
	 
	 
}

void Key_Main_Task(void *pMsg)
{
	
	 KEY_S *p = (KEY_S *)pMsg;
	 Key_Scan(p,p->pf());  //p->pf回调的Read_Key_Port
	 Key_LScan(p);         //长按扫描      
	
	  /*主题模式*/
	 if( p->KeyLF )        //只要有按键按下，p->KeyLF不为0
	 Creat_Subject_Msg( p, &p->KeyLF);         
	 
   	 
}

/*接收按键判断*/
void Combination_Key_Task(void *pMsg)
{
	 KEY_S *p = (KEY_S *)pMsg;
	
	 MSG_S msg;
	
	 INT8U i,len;
	
	 MSG_QUEUE_S *pQ;
	
	 
	   /*计算有几个主题栏*/
			len = sizeof(p->Subject)/sizeof(MSG_QUEUE_S *);
		 
			for(i=0; i<len; i++)
		 {
				pQ = p->Subject[i];
			 
				if( MsgQueue_Num(pQ) )
				{
					     Get_MsgQueue(pQ,&msg );
		
							if( ( msg.Code == HEAT_SWITCH_KEY_CODE )&& ( msg.Event == HEAT_SWITCH_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
								   //蜂鸣器嘀一声
								   /*加热关闭*/
								   DeviceInfor.OutControl.ReDan.StopHeatFlag ^= TRUE;           //来回取反
							}
							
							else if( ( msg.Code == CPF_RESET_KEY_CODE ) && ( msg.Event == CPF_RESET_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
							}
					
							else if( ( msg.Code == DF_RESET_KEY_CODE ) && ( msg.Event == DF_RESET_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
									
							}
							else if( ( msg.Code == YINGJI_SWITCH_KEY_CODE ) && ( msg.Event == YINGJI_SWITCH_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
							}
							
							else if( ( msg.Code == COMBINATION_KEY_CODE ) && ( msg.Event == COMBINATION_KEY_L_EVENT ) )
							{
//								  gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
							}
					
				}
		 }
			 
}

