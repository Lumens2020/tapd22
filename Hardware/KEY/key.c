
#include "define.h"
#include "gd32e23x_it.h"

KEY_S KeyS;
MSG_QUEUE_S Queue0;  //����
MSG_QUEUE_S Queue1;  //CPF
MSG_QUEUE_S Queue2;  //DF
MSG_QUEUE_S Queue3;  //Ӧ��


PORT_UNION PortUnion;


void Key_Init(KEY_S *p, INT8U lTime0,INT16U lTime1,PFUN_KEY_READ pf)
{
	   INT8U i;
	
		 p->KeyDown = 0x00;
		 p->KeyF    = 0x00;
		 p->KeyFall = 0x00;
		 p->KeyS1   = ~0x00;  //����Ϊ1����������ʼ״̬
		 p->KeyS2   = ~0x00;  //����Ϊ1����������ʼ״̬
		 p->pf      = pf;     //�ص������ӿ�
		
	   p->KeyLF   = 0x00;
	   p->KeyLTime0= lTime0;
	   p->KeyLTime1= lTime1; 

		 for(i=0; i<KEY_NUM; i++)
		 {
			 p->KeyLCnt[i] = 0x00;
		 }
				
		 MsgQueue_Init(&Queue0);
		 Queue0.Id[0] = HEAT_SWITCH_KEY_L_EVENT;
		 Queue0.Id[1] = 0XFF;                     //�������õ���
		
		 MsgQueue_Init(&Queue1);
		 Queue1.Id[0] = CPF_RESET_KEY_L_EVENT;
		 Queue1.Id[1] = 0XFF;
		
		 MsgQueue_Init(&Queue2);
		 Queue2.Id[0] = DF_RESET_KEY_L_EVENT;
		 Queue2.Id[1] = 0XFF;
		 
		 MsgQueue_Init(&Queue3);
		 Queue3.Id[0] = YINGJI_SWITCH_KEY_L_EVENT;
		 Queue3.Id[1] = 0XFF;
		
		 /*������*/
		 p->Subject[0] = &Queue0;
		 p->Subject[1] = &Queue1;
		 p->Subject[2] = &Queue2;
		 p->Subject[3] = &Queue3;
		
	   Key_Port_Init();    //����IO��ʼ��        
}
/*S2,S1,S0����״̬*/
void Key_Scan( KEY_S *p, INT8U keys0 )
{
	 p->KeyDown = (~p->KeyS1) & (~keys0);     //��ⰴ����ס
	 p->KeyFall =  p->KeyDown & p->KeyS2;     //��������
	 p->KeyF   |=  p->KeyFall;                //���水��״̬
	
	 p->KeyS2   = p->KeyS1;
	 p->KeyS1   = keys0;
	
}



/*����*/
void Key_LScan(KEY_S *p)
{
	  INT8U  i = 0;
	  static INT8U  combinationflag = 0;
	
	  for(i=0; i<KEY_NUM; i++ )
		{
			
					if( p->KeyDown & (1<<i) )                 //����p->keyDownΪ1��
				 {
					  p->KeyLCnt[i]++;
					   /*����������ס2S���ϴ�����*/
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
									if( p->KeyLCnt[i] == p->KeyLTime0 )     //����ʱ�䵽
								{
										p->KeyLF |= (1<<i);                   //������־λ��1
									  
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

/*��ȡ����IO*/
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


/*����ģʽ*/
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
			  num = *pKeyF;         //��������ֵ
			

	    
			  /*�����м���������*/
			  len = sizeof(p->Subject)/sizeof(MSG_QUEUE_S *);
			 
			  for(i=0; i<len; i++)
			 {
						pQ = p->Subject[i];
					 
						if( ( pQ != MSG_NULL ) && Is_MsgQueue_Full(pQ) == FALSE )
						{
											/*����*/					
										 if( num == HEAT_SWITCH_KEY_CODE )
										 {
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
		                 	 /*�������*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 	/*CPF*/		
										  else if( num == CPF_RESET_KEY_CODE )
										 {
											 i       += 1;
											 pQ       = pQ + i;  //ָ�� p->Subject[1]
											 msg.Code = num;
											 msg.Time = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*�������*/
									     Save_MsgQueue(pQ,&msg);
										   break;
										 }
										 	/*DF*/		
										 else if( num == DF_RESET_KEY_CODE )
										 {
											 i         += 2;
											 pQ        = pQ + i; //ָ�� p->Subject[2]
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*�������*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 /*Ӧ��*/										 							 
										 else if( num == YINGJI_SWITCH_KEY_CODE )
										 {
											 i         += 3;
											 pQ        = pQ + i; //ָ�� p->Subject[3]
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = p->Subject[i]->Id[0];
											 /*�������*/
											
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
										 
										 /*����+Ӧ��*/
										 else if( num == COMBINATION_KEY_CODE )
										 {
											 msg.Code  = num;
											 msg.Time  = TimeStamp;
											 msg.Event = COMBINATION_KEY_L_EVENT;
											 /*�������*/
									     Save_MsgQueue(pQ,&msg);
											 break;
										 }
																														
						}
			 }
			 
			 /*������־��0*/
			 *pKeyF &= ~(num);
			 
		}
	 
	 
}

void Key_Main_Task(void *pMsg)
{
	
	 KEY_S *p = (KEY_S *)pMsg;
	 Key_Scan(p,p->pf());  //p->pf�ص���Read_Key_Port
	 Key_LScan(p);         //����ɨ��      
	
	  /*����ģʽ*/
	 if( p->KeyLF )        //ֻҪ�а������£�p->KeyLF��Ϊ0
	 Creat_Subject_Msg( p, &p->KeyLF);         
	 
   	 
}

/*���հ����ж�*/
void Combination_Key_Task(void *pMsg)
{
	 KEY_S *p = (KEY_S *)pMsg;
	
	 MSG_S msg;
	
	 INT8U i,len;
	
	 MSG_QUEUE_S *pQ;
	
	 
	   /*�����м���������*/
			len = sizeof(p->Subject)/sizeof(MSG_QUEUE_S *);
		 
			for(i=0; i<len; i++)
		 {
				pQ = p->Subject[i];
			 
				if( MsgQueue_Num(pQ) )
				{
					     Get_MsgQueue(pQ,&msg );
		
							if( ( msg.Code == HEAT_SWITCH_KEY_CODE )&& ( msg.Event == HEAT_SWITCH_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
								   //��������һ��
								   /*���ȹر�*/
								   DeviceInfor.OutControl.ReDan.StopHeatFlag ^= TRUE;           //����ȡ��
							}
							
							else if( ( msg.Code == CPF_RESET_KEY_CODE ) && ( msg.Event == CPF_RESET_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
							}
					
							else if( ( msg.Code == DF_RESET_KEY_CODE ) && ( msg.Event == DF_RESET_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
									
							}
							else if( ( msg.Code == YINGJI_SWITCH_KEY_CODE ) && ( msg.Event == YINGJI_SWITCH_KEY_L_EVENT ) )
							{
//									gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
							}
							
							else if( ( msg.Code == COMBINATION_KEY_CODE ) && ( msg.Event == COMBINATION_KEY_L_EVENT ) )
							{
//								  gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //��˸��֤ͨ��
							}
					
				}
		 }
			 
}

