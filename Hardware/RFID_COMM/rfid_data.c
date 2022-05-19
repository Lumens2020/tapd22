
#include "define.h"

RFID_DATA_TX volatile  RfidDataTx;
RFID_DATA_RX volatile  RfidDataRx;


 /*数据发送临时缓存区*/
const INT8U RfidTxProBuf[USART1_SND_BUF_SIZE] = {0x00,0x00,0xFF,0x04,0xFC,0xD4,0x4A,0x01,0x00,0xE1,0x00};

static INT8U CardMess[4] = {0};

void Rfid_Data_Struct_Init(void)
{
	 memset((void *)&RfidDataTx,0,sizeof(RfidDataTx));
	 memset((void *)&RfidDataRx,0,sizeof(RfidDataRx));
}

void Rfid_Send_Packet(INT8U *str,INT8U len)
{

	 /*调用此函数串口最底层函数*/	
	 Usart1_Tx_Data(str,len);
}




/*串口1发送数据给RFID*/
void Rfid_Data_Update_Infor(void)
{
	 INT8U i = 0;
	 INT16U sum = 0;
	 /*使用指针必须给指针一个初始化*/
	 RFID_DATA_TX *p_ctrl;
  
	/*FALSE:忙，TRUE:不忙*/
	if( Usart1TaskData.Snd.SndOk == FALSE )
	{
		return;
	}
	
 	p_ctrl = (RFID_DATA_TX *)&RfidDataTx;   //指针初始化
	
	 /*添加更新的数据*/
	 p_ctrl->Head              = RfidTxProBuf[ProData0];
	 p_ctrl->Data2             = RfidTxProBuf[ProData1];
   p_ctrl->Data3             = RfidTxProBuf[ProData2];  
   p_ctrl->Data4             = RfidTxProBuf[ProData3];  
	 p_ctrl->Data5             = RfidTxProBuf[ProData4]; 
	 p_ctrl->Data6             = RfidTxProBuf[ProData5];  
   p_ctrl->Data7             = RfidTxProBuf[ProData6];  
	 p_ctrl->Data8             = RfidTxProBuf[ProData7];  
	 p_ctrl->Data9             = RfidTxProBuf[ProData8];  
   p_ctrl->Data10            = RfidTxProBuf[ProData9]; 
	 p_ctrl->EndData           = RfidTxProBuf[ProData10]; 

	
	/*发送数据*/
   Rfid_Send_Packet((INT8U *)p_ctrl,ProData10+1);
	
}


/*接收物理层数据*/
void Rfid_Data_Rx_Ok(INT8U *buf, INT8U len)
{
		 INT8U i = 0;
	   INT16U sum = 0;
	
		 OS_ENTER_CRITICAL();			 
		 for(i=0; i<len; i++ )
		{
			 RfidDataRx.RecBuf[i] = buf[i];
		}		
		OS_EXIT_CRITICAL();	
		
		/*无卡*/
		if(len <= RFID_NO_CARD_REC_NUM )
		{
			return;
		}
		/*有卡*/
		else if( len >= RFID_CARD_REC_NUM)
		{
				 len = RFID_CARD_REC_NUM;
				 for(i=11; i<len-1; i++)
				{
						 sum +=RfidDataRx.RecBuf[i]; 
				}
				
				sum = 0x100 - (INT8U)sum;
				sum %= 0x100;
		}

		/*有卡校验和判断*/
		if( sum == RfidDataRx.DATA.CheckNum )
		{
	     /*处理命令信息*/
			
			 /*提取卡号信息*/ 
       CardMess[0] = RfidDataRx.DATA.CardData1;
			 CardMess[1] = RfidDataRx.DATA.CardData2;
			 CardMess[2] = RfidDataRx.DATA.CardData3;
			 CardMess[3] = RfidDataRx.DATA.CardData4;
			
			 memset((void *)RfidDataRx.RecBuf,0,RFID_CARD_REC_NUM);      //保证每次数据清0
			
//			 gpio_bit_toggle(GPIOC,GPIO_PIN_13);                         //闪烁验证通信
		}
		else
		{

			 return;
		}
		

}



