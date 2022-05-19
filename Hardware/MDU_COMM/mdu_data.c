
#include "define.h"

 MDU_DATA_TX  volatile  MduDataTx;
 MDU_DATA_RX  volatile  MduDataRx;
 

/*数据发送临时缓存区*/
volatile INT8U MduTxProBuf[ProDataNum] = {0x9A,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void Mdu_Data_Struct_Init(void)
{
	 memset((void *)&MduDataTx,0,sizeof(MduDataTx));
	 memset((void *)&MduDataRx,0,sizeof(MduDataRx));
}



void Mdu_Data_Send_Packet(INT8U *str,INT8U len)
{

	 /*调用此函数串口最底层函数*/	
	 Usart0_Tx_Data(str,len);
}

void Mdu_Data_Update_Infor(void)
{
	 INT16U sum = 0;

	/*FALSE:忙，TRUE:不忙*/
	if( Usart0TaskData.Snd.SndOk == FALSE  )
	{
		return;
	}
	/*把发送值送到一个临时缓冲区*/
//	 MduTxProBuf[ProData2]            = MduDataRx.DATA.CommandType;
	 MduTxProBuf[ProData2]            = 0X02;
	 MduTxProBuf[ProData4]            = MduDataRx.DATA.KeepTempSet;
	

	/*临时缓冲区的值赋值给发送缓冲区*/
   MduDataTx.DATA.Head              = MduTxProBuf[ProData0];
	 MduDataTx.DATA.RandomNum         = MduTxProBuf[ProData1];
   MduDataTx.DATA.ReplyType         = MduTxProBuf[ProData2];  //查询:01,控制：02
//   MduDataTx.DATA.MduDataData3.Val  = MduTxProBuf[ProData3];     //bit位计算
	 MduDataTx.DATA.MduDataData3.Val  = 0X01;                        //bit位计算
	 MduDataTx.DATA.ReDanTemState     = MduTxProBuf[ProData4];  //默认：0，开水：01，热饮：02，冲奶：03
	 MduDataTx.DATA.ReDanTemp         = MduTxProBuf[ProData5];  //热胆温度
	 MduDataTx.DATA.OutWaterType      = MduDataRx.DATA.KeyValue;  //停止：0，常温水：1，热水：2
	
   MduDataTx.DATA.OriginTdsHigh     = MduTxProBuf[ProData7];  //原水TDS值的高位
   MduDataTx.DATA.OriginTdsLow      = MduTxProBuf[ProData8];  //原水TDS值的低位
	 MduDataTx.DATA.CleanTdsHigh      = MduTxProBuf[ProData9];  //净水TDS值的高位
	 MduDataTx.DATA.CleanTdsLow       = MduTxProBuf[ProData10];  //净水TDS值的低位
   MduDataTx.DATA.CpfLifeHigh       = MduTxProBuf[ProData11]; //CPF滤芯寿命的高位
	 MduDataTx.DATA.CpfLifeLow        = MduTxProBuf[ProData12]; //CPF滤芯寿命的低位
	 MduDataTx.DATA.DfLifeHigh        = MduTxProBuf[ProData13]; //DF滤芯寿命的高位
   MduDataTx.DATA.DfLifeLow         = MduTxProBuf[ProData14]; //DF滤芯寿命的低位 
	
	
   sum =  MduDataTx.DATA.Head          + MduDataTx.DATA.RandomNum    + MduDataTx.DATA.ReplyType    + MduDataTx.DATA.MduDataData3.Val + \
	        MduDataTx.DATA.ReDanTemState + MduDataTx.DATA.ReDanTemp    + MduDataTx.DATA.OutWaterType + MduDataTx.DATA.OriginTdsHigh    + \
	        MduDataTx.DATA.OriginTdsLow  + MduDataTx.DATA.CleanTdsHigh + MduDataTx.DATA.CleanTdsLow  + MduDataTx.DATA.CpfLifeHigh      + \
	        MduDataTx.DATA.CpfLifeLow    + MduDataTx.DATA.CpfLifeHigh +  MduDataTx.DATA.DfLifeLow ;
	        
	
	 MduDataTx.DATA.CheckNum = ( INT8U )sum;                     //0-14字节累加，取低8位
	
	/*发送数据*/
   Mdu_Data_Send_Packet( (void *)MduDataTx.SndBuf, SND_BUF_NUM );
	
}



/*数据接收,执行功能*/
void Mdu_Data_Rx_Exe_Func(void)
{
		 	
				 /*控制+常温水*/
				if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x01 ) )
				{				
							/*状态*/
							DeviceInfor.InControl.GetWater.ColdWaterPriority = 3;						
							/*动作，F1、M1、F2打开*/
							F1_VALVE_ON;
							M1_ON;
							F2_VALVE_ON;
						 

				}
				 /*控制+热水*/
				else if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x02 ) )
				{
						RESHUI_MOTOR_ON_24V;
						RESHUI_MOTOR_ON_24V;
				}
				
				else if( ( MduDataRx.DATA.CommandType == 0x02 ) && ( MduDataRx.DATA.KeyValue == 0x00 ) )
				{

					
						/*常温水取水停止，标志位置TRUE*/
						 DeviceInfor.InControl.GetWater.StopFlag          = TRUE;
						 DeviceInfor.InControl.GetWater.ColdWaterPriority = 0;								
						 /*F1、M1、F2关闭*/
						 F1_VALVE_OFF;
						 M1_OFF;
						 RESHUI_MOTOR_OFF_24V;
						 F2_VALVE_OFF;	
				}
			 /*保温设定*/	
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
				/*沸点清除*/
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.BoilPointClear == 0x01) )
      {
				 	DeviceInfor.InControl.BoilPointStudy.Clear  = 1;
			}
			 /*工厂设置*/
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.FactorySet == 0x01) )
      {
				 	 DeviceInfor.InControl.RecoverFactory.Set = 1; 
			}
		   /*工程模式*/	
			 if( ( MduDataRx.DATA.CommandType == 0x02 )&&( MduDataRx.DATA.EngineerMode == 0x01) )
      {
				 	DeviceInfor.InControl.EngineerMode.Set = 1;
			}

	  
					  
		/*上传状态信息*/
		Mdu_Data_Update_Infor();
//		gpio_bit_toggle(GPIOC,GPIO_PIN_13);  //通信测试
			

}





/*外面数据更新调用此函数*/
void Mdu_Data_Auto_Update(INT8U idx,INT8U val,INT8U flag)
{
	   INT8U  t = idx;
			if( TRUE == flag )
			{
					flag = FALSE;
				 switch(t) 
				 {
					   
						 case ProData2: MduTxProBuf[ProData2] = val;break;   //应答数据类型
						 case ProData3: MduTxProBuf[ProData3] = val;break;   //数据3
						 case ProData4: MduTxProBuf[ProData4] = val;break;   //热胆温度设置状态
						 case ProData5: MduTxProBuf[ProData5] = val;break;   //热胆温度
						 case ProData6: MduTxProBuf[ProData6] = val;break;   //出水类型 
						 case ProData7: MduTxProBuf[ProData7] = val;break;   //原水TDS值的高位
						 case ProData8: MduTxProBuf[ProData8] = val;break;   //原水TDS值的低位
						 case ProData9: MduTxProBuf[ProData9] = val;break;   //出水TDS值的高位
						 case ProData10: MduTxProBuf[ProData10] = val;break; //出水TDS值的高位
						 case ProData11: MduTxProBuf[ProData11] = val;break; //CPF滤芯寿命的高位
						 case ProData12: MduTxProBuf[ProData12] = val;break; //CPF滤芯寿命的低位
						 case ProData13: MduTxProBuf[ProData13] = val;break; //DF滤芯寿命的高位
						 case ProData14: MduTxProBuf[ProData14] = val;break; //DF滤芯寿命的低位
						 
						 default:break;
						
				 }	
			}		 
		
}



void Mdu_Data_Update_Task(void *pMsg)
{
	 pMsg = pMsg;
	 
	 /*上传状态信息*/
	 Mdu_Data_Update_Infor();

	 
}


