
#include "define.h"

 MDU_DATA_TX  volatile  MduDataTx;
 MDU_DATA_RX  volatile  MduDataRx;
 

/*���ݷ�����ʱ������*/
volatile INT8U MduTxProBuf[ProDataNum] = {0x9A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void Mdu_Data_Struct_Init(void)
{
	 memset((void *)&MduDataTx,0,sizeof(MduDataTx));
	 memset((void *)&MduDataRx,0,sizeof(MduDataRx));
}



void Mdu_Data_Send_Packet(INT8U *str,INT8U len)
{

	 /*���ô˺���������ײ㺯��*/	
	 Usart0_Tx_Data(str,len);
}

void Mdu_Data_Update_Infor(void)
{
	 INT16U sum = 0;

	/*FALSE:æ��TRUE:��æ*/
	if( Usart0TaskData.Snd.SndOk == FALSE  )
	{
		return;
	}
	/*�ѷ���ֵ�͵�һ����ʱ������*/
//	 MduTxProBuf[ProData2]            = MduDataRx.DATA.CommandType;
	 MduTxProBuf[ProData2]            = 0X02;
	 MduTxProBuf[ProData4]            = MduDataRx.DATA.KeepTempSet;
	

	/*��ʱ��������ֵ��ֵ�����ͻ�����*/
   MduDataTx.DATA.Head              = MduTxProBuf[ProData0];
	 MduDataTx.DATA.RandomNum         = MduTxProBuf[ProData1];
   MduDataTx.DATA.ReplyType         = MduTxProBuf[ProData2];  //��ѯ:01,���ƣ�02
//   MduDataTx.DATA.MduDataData3.Val  = MduTxProBuf[ProData3];     //bitλ����
	 MduDataTx.DATA.MduDataData3.Val  = 0X01;                        //bitλ����
	 MduDataTx.DATA.ReDanTemState     = MduTxProBuf[ProData4];  //Ĭ�ϣ�0����ˮ��01��������02�����̣�03
	 MduDataTx.DATA.ReDanTemp         = MduTxProBuf[ProData5];  //�ȵ��¶�
	 MduDataTx.DATA.OutWaterType      = MduDataRx.DATA.KeyValue;  //ֹͣ��0������ˮ��1����ˮ��2
	
   MduDataTx.DATA.OriginTdsHigh     = MduTxProBuf[ProData7];  //ԭˮTDSֵ�ĸ�λ
   MduDataTx.DATA.OriginTdsLow      = MduTxProBuf[ProData8];  //ԭˮTDSֵ�ĵ�λ
	 MduDataTx.DATA.CleanTdsHigh      = MduTxProBuf[ProData9];  //��ˮTDSֵ�ĸ�λ
	 MduDataTx.DATA.CleanTdsLow       = MduTxProBuf[ProData10];  //��ˮTDSֵ�ĵ�λ
   MduDataTx.DATA.CpfLifeHigh       = MduTxProBuf[ProData11]; //CPF��о�����ĸ�λ
	 MduDataTx.DATA.CpfLifeLow        = MduTxProBuf[ProData12]; //CPF��о�����ĵ�λ
	 MduDataTx.DATA.DfLifeHigh        = MduTxProBuf[ProData13]; //DF��о�����ĸ�λ
   MduDataTx.DATA.DfLifeLow         = MduTxProBuf[ProData14]; //DF��о�����ĵ�λ 
	
	
   sum =  MduDataTx.DATA.Head          + MduDataTx.DATA.RandomNum    + MduDataTx.DATA.ReplyType    + MduDataTx.DATA.MduDataData3.Val + \
	        MduDataTx.DATA.ReDanTemState + MduDataTx.DATA.ReDanTemp    + MduDataTx.DATA.OutWaterType + MduDataTx.DATA.OriginTdsHigh    + \
	        MduDataTx.DATA.OriginTdsLow  + MduDataTx.DATA.CleanTdsHigh + MduDataTx.DATA.CleanTdsLow  + MduDataTx.DATA.CpfLifeHigh      + \
	        MduDataTx.DATA.CpfLifeLow    + MduDataTx.DATA.CpfLifeHigh +  MduDataTx.DATA.DfLifeLow ;
	        
	
	 MduDataTx.DATA.CheckNum = ( INT8U )sum;                     //0-14�ֽ��ۼӣ�ȡ��8λ
	
	/*��������*/
   Mdu_Data_Send_Packet( (void *)MduDataTx.SndBuf, SND_BUF_NUM );
	
}



/*���ݽ���,ִ�й���*/
void Mdu_Data_Rx_Exe_Func(void)
{
		 	
				 /*����+����ˮ*/
				if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x01 ) )
				{				
							/*״̬*/
							DeviceInfor.InControl.GetWater.ColdWaterPriority = 3;						
							/*������F1��M1��F2��*/
							F1_VALVE_ON;
							M1_ON;
							F2_VALVE_ON;
						 

				}
				 /*����+��ˮ*/
				else if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x02 ) )
				{
						RESHUI_MOTOR_ON_24V;
						RESHUI_MOTOR_ON_24V;
				}
				
				else if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x00 ) )
				{

					
						/*����ˮȡˮֹͣ����־λ��TRUE*/
						 DeviceInfor.InControl.GetWater.StopFlag          = TRUE;
						 DeviceInfor.InControl.GetWater.ColdWaterPriority = 0;								
						 /*F1��M1��F2�ر�*/
						 F1_VALVE_OFF;
						 M1_OFF;
						 RESHUI_MOTOR_OFF_24V;
						 F2_VALVE_OFF;	
				}
			 /*�����趨*/	
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.KeepTempSet == 0x01) )
			{
//				  DeviceInfor.InControl.Keep.Temp95   = KEEP_TEMP_95C;
				  DeviceInfor.InControl.Keep.SetTemp  = KEEP_TEMP_95C;
			}
			else if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.KeepTempSet == 0x02) )
			{
//				  DeviceInfor.InControl.Keep.Temp60   = KEEP_TEMP_60C;
				  DeviceInfor.InControl.Keep.SetTemp  = KEEP_TEMP_60C;
			}
			else if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.KeepTempSet == 0x03) )
			{
//				  DeviceInfor.InControl.Keep.Temp45   = KEEP_TEMP_45C;	 
          DeviceInfor.InControl.Keep.SetTemp  = KEEP_TEMP_45C;				
			}
				/*�е����*/
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.BoilPointClear == 0x01) )
      {
				 	DeviceInfor.InControl.BoilPointStudy.Clear  = 1;
			}
			 /*��������*/
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.FactorySet == 0x01) )
      {
				 	 DeviceInfor.InControl.RecoverFactory.Set = 1; 
			}
		   /*����ģʽ*/	
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.EngineerMode == 0x01) )
      {
				 	DeviceInfor.InControl.EngineerMode.Set = 1;
			}

	  
					  
		/*�ϴ�״̬��Ϣ*/
		Mdu_Data_Update_Infor();
//		gpio_bit_toggle(GPIOC,GPIO_PIN_13);  //ͨ�Ų���
			

}





/*�������ݸ��µ��ô˺���*/
void Mdu_Data_Auto_Update(INT8U idx,INT8U val,INT8U flag)
{
	   INT8U  t = idx;
			if( TRUE == flag )
			{
					flag = FALSE;
				 switch(t) 
				 {
					   
						 case ProData2: MduTxProBuf[ProData2] = val;break;   //Ӧ����������
						 case ProData3: MduTxProBuf[ProData3] = val;break;   //����3
						 case ProData4: MduTxProBuf[ProData4] = val;break;   //�ȵ��¶�����״̬
						 case ProData5: MduTxProBuf[ProData5] = val;break;   //�ȵ��¶�
						 case ProData6: MduTxProBuf[ProData6] = val;break;   //��ˮ���� 
						 case ProData7: MduTxProBuf[ProData7] = val;break;   //ԭˮTDSֵ�ĸ�λ
						 case ProData8: MduTxProBuf[ProData8] = val;break;   //ԭˮTDSֵ�ĵ�λ
						 case ProData9: MduTxProBuf[ProData9] = val;break;   //��ˮTDSֵ�ĸ�λ
						 case ProData10: MduTxProBuf[ProData10] = val;break; //��ˮTDSֵ�ĸ�λ
						 case ProData11: MduTxProBuf[ProData11] = val;break; //CPF��о�����ĸ�λ
						 case ProData12: MduTxProBuf[ProData12] = val;break; //CPF��о�����ĵ�λ
						 case ProData13: MduTxProBuf[ProData13] = val;break; //DF��о�����ĸ�λ
						 case ProData14: MduTxProBuf[ProData14] = val;break; //DF��о�����ĵ�λ
						 
						 default:break;
						
				 }	
			}		 
		
}



void Mdu_Data_Update_Task(void *pMsg)
{
	 pMsg = pMsg;
	 
	 /*�ϴ�״̬��Ϣ*/
	 Mdu_Data_Update_Infor();

	 
}


